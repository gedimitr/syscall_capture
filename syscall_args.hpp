#ifndef SYSCALL_ARGS_HPP
#define SYSCALL_ARGS_HPP

#include <cstdint>

class Configuration;
class ManagedBuffer;
class SyscallRecord;

class ArgumentWriter
{
public:
    ArgumentWriter(const Configuration &configuration, ManagedBuffer &managed_buffer);

    void writeArgs(const SyscallRecord &syscall_record);

private:
    void writeArg(const SyscallRecord &syscall_record, int i);

    void writeArgInt(int64_t arg);
    void writeArgData(int64_t arg, int64_t length);

    const Configuration &m_configuration;
    ManagedBuffer &m_managed_buffer;
};

#endif // SYSCALL_ARGS_HPP
