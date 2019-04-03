#ifndef MANAGED_BUFFERS_HPP_
#define MANAGED_BUFFERS_HPP_

#include <cstdint>

class ManagedBuffer
{
public:
    ManagedBuffer(char *buffer, uint32_t capacity);

    bool hasRoomFor(uint32_t num_bytes) const;

    template<typename FieldType>
    void writeField(FieldType value);
    void writeData(const char *in, uint32_t num_bytes);
    void writePadding(uint8_t padding_level);

    uint32_t getCurrentPosition() const;
    void setCurrentPosition(uint32_t pos);

    void advance(int32_t diff);

    ManagedBuffer(const ManagedBuffer &) = delete;
    ManagedBuffer &operator=(const ManagedBuffer &) = delete;

private:
    void writeBytes(uint8_t num_bytes, char value);

    char *m_buffer;
    uint32_t m_capacity;
    uint32_t m_cur_pos;
};

// Templated Methods Implementation
template<typename FieldType>
void ManagedBuffer::writeField(FieldType value)
{
    char *inp = reinterpret_cast<char *>(&value);
    char *outp = m_buffer + m_cur_pos;
    for (uint32_t i = 0; i < sizeof(FieldType); i++) {
        outp[i] = inp[i];
    }
    m_cur_pos += sizeof(FieldType);
}

class ScopedPadder
{
public:
    ScopedPadder(ManagedBuffer &managed_buffer, uint8_t padding_level) :
        m_managed_buffer(managed_buffer),
        m_padding_level(padding_level) { }

    ~ScopedPadder()
    {
        m_managed_buffer.writePadding(m_padding_level);
    }

private:
    ManagedBuffer &m_managed_buffer;
    uint8_t m_padding_level;
};

#endif
