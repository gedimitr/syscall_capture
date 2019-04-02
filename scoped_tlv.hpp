#ifndef SCOPED_TLV_HPP
#define SCOPED_TLV_HPP

#include "length_recorder.hpp"
#include "managed_buffers.hpp"

/*
 * Scoped output of a TLV
 *
 * When an object of this class is created, the tag is immediately recorded at the managed buffer, and
 * afterwards the cursor is advanced so that the necessary space for the length field is reserved. At
 * destruction, the count of all value bytes added during the object's lifetime is recorded in the (previously
 * reserved) length field. Afterwards, padding bytes are added so that the TLV footprint is a multiple of 4
 * bytes.
 *
 * Implementation notes:
 * - The scoped padder is initialized before the Length Recorder, so that they are destroyed in the reverse
 *   order: first the Length Recorder (writing the count of all bytes addded in the managed buffer until that
 *   time) and afterwards the Scoped Padder (adding any necessary padding bytes). This way the padding bytes
 *   are not counted into the length field.
 * - The comma operator used during the initialization of the Length Recorder data member makes sure that the
 *   tag is first written in the managed buffer and then the managed buffer is offered to the Length Recorder
 *   constructor.
 */
template <typename TagType, typename LengthType>
class ScopedTlv
{
public:
    ScopedTlv(ManagedBuffer &managed_buffer, TagType tag) :
        m_managed_buffer(managed_buffer),
        m_scoped_padder(managed_buffer, 4),
        m_length_recorder((managed_buffer.writeField(tag), managed_buffer)) { }

private:
    ManagedBuffer &m_managed_buffer;
    ScopedPadder m_scoped_padder;
    LengthRecorder<LengthType> m_length_recorder;
};

#endif // SCOPED_TLV_HPP
