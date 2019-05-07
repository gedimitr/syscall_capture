#ifndef FILE_WRITER_HPP
#define FILE_WRITER_HPP

#include <array>

#include "managed_buffers.hpp"
#include "memory.hpp"

class Configuration;

class FileWriter
{
public:
    FileWriter(const Configuration &configuration);
    ~FileWriter();

    bool openOutputFile();

    ManagedBuffer &getManagedBuffer();

    void lock();
    void unlock();

private:
    void flush();

    const Configuration &m_configuration;
    int m_output_file_fd;

    char *m_memory;

    ManagedBuffer m_managed_buffer;
};

#endif // FILE_WRITER_HPP
