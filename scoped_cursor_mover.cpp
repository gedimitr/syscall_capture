#include "managed_buffers.hpp"
#include "scoped_cursor_mover.hpp"

ScopedCursorMover::ScopedCursorMover(ManagedBuffer &managed_buffer) :
    m_managed_buffer(managed_buffer),
    m_original_pos(managed_buffer.getCurrentPosition()) { }

ScopedCursorMover::ScopedCursorMover(ManagedBuffer &managed_buffer, uint32_t new_pos) :
    ScopedCursorMover(managed_buffer)
{
    m_managed_buffer.setCurrentPosition(new_pos);
}

ScopedCursorMover::~ScopedCursorMover()
{
    m_managed_buffer.setCurrentPosition(m_original_pos);
}
