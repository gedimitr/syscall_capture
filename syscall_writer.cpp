#include <cassert>

#include "clocks.hpp"
#include "configuration.hpp"
#include "execution_state.hpp"
#include "file_writer.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "syscall_record.hpp"
#include "thread_id.hpp"

#include "syscall_writer.hpp"

SyscallWriter::SyscallWriter(const Configuration &configuration, const ExecutionState &execution_state,
                             FileWriter &file_writer) :
    m_configuration(configuration),
    m_execution_state(execution_state),
    m_file_writer(file_writer) { }

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
        record.syscall_duration = stopwatch->getNanoseconds();
    }

    int32_t current_thread_id = getCurrentThreadId();
    if (m_execution_state.m_process_id != current_thread_id) {
        record.thread_id = current_thread_id;
    }

    return record;
}
