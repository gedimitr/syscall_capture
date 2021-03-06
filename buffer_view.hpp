#ifndef BUFFER_VIEW_HPP_
#define BUFFER_VIEW_HPP_

#include <cstdint>

class BufferView
{
public:
    BufferView(char *buffer, uint32_t capacity);

    bool hasRoomFor(uint32_t num_bytes) const;

    template<typename FieldType>
    void writeField(FieldType value);
    void writeData(const char *in, uint32_t num_bytes);
    void writePadding(uint8_t padding_level);
    void writeRecurringByte(uint32_t num_bytes, char value);

    uint32_t getCurrentPosition() const;
    void setCurrentPosition(uint32_t pos);

    void advance(int32_t diff);
    void reset();

    void subtract(const BufferView &buffer_view);

    const char *getRawBuffer() const;

private:
    char *m_buffer;
    uint32_t m_capacity;
    uint32_t m_cur_pos;
};

// Templated Methods Implementation
template<typename FieldType>
void BufferView::writeField(FieldType value)
{
    char *inp = reinterpret_cast<char *>(&value);
    char *outp = m_buffer + m_cur_pos;
    for (uint32_t i = 0; i < sizeof(FieldType); i++) {
        outp[i] = inp[i];
    }
    m_cur_pos += sizeof(FieldType);
}

#endif
