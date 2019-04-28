#ifndef SYSCALL_WRITER_HPP
#define SYSCALL_WRITER_HPP

#include <cstdint>

class Configuration;
class FileWriter;
class SyscallRecord;

class SyscallWriter
{
public:
    SyscallWriter(const Configuration &configuration, FileWriter &file_writer);

    void write(const SyscallRecord &syscall_record);

private:
    const Configuration &m_configuration;
    FileWriter &m_file_writer;
};

#endif // SYSCALL_WRITER_HPP
