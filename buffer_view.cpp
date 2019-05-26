#include <cassert>
#include <cstring>

#include "buffer_view.hpp"

namespace {

constexpr uint8_t PADDING_BYTE = 0x00;

uint8_t calcNumPaddingBytes(uint32_t current_position, uint8_t padding_level)
{
    uint8_t modulo = current_position % padding_level;
    return modulo ? padding_level - modulo
                  : 0;
}

}

BufferView::BufferView(char *buffer, uint32_t capacity) :
    m_buffer(buffer),
    m_capacity(capacity),
    m_cur_pos(0) { }

bool BufferView::hasRoomFor(uint32_t num_bytes) const
{
    uint32_t avail_space = m_capacity - m_cur_pos;
    return (avail_space >= num_bytes);
}

void BufferView::writeData(const char *in, uint32_t num_bytes)
{
    assert(hasRoomFor(num_bytes));
    char *outp = m_buffer + m_cur_pos;
    memcpy(outp, in, num_bytes);
    m_cur_pos += num_bytes;
}

void BufferView::writePadding(uint8_t padding_level)
{
    uint8_t num_padding_bytes = calcNumPaddingBytes(m_cur_pos, padding_level);
    writeRecurringByte(num_padding_bytes, PADDING_BYTE);
}

void BufferView::writeRecurringByte(uint32_t num_bytes, char value)
{
    assert(hasRoomFor(num_bytes));
    for (uint32_t i = 0; i < num_bytes; i++) {
        m_buffer[m_cur_pos++] = value;
    }
}

uint32_t BufferView::getCurrentPosition() const
{
    return m_cur_pos;
}

void BufferView::setCurrentPosition(uint32_t pos)
{
    assert(pos <= m_capacity);
    m_cur_pos = pos;
}

void BufferView::advance(int32_t diff)
{
    int64_t new_pos = static_cast<int64_t>(m_cur_pos) + diff;
    assert(new_pos >= 0 && new_pos <= m_capacity);
    m_cur_pos = static_cast<uint32_t>(new_pos);
}

void BufferView::reset()
{
    m_cur_pos = 0;
}

void BufferView::subtract(const BufferView &buffer_view)
{
    assert(m_buffer == buffer_view.getRawBuffer());

    uint32_t num_bytes = buffer_view.getCurrentPosition();
    m_buffer += num_bytes;
    m_capacity -= num_bytes;
}

const char *BufferView::getRawBuffer() const
{
    return m_buffer;
}
