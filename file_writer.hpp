#ifndef FILE_WRITER_HPP
#define FILE_WRITER_HPP

#include "managed_buffers.hpp"

class Configuration;

class FileWriter
{
public:
    FileWriter(const Configuration &configuration);
    ~FileWriter();

    bool openOutputFile();

    ManagedBuffer &getManagedBuffer();

private:
    static constexpr uint32_t PAGE_SIZE = 1024 * 1024;

    const Configuration &m_configuration;
    int m_output_file_fd;
    char *m_memory;
    ManagedBuffer m_managed_buffer;
};

#endif // FILE_WRITER_HPP
