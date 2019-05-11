#ifndef VIEWED_MAPPED_MEMORY_HPP
#define VIEWED_MAPPED_MEMORY_HPP

#include <cstdint>

#include "memory.hpp"
#include "managed_buffers.hpp"

class ViewedMappedMemory
{
public:
    ViewedMappedMemory(uint32_t size);

    ManagedBuffer &getManagedBuffer();

    void resetView();

private:
    MappedMemory m_mapped_memory;
    ManagedBuffer m_managed_buffer;
};

#endif // VIEWED_MAPPED_MEMORY_HPP
