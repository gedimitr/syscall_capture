#ifndef SYSCALL_WRITER_HPP
#define SYSCALL_WRITER_HPP

#include <cstdint>

class Configuration;
class ExecutionState;
class FileWriter;
class SyscallRecord;

class SyscallWriter
{
public:
    SyscallWriter(const Configuration &configuration, const ExecutionState &execution_state);

    SyscallRecord invokeAndRecord(int64_t syscall_number, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3,
                            int64_t arg4, int64_t arg5);

private:
    const Configuration &m_configuration;
    const ExecutionState &m_execution_state;
};

void writeSyscall(const SyscallRecord &syscall_record, FileWriter &file_writer);

#endif // SYSCALL_WRITER_HPP
