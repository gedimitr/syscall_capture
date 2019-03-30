#ifndef SCOPED_TLV_HPP
#define SCOPED_TLV_HPP

#include "length_recorder.hpp"
#include "managed_buffers.hpp"

template <typename TagType, typename LengthType>
class ScopedTlv
{
public:
    ScopedTlv(ManagedBuffer &managed_buffer, TagType tag);

private:
    ManagedBuffer &m_managed_buffer;
    LengthRecorder<LengthType> m_length_recorder;
    ScopedPadder m_scoped_padder;
};

template <typename TagType, typename LengthType>
ScopedTlv<TagType, LengthType>::ScopedTlv(ManagedBuffer &managed_buffer, TagType tag) :
    m_managed_buffer(managed_buffer),
    m_length_recorder((managed_buffer.writeField(tag), managed_buffer)),
    m_scoped_padder(managed_buffer, 4) { }

#endif // SCOPED_TLV_HPP
