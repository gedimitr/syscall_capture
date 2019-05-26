#ifndef SCOPED_PADDER_HPP
#define SCOPED_PADDER_HPP

#include "buffer_view.hpp"

class ScopedPadder
{
public:
    ScopedPadder(BufferView &buffer_view, uint8_t padding_level) :
        m_buffer_view(buffer_view),
        m_padding_level(padding_level) { }

    ~ScopedPadder()
    {
        m_buffer_view.writePadding(m_padding_level);
    }

private:
    BufferView &m_buffer_view;
    uint8_t m_padding_level;
};

#endif // SCOPED_PADDER_HPP
