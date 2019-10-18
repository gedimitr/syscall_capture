#include <array>
#include <cassert>
#include <syscall.h>
#include <sys/types.h>
#include <fcntl.h>

#include "configuration.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "memory.hpp"
#include "segments.hpp"

#include "file_writer.hpp"

namespace {

constexpr uint32_t minimumSegmentSize()
{
    return sizeof(SegmentTagType) + sizeof(SegmentLengthType);
}

static constexpr uint32_t PAGE_SIZE = 1024 * 1024;
static constexpr uint32_t FLIP_THRESHOLD = 128 * 1024;
static constexpr uint32_t FLUSH_THRESHOLD = PAGE_SIZE - minimumSegmentSize();

static constexpr int INVALID_FD = -1;

int openFileForWriting(const char *path)
{
    return syscall_no_intercept(SYS_open, path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
}

void flushBuffer(BufferView &buffer_view, int fd)
{
    if (fd != INVALID_FD) {
        uint32_t num_bytes = buffer_view.getCurrentPosition();
        const char *data = buffer_view.getRawBuffer();

        syscall_no_intercept(SYS_write, fd, data, num_bytes);
    }
}

void writeFillerSegment(BufferView &view, uint32_t payload_length)
{
    ScopedSegment seg(view, SegmentTag::Filler);
    view.writeRecurringByte(payload_length, 0);
}

}

FileWriter::FileWriter(const Configuration &configuration) :
    m_configuration{configuration},
    m_buffers{PAGE_SIZE, PAGE_SIZE},
    m_working_buffer{&m_buffers.front()},
    m_standby_buffer{&m_buffers.back()},
    m_flush_pending_working_data{m_working_buffer->getBufferView()},
    m_output_file_fd{INVALID_FD} { }

FileWriter::~FileWriter()
{
    flush(false);
    syscall_no_intercept(SYS_close, m_output_file_fd);
}

bool FileWriter::openOutputFile()
{
    if (m_output_file_fd == INVALID_FD) {
        m_output_file_fd = openFileForWriting(m_configuration.getOutputFilePath());
    }

    return (m_output_file_fd != INVALID_FD);
}

BufferView &FileWriter::getBufferView()
{
    return m_working_buffer->getBufferView();
}

void FileWriter::lock()
{
    // TODO: Implement this when multithreaded capturing is to be supported
}

void FileWriter::unlock()
{
    if (!getBufferView().hasRoomFor(FLIP_THRESHOLD)) {
        assert(m_standby_buffer->getBufferView().getCurrentPosition() == 0);
        flipWorkingBuffer();
        m_flush_pending_working_data = m_working_buffer->getBufferView();
    }

    if (getTotalDataSize() >= FLUSH_THRESHOLD) {
        flush(true);
    }

    m_flush_pending_working_data = m_working_buffer->getBufferView();

    // TODO: Implement this when multithreaded capturing is to be supported
}

void FileWriter::flush(bool add_filler)
{
    uint32_t flush_pending_data_size = getFlushPendingDataSize();
    assert(flush_pending_data_size < PAGE_SIZE);

    flushBuffer(m_standby_buffer->getBufferView(), m_output_file_fd);
    m_standby_buffer->resetView();

    flushBuffer(m_flush_pending_working_data, m_output_file_fd);
    m_working_buffer->getBufferView().subtract(m_flush_pending_working_data);

    if (add_filler) {
        uint32_t filler_payload_length = PAGE_SIZE - flush_pending_data_size - minimumSegmentSize();
        outputFillerSegment(filler_payload_length);
    }
}

void FileWriter::flipWorkingBuffer()
{
    m_standby_buffer = m_working_buffer;
    if (m_working_buffer == &m_buffers[0]) {
        m_working_buffer = &m_buffers[1];
    } else {
        m_working_buffer = &m_buffers[0];
    }
}

uint32_t FileWriter::getFlushPendingDataSize()
{
    uint32_t data_size = m_standby_buffer->getBufferView().getCurrentPosition();
    data_size += m_flush_pending_working_data.getCurrentPosition();

    return data_size;
}

uint32_t FileWriter::getTotalDataSize() const
{
    return m_standby_buffer->getBufferView().getCurrentPosition() +
           m_working_buffer->getBufferView().getCurrentPosition();
}

void FileWriter::outputFillerSegment(uint32_t payload_length)
{
    BufferView view = m_standby_buffer->getBufferView();
    writeFillerSegment(view, payload_length);
    flushBuffer(view, m_output_file_fd);
    m_standby_buffer->resetView();
}
