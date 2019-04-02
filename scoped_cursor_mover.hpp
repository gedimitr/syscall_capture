#ifndef SCOPED_CURSOR_MOVER_HPP
#define SCOPED_CURSOR_MOVER_HPP

#include <cstdint>

#include "managed_buffers.hpp"

class ScopedCursorMover
{
public:
    ScopedCursorMover(ManagedBuffer &managed_buffer) :
        m_managed_buffer(managed_buffer),
        m_original_pos(managed_buffer.getCurrentPosition()) { }

    ScopedCursorMover(ManagedBuffer &managed_buffer, uint32_t new_pos) :
        ScopedCursorMover(managed_buffer)
    {
        m_managed_buffer.setCurrentPosition(new_pos);
    }

    ~ScopedCursorMover()
    {
        m_managed_buffer.setCurrentPosition(m_original_pos);
    }

    ScopedCursorMover(const ScopedCursorMover &) = delete;
    ScopedCursorMover &operator=(const ScopedCursorMover &) = delete;

private:
    ManagedBuffer &m_managed_buffer;
    uint32_t m_original_pos;
};

#endif // SCOPED_CURSOR_MOVER_HPP
