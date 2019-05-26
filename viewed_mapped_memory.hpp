#ifndef VIEWED_MAPPED_MEMORY_HPP
#define VIEWED_MAPPED_MEMORY_HPP

#include <cstdint>

#include "memory.hpp"
#include "buffer_view.hpp"

class ViewedMappedMemory
{
public:
    ViewedMappedMemory(uint32_t size);

    BufferView &getBufferView();

    void resetView();

private:
    MappedMemory m_mapped_memory;
    BufferView m_buffer_view;
};

#endif // VIEWED_MAPPED_MEMORY_HPP
