#ifndef BUFFERS_H_
#define BUFFERS_H_

struct ManagedBuffer
{
    char *buffer;
    long capacity;
    long current_position;
};

void initManagedBuffer(struct ManagedBuffer *managed_buffer, char *buffer, long capacity);

int hasRoomFor(struct ManagedBuffer *managed_buffer, unsigned long num_bytes);

int writeData(struct ManagedBuffer *managed_buffer, char *buf, unsigned long length);

int writePadding(struct ManagedBuffer *managed_buffer, int padding_level);

#endif
