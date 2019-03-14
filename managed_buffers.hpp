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

    bool setCursor(uint32_t pos);

private:
    char *getCurrentWritePointer();

    char *m_buffer;
    uint32_t m_capacity;
    uint32_t m_cur_pos;
};

#endif
