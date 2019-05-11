#ifndef FILE_WRITER_HPP
#define FILE_WRITER_HPP

#include <array>

#include "viewed_mapped_memory.hpp"

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
    void flush(bool add_filler);

    void flipWorkingBuffer();
    uint32_t getFlushPendingDataSize();
    uint32_t getTotalDataSize() const;

    void outputFillerSegment(uint32_t payload_length);

    const Configuration &m_configuration;

    std::array<ViewedMappedMemory, 2> m_buffers;
    ViewedMappedMemory *m_working_buffer;
    ViewedMappedMemory *m_standby_buffer;
    ManagedBuffer m_flush_pending_working_data;

    int m_output_file_fd;
};

#endif // FILE_WRITER_HPP
