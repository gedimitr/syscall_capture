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

    bool writeData(const char *in, uint32_t num_bytes);
    bool writePadding(uint8_t padding_level);

    uint32_t getCurrentPosition() const;
    bool setCurrentPosition(uint32_t pos);
    bool advance(int32_t diff);

private:
    char *m_buffer;
    uint32_t m_capacity;
    uint32_t m_cur_pos;
};


template<typename FieldType>
bool ManagedBuffer::writeField(FieldType value)
{
    if (hasRoomFor(sizeof(FieldType))) {
        char *inp = reinterpret_cast<char *>(&value);
        char *outp = m_buffer + m_cur_pos;
        for (uint32_t i = 0; i < sizeof(FieldType); i++) {
            outp[i] = inp[i];
        }
        m_cur_pos += sizeof(FieldType);
        return true;
    } else {
        return false;
    }
}

#endif
