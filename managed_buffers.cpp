#include <cassert>
#include <cstring>

#include "managed_buffers.hpp"

namespace {

constexpr uint8_t PADDING_BYTE = 0x00;

uint8_t calcNumPaddingBytes(uint32_t current_position, uint8_t padding_level)
{
    uint8_t modulo = current_position % padding_level;
    return modulo ? padding_level - modulo
                  : 0;
}

}

ManagedBuffer::ManagedBuffer(char *buffer, uint32_t capacity) :
    m_buffer(buffer),
    m_capacity(capacity),
    m_cur_pos(0) { }

bool ManagedBuffer::hasRoomFor(uint32_t num_bytes) const
{
    uint32_t avail_space = m_capacity - m_cur_pos;
    return (avail_space >= num_bytes);
}

void ManagedBuffer::writeData(const char *in, uint32_t num_bytes)
{
    assert(hasRoomFor(num_bytes));
    char *outp = m_buffer + m_cur_pos;
    memcpy(outp, in, num_bytes);
    m_cur_pos += num_bytes;
}

void ManagedBuffer::writePadding(uint8_t padding_level)
{
    uint8_t num_padding_bytes = calcNumPaddingBytes(m_cur_pos, padding_level);
    writeRecurringByte(num_padding_bytes, PADDING_BYTE);
}

void ManagedBuffer::writeRecurringByte(uint32_t num_bytes, char value)
{
    assert(hasRoomFor(num_bytes));
    for (uint32_t i = 0; i < num_bytes; i++) {
        m_buffer[m_cur_pos++] = value;
    }
}

uint32_t ManagedBuffer::getCurrentPosition() const
{
    return m_cur_pos;
}

void ManagedBuffer::setCurrentPosition(uint32_t pos)
{
    assert(pos <= m_capacity);
    m_cur_pos = pos;
}

void ManagedBuffer::advance(int32_t diff)
{
    int64_t new_pos = static_cast<int64_t>(m_cur_pos) + diff;
    assert(new_pos >= 0 && new_pos <= m_capacity);
    m_cur_pos = static_cast<uint32_t>(new_pos);
}

void ManagedBuffer::reset()
{
    m_cur_pos = 0;
}

void ManagedBuffer::subtract(const ManagedBuffer &managed_buffer)
{
    assert(m_buffer == managed_buffer.getRawBuffer());

    uint32_t num_bytes = managed_buffer.getCurrentPosition();
    m_buffer += num_bytes;
    m_capacity -= num_bytes;
}

const char *ManagedBuffer::getRawBuffer() const
{
    return m_buffer;
}
