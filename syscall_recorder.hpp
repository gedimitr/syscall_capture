#ifndef SYSCALL_RECORDER_HPP
#define SYSCALL_RECORDER_HPP

#include <cstdint>

class Configuration;
class ExecutionState;
class SyscallRecord;

class SyscallRecorder
{
public:
    SyscallRecorder(const Configuration &configuration, const ExecutionState &execution_state);

    SyscallRecord invokeAndRecord(int64_t syscall_number, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3,
                            int64_t arg4, int64_t arg5);

private:
    const Configuration &m_configuration;
    const ExecutionState &m_execution_state;
};

#endif // SYSCALL_RECORDER_HPP
