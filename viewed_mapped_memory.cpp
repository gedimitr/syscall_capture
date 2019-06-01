#include "viewed_mapped_memory.hpp"

namespace {

BufferView getView(MappedMemory &mapped_memory)
{
    return BufferView(static_cast<char *>(mapped_memory.data()), mapped_memory.getSize());
}

}

ViewedMappedMemory::ViewedMappedMemory(uint32_t size) :
    m_mapped_memory{size},
    m_buffer_view{getView(m_mapped_memory)} { }

BufferView &ViewedMappedMemory::getBufferView()
{
    return m_buffer_view;
}

void ViewedMappedMemory::resetView()
{
    m_buffer_view = getView(m_mapped_memory);
}
