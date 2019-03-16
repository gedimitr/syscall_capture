#ifndef LENGTHRECORDER_HPP_
#define LENGTHRECORDER_HPP_

#include "managed_buffers.hpp"
#include "scoped_cursor_mover.hpp"

template <typename IntType>
class LengthRecorder
{
public:
    LengthRecorder(ManagedBuffer &managed_buffer);
    ~LengthRecorder();

    LengthRecorder(const LengthRecorder &) = delete;
    LengthRecorder &operator=(const LengthRecorder &) = delete;

private:
    ManagedBuffer &m_managed_buffer;
    uint32_t m_bookmarked_pos;
};

template <typename IntType>
LengthRecorder<IntType>::LengthRecorder(ManagedBuffer &managed_buffer) :
    m_managed_buffer(managed_buffer),
    m_bookmarked_pos(managed_buffer.getCurrentPosition())
{
    m_managed_buffer.advance(sizeof(IntType));
}

template <typename IntType>
LengthRecorder<IntType>::~LengthRecorder()
{
    uint32_t current_pos = m_managed_buffer.getCurrentPosition();
    uint32_t length = current_pos - m_bookmarked_pos;

    ScopedCursorMover cursor_mover(m_managed_buffer, m_bookmarked_pos);

    IntType length_field = static_cast<IntType>(length);
    length_field -= sizeof(IntType);

    const char *outp = reinterpret_cast<const char *>(&length_field);
    m_managed_buffer.writeData(outp, sizeof(IntType));
}

#endif
