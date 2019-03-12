#include <string.h>

#include "managed_buffers.h"

static inline char *getCurrentWritePointer(struct ManagedBuffer *managed_buffer)
{
    return managed_buffer->buffer + managed_buffer->current_position;
}

void initManagedBuffer(struct ManagedBuffer *managed_buffer, char *buffer, long capacity)
{
    managed_buffer->buffer = buffer;
    managed_buffer->capacity = capacity;
    managed_buffer->current_position = 0;
}

int hasRoomFor(struct ManagedBuffer *managed_buffer, unsigned long num_bytes)
{
    long avail_space = managed_buffer->capacity - managed_buffer->current_position;
    return (avail_space >= (long)num_bytes);
}

int writeData(struct ManagedBuffer *managed_buffer, char *buf, unsigned long length)
{
    int res = 0;

    if (hasRoomFor(managed_buffer, length)) {
        char *outp = getCurrentWritePointer(managed_buffer);
        memcpy(outp, buf, length);
        managed_buffer->current_position += length;
        res = 1;
    }

    return res;
}

static int calcNumPaddingBytes(long current_position, int padding_level)
{
    int modulo = current_position % padding_level;

    if (modulo) {
        return padding_level - modulo;
    } else {
        return 0;
    }
}

int writePadding(struct ManagedBuffer *managed_buffer, int padding_level)
{
    int res = 0;

    int num_padding_bytes = calcNumPaddingBytes(managed_buffer->current_position, padding_level);
    if (hasRoomFor(managed_buffer, (unsigned long)num_padding_bytes)) {
        for (int i = 0; i < num_padding_bytes; i++) {
            managed_buffer->buffer[managed_buffer->current_position++] = 0;
        }
        res = 1;
    }

    return res;
}
