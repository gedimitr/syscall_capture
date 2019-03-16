#include <cstring>

#include "managed_buffers.hpp"

namespace {

static uint8_t calcNumPaddingBytes(uint32_t current_position, uint8_t padding_level)
{
    uint8_t modulo = current_position % padding_level;

    if (modulo) {
        return padding_level - modulo;
    } else {
        return 0;
    }
}

}

ManagedBuffer::ManagedBuffer(char *buffer, uint32_t capacity) :
    m_buffer(buffer),
    m_capacity(capacity),
    m_cur_pos(0) { }

bool ManagedBuffer::hasRoomFor(uint32_t num_bytes) const
{
    auto avail_space = m_capacity - m_cur_pos;
    return (avail_space >= num_bytes);
}

bool ManagedBuffer::writeData(const char *in, uint32_t num_bytes)
{
    if (hasRoomFor(num_bytes)) {
        char *outp = getCurrentWritePointer();
        memcpy(outp, in, num_bytes);
        m_cur_pos += num_bytes;
        return true;
    } else {
        return false;
    }
}

bool ManagedBuffer::writePadding(uint8_t padding_level)
{
    uint8_t num_padding_bytes = calcNumPaddingBytes(m_cur_pos, padding_level);
    if (hasRoomFor(num_padding_bytes)) {
        for (int i = 0; i < num_padding_bytes; i++) {
            m_buffer[m_cur_pos++] = 0;
        }
        return true;
    } else {
        return false;
    }
}

uint32_t ManagedBuffer::getCurrentPosition() const
{
    return m_cur_pos;
}

bool ManagedBuffer::setCurrentPosition(uint32_t pos)
{
    if (pos < m_capacity) {
        m_cur_pos = pos;
        return true;
    } else {
        return false;
    }
}

bool ManagedBuffer::advance(int32_t diff)
{
    int64_t new_pos = static_cast<int64_t>(m_cur_pos) + diff;
    if (new_pos >= 0 && new_pos < m_capacity) {
        m_cur_pos = static_cast<uint32_t>(new_pos);
        return true;
    } else {
        return false;
    }
}

char *ManagedBuffer::getCurrentWritePointer()
{
    return m_buffer + m_cur_pos;
}
