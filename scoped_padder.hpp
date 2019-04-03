#ifndef SCOPED_PADDER_HPP
#define SCOPED_PADDER_HPP

#include "managed_buffers.hpp"

class ScopedPadder
{
public:
    ScopedPadder(ManagedBuffer &managed_buffer, uint8_t padding_level) :
        m_managed_buffer(managed_buffer),
        m_padding_level(padding_level) { }

    ~ScopedPadder()
    {
        m_managed_buffer.writePadding(m_padding_level);
    }

private:
    ManagedBuffer &m_managed_buffer;
    uint8_t m_padding_level;
};

#endif // SCOPED_PADDER_HPP
