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

    uint32_t getSize() const;

    void *data();
    const void *data() const;

    bool allocate(uint32_t size);

    void free();

    bool isEmpty() const;

    MappedMemory(const MappedMemory &) = delete;
    MappedMemory &operator=(const MappedMemory &) = delete;

private:
    void *m_addr{nullptr};
    uint32_t m_length{0};
};

#endif // MEMORY_HPP
