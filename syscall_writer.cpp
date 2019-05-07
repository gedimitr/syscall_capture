#include <cassert>

#include "configuration.hpp"
#include "execution_state.hpp"
#include "file_write_permit.hpp"
#include "file_writer.hpp"
#include "multi_unit_time.hpp"
#include "segments.hpp"
#include "syscall_args.hpp"
#include "syscall_record.hpp"

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

}

SyscallWriter::SyscallWriter(const Configuration &configuration, FileWriter &file_writer) :
    m_configuration(configuration),
    m_file_writer(file_writer) { }

void SyscallWriter::write(const SyscallRecord &syscall_record)
{
    FileWritePermit permit(m_file_writer);

    ManagedBuffer &managed_buffer = permit.getManagedBuffer();
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

    ArgumentWriter arg_writer(m_configuration, managed_buffer);
    arg_writer.writeArgs(syscall_record);
}
