#ifndef MANAGED_BUFFERS_HPP_
#define MANAGED_BUFFERS_HPP_

#include <cstdint>

class ManagedBuffer
{
public:
    ManagedBuffer(char *buffer, uint32_t capacity);

    bool hasRoomFor(uint32_t num_bytes) const;

    bool writeData(const char *in, uint32_t num_bytes);
    bool writePadding(uint8_t padding_level);

    uint32_t getCurrentPosition() const;
    bool setCurrentPosition(uint32_t pos);
    bool advance(int32_t diff);

private:
    char *getCurrentWritePointer();

    char *m_buffer;
    uint32_t m_capacity;
    uint32_t m_cur_pos;
};

#endif
