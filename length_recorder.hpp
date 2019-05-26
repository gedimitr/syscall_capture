#ifndef LENGTHRECORDER_HPP_
#define LENGTHRECORDER_HPP_

#include "buffer_view.hpp"
#include "scoped_cursor_mover.hpp"

template <typename IntType>
class LengthRecorder
{
public:
    LengthRecorder(BufferView &buffer_view);
    ~LengthRecorder();

    LengthRecorder(const LengthRecorder &) = delete;
    LengthRecorder &operator=(const LengthRecorder &) = delete;

private:
    BufferView &m_buffer_view;
    uint32_t m_bookmarked_pos;
};

template <typename IntType>
LengthRecorder<IntType>::LengthRecorder(BufferView &buffer_view) :
    m_buffer_view(buffer_view),
    m_bookmarked_pos(buffer_view.getCurrentPosition())
{
    m_buffer_view.advance(sizeof(IntType));
}

template <typename IntType>
LengthRecorder<IntType>::~LengthRecorder()
{
    uint32_t current_pos = m_buffer_view.getCurrentPosition();
    uint32_t length = current_pos - m_bookmarked_pos;

    ScopedCursorMover cursor_mover(m_buffer_view, m_bookmarked_pos);

    IntType length_field = static_cast<IntType>(length);
    length_field -= sizeof(IntType);

    m_buffer_view.writeField(length_field);
}

#endif
