#include <syscall.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

#include "configuration.hpp"
#include "libsyscall_intercept_hook_point.h"

#include "file_writer.hpp"

namespace {

char *mapMemory(uint32_t size)
{
    int64_t res = syscall_no_intercept(SYS_mmap, 0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    return reinterpret_cast<char *>(res);
}

int openFileForWriting(const char *path)
{
    int64_t res = syscall_no_intercept(SYS_open, path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    return static_cast<int>(res);
}

}

FileWriter::FileWriter(const Configuration &configuration) :
    m_configuration(configuration),
    m_output_file_fd(-1),
    m_memory(mapMemory(PAGE_SIZE)),
    m_managed_buffer(m_memory, PAGE_SIZE) { }

FileWriter::~FileWriter()
{
    if (m_output_file_fd == -1) {
        return;
    }

    uint32_t num_bytes = m_managed_buffer.getCurrentPosition();
    const char *data = m_managed_buffer.getRawBuffer();

    syscall_no_intercept(SYS_write, m_output_file_fd, data, num_bytes);
    syscall_no_intercept(SYS_close, m_output_file_fd);
}

bool FileWriter::openOutputFile()
{
    if (m_output_file_fd == -1) {
        m_output_file_fd = openFileForWriting(m_configuration.getOutputFilePath());
    }

    return (m_output_file_fd != -1);
}

ManagedBuffer &FileWriter::getManagedBuffer()
{
    return m_managed_buffer;
}
