#include <syscall.h>
#include <sys/types.h>
#include <fcntl.h>

#include "configuration.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "memory.hpp"

#include "file_writer.hpp"

namespace {

static constexpr uint32_t PAGE_SIZE = 1024 * 1024;
static constexpr uint32_t FLUSH_THRESHOLD = 128 * 1024;

static constexpr int INVALID_FD = -1;

int openFileForWriting(const char *path)
{
    int64_t res = syscall_no_intercept(SYS_open, path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    return static_cast<int>(res);
}

}

FileWriter::FileWriter(const Configuration &configuration) :
    m_configuration{configuration},
    m_buffers{PAGE_SIZE, PAGE_SIZE},
    m_working_buffer{&m_buffers.front()},
    m_output_file_fd{INVALID_FD} { }

FileWriter::~FileWriter()
{
    flush();
    syscall_no_intercept(SYS_close, m_output_file_fd);
}

bool FileWriter::openOutputFile()
{
    if (m_output_file_fd == INVALID_FD) {
        m_output_file_fd = openFileForWriting(m_configuration.getOutputFilePath());
    }

    return (m_output_file_fd != INVALID_FD);
}

ManagedBuffer &FileWriter::getManagedBuffer()
{
    return m_buffers.front().getManagedBuffer();
}

void FileWriter::lock()
{
    // TODO: Implement this when multithreaded capturing is to be supported
}

void FileWriter::unlock()
{
    if (!getManagedBuffer().hasRoomFor(FLUSH_THRESHOLD)) {
        flush();
    }

    // TODO: Implement this when multithreaded capturing is to be supported
}

void FileWriter::flush()
{
    ManagedBuffer &managed_buffer = getManagedBuffer();
    if (m_output_file_fd != INVALID_FD) {
        uint32_t num_bytes = managed_buffer.getCurrentPosition();
        const char *data = managed_buffer.getRawBuffer();

        syscall_no_intercept(SYS_write, m_output_file_fd, data, num_bytes);
    }

    managed_buffer.reset();
}
