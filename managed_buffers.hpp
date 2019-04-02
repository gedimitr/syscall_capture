#ifndef MANAGED_BUFFERS_HPP_
#define MANAGED_BUFFERS_HPP_

#include <cstdint>

class ManagedBuffer
{
public:
    ManagedBuffer(char *buffer, uint32_t capacity);

    bool hasRoomFor(uint32_t num_bytes) const;

    template<typename FieldType>
    bool writeField(FieldType value);

    template<typename FieldType>
    void writeFieldUnchecked(FieldType value);

    template<typename FieldType>
    bool writeFieldBigEndian(FieldType value);

    template<typename FieldType>
    void writeFieldBigEndianUnchecked(FieldType value);

    bool writeData(const char *in, uint32_t num_bytes);
    void writeDataUnchecked(const char *in, uint32_t num_bytes);

    bool writePadding(uint8_t padding_level);
    void writePaddingUnchecked(uint8_t padding_level);

    uint32_t getCurrentPosition() const;
    bool setCurrentPosition(uint32_t pos);
    bool advance(int32_t diff);

    ManagedBuffer(const ManagedBuffer &) = delete;
    ManagedBuffer &operator=(const ManagedBuffer &) = delete;

private:
    void writeBytesUnchecked(uint8_t num_bytes, char value);

    char *m_buffer;
    uint32_t m_capacity;
    uint32_t m_cur_pos;
};

class ScopedPadder
{
public:
    ScopedPadder(ManagedBuffer &managed_buffer, uint8_t padding_level) :
        m_managed_buffer(managed_buffer),
        m_padding_level(padding_level) { }

    ~ScopedPadder()
    {
        m_managed_buffer.writePaddingUnchecked(m_padding_level);
    }

private:
    ManagedBuffer &m_managed_buffer;
    uint8_t m_padding_level;
};

// Templated Methods Implementation
template<typename FieldType>
bool ManagedBuffer::writeField(FieldType value)
{
    if (hasRoomFor(sizeof(FieldType))) {
        writeFieldUnchecked(value);
        return true;
    } else {
        return false;
    }
}

template<typename FieldType>
void ManagedBuffer::writeFieldUnchecked(FieldType value)
{
    char *inp = reinterpret_cast<char *>(&value);
    char *outp = m_buffer + m_cur_pos;
    for (uint32_t i = 0; i < sizeof(FieldType); i++) {
        outp[i] = inp[i];
    }
    m_cur_pos += sizeof(FieldType);
}

template<typename FieldType>
bool ManagedBuffer::writeFieldBigEndian(FieldType value)
{
    if (hasRoomFor(sizeof(FieldType))) {
        writeFieldBigEndianUnchecked(value);
        return true;
    } else {
        return false;
    }
}

template<typename FieldType>
void ManagedBuffer::writeFieldBigEndianUnchecked(FieldType value)
{
    char *outp = m_buffer + m_cur_pos;
    for (int32_t i = sizeof(FieldType) - 1; i >= 0; i--) {
        char byte = value & 0xff;
        outp[i] = byte;
        value >>= 8;
    }
    m_cur_pos += sizeof(FieldType);
}

#endif
