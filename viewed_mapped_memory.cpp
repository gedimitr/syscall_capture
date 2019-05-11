#include "viewed_mapped_memory.hpp"

namespace {

ManagedBuffer getView(MappedMemory &mapped_memory)
{
    return ManagedBuffer(static_cast<char *>(mapped_memory.data()), mapped_memory.getSize());
}

}

ViewedMappedMemory::ViewedMappedMemory(uint32_t size) :
    m_mapped_memory(size),
    m_managed_buffer(getView(m_mapped_memory)) { }

ManagedBuffer &ViewedMappedMemory::getManagedBuffer()
{
    return m_managed_buffer;
}

void ViewedMappedMemory::resetView()
{
    m_managed_buffer = getView(m_mapped_memory);
}
