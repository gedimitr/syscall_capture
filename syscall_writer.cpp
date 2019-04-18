#include <cassert>

#include "clocks.hpp"
#include "configuration.hpp"
#include "execution_state.hpp"
#include "file_writer.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "multi_unit_time.hpp"
#include "segments.hpp"
#include "syscall_args.hpp"
#include "syscall_info.hpp"
#include "syscall_record.hpp"
#include "thread_id.hpp"

#include "syscall_writer.hpp"

namespace {

enum SyscallRecordFlag
{
    FLAG_RESULT = 0x01,
    FLAG_THREAD_ID = 0x02,
    FLAG_TIMESTAMP = 0x04,
    FLAG_DURATION = 0x08,
    FLAG_ERRNO = 0x10
};

#include <syscall.h>

uint8_t encodeFlags(const SyscallRecord &syscall_record)
{
    uint8_t flags = 0x00;

    if (syscall_record.result != 0) {
        flags |= FLAG_RESULT;
    }

    if (syscall_record.thread_id) {
        flags |= FLAG_THREAD_ID;
    }

    if (syscall_record.entry_timestamp) {
        flags |= FLAG_TIMESTAMP;
    }

    if (syscall_record.syscall_duration) {
        flags |= FLAG_DURATION;
    }

    if (syscall_record.errnum) {
        flags |= FLAG_ERRNO;
    }

    return flags;
}

void writeSyscallArgument(ManagedBuffer &managed_buffer, const SyscallRecord &syscall_record, int arg_num)
{
    const SyscallArg *syscall_arg = getSyscallArg(syscall_record.syscall_number, arg_num);
    if (!syscall_arg) {
        return;
    }
    ArgType arg_type = syscall_arg->type;

    int64_t arg = syscall_record.args[arg_num];
    switch(arg_type) {
    case ARG_INT:
        writeSyscallArgumentInt(managed_buffer, arg);
        break;
    case ARG_DATA:
        writeSyscallArgumentData(managed_buffer, arg, syscall_record.args[syscall_arg->aux_arg]);
        break;
    default:
        writeSyscallArgumentInt(managed_buffer, arg);
    }
}

}

SyscallWriter::SyscallWriter(const Configuration &configuration, const ExecutionState &execution_state) :
    m_configuration(configuration),
    m_execution_state(execution_state) { }

SyscallRecord SyscallWriter::invokeAndRecord(int64_t syscall_number, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3, int64_t arg4, int64_t arg5)
{
    SyscallRecord record;

    record.syscall_number = syscall_number;
    record.args[0] = arg0;
    record.args[1] = arg1;
    record.args[2] = arg2;
    record.args[3] = arg3;
    record.args[4] = arg4;
    record.args[5] = arg5;

    std::optional<Stopwatch> stopwatch;
    if (m_configuration.shouldRecordSyscallEntryTimestamp() ||
        m_configuration.shouldRecordSyscallDuration()) {
        stopwatch = Stopwatch();
        record.entry_timestamp = stopwatch->getStartTimestamp();
    }

    record.result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4, arg5);
    record.errnum = errno;

    if (m_configuration.shouldRecordSyscallDuration()) {
        assert(stopwatch);
        record.syscall_duration = stopwatch->getElapsedNanoseconds();
    }

    int32_t current_thread_id = getCurrentThreadId();
    if (m_execution_state.m_process_id != current_thread_id) {
        record.thread_id = current_thread_id;
    }

    return record;
}

void writeSyscall(const SyscallRecord &syscall_record, FileWriter &file_writer)
{
    ManagedBuffer &managed_buffer = file_writer.getManagedBuffer();
    ScopedSegment scoped_segment(managed_buffer, SegmentTag::CapturedSyscall);

    assert(syscall_record.syscall_number <= 0xffff);
    uint16_t short_syscall_number = static_cast<uint16_t>(syscall_record.syscall_number);
    managed_buffer.writeField(short_syscall_number);

    uint8_t flags = encodeFlags(syscall_record);
    managed_buffer.writeField(flags);

    managed_buffer.writeField<uint8_t>(0x00);

    if (syscall_record.result) {
        managed_buffer.writeField(syscall_record.result);
    }

    if (syscall_record.thread_id) {
        managed_buffer.writeField(*syscall_record.thread_id);
    }

    if (syscall_record.entry_timestamp) {
        managed_buffer.writeField(*syscall_record.entry_timestamp);
    }

    if (syscall_record.syscall_duration) {
        uint32_t multi_unit_time = encodeMultiUnitTime(*syscall_record.syscall_duration);
        managed_buffer.writeField(multi_unit_time);
    }

    if (syscall_record.errnum) {
        managed_buffer.writeField(syscall_record.errnum);
    }

    for (int i = 0; i < getNumberOfArguments(syscall_record.syscall_number); i++) {
        writeSyscallArgument(managed_buffer, syscall_record, i);
    }
}
