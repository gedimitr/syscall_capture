#ifndef SCOPED_CURSOR_MOVER_HPP
#define SCOPED_CURSOR_MOVER_HPP

#include <cstdint>

#include "buffer_view.hpp"

class ScopedCursorMover
{
public:
    ScopedCursorMover(BufferView &buffer_view) :
        m_buffer_view(buffer_view),
        m_original_pos(buffer_view.getCurrentPosition()) { }

    ScopedCursorMover(BufferView &buffer_view, uint32_t new_pos) :
        ScopedCursorMover(buffer_view)
    {
        m_buffer_view.setCurrentPosition(new_pos);
    }

    ~ScopedCursorMover()
    {
        m_buffer_view.setCurrentPosition(m_original_pos);
    }

    ScopedCursorMover(const ScopedCursorMover &) = delete;
    ScopedCursorMover &operator=(const ScopedCursorMover &) = delete;

private:
    BufferView &m_buffer_view;
    uint32_t m_original_pos;
};

#endif // SCOPED_CURSOR_MOVER_HPP
