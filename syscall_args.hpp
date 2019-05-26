#ifndef SYSCALL_ARGS_HPP
#define SYSCALL_ARGS_HPP

#include <cstdint>

class Configuration;
class BufferView;
class SyscallRecord;

class ArgumentWriter
{
public:
    ArgumentWriter(const Configuration &configuration, BufferView &buffer_view);

    void writeArgs(const SyscallRecord &syscall_record);

private:
    void writeArg(const SyscallRecord &syscall_record, int i);

    void writeArgInt(int64_t arg);
    void writeArgData(int64_t arg, int64_t length);
    void writeArgString(int64_t arg);
    void writeArgStat(int64_t arg);
    void writeArgPollFds(int64_t arg, int64_t num_fds);
    void writeArgSigAction(int64_t arg, int64_t sigsetsize);
    void writeArgIntP(int64_t arg);

    const Configuration &m_configuration;
    BufferView &m_buffer_view;
};

#endif // SYSCALL_ARGS_HPP
