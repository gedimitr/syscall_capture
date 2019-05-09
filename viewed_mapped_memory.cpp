#include "viewed_mapped_memory.hpp"

ViewedMappedMemory::ViewedMappedMemory(uint32_t size) :
    m_mapped_memory(size),
    m_managed_buffer(static_cast<char *>(m_mapped_memory.data()), size) { }

ManagedBuffer &ViewedMappedMemory::getManagedBuffer()
{
    return m_managed_buffer;
}
