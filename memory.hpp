#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

void *mapMemory(unsigned long length);
bool unmapMemory(void *addr, unsigned long length);

class MappedMemory
{
public:
    MappedMemory() {};
    MappedMemory(uint32_t size);
    ~MappedMemory();

    void *data();
    const void *data() const;

    bool allocate(unsigned long size);

    void free();

    bool isEmpty() const;

    MappedMemory(const MappedMemory &) = delete;
    MappedMemory &operator=(const MappedMemory &) = delete;

private:
    void *m_addr{nullptr};
    unsigned long m_length{0};
};

#endif // MEMORY_HPP
