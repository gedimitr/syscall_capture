#include "tlv.hpp"

namespace {

ManagedBuffer &writeTag(ManagedBuffer &managed_buffer, uint16_t tag)
{
    managed_buffer.writeField(tag);
    return managed_buffer;
}

}

bool writeTlv(ManagedBuffer &managed_buffer, uint16_t tag, const char *inp, uint32_t num_bytes)
{
    uint32_t space_reqs = 8 + calcBytesAfterPadding(num_bytes);

    if (!managed_buffer.hasRoomFor(space_reqs)) {
        return false;
    }

    if (canFitInShortTlv(num_bytes)) {
        uint16_t num_bytes_short = static_cast<uint16_t>(num_bytes);
        managed_buffer.writeFieldBigEndianUnchecked<uint16_t>(tag);
        managed_buffer.writeFieldBigEndianUnchecked<uint16_t>(num_bytes_short);
    } else {
        managed_buffer.writeFieldBigEndianUnchecked<uint32_t>(tag);
        managed_buffer.writeFieldBigEndianUnchecked<uint32_t>(num_bytes);
    }

    managed_buffer.writeDataUnchecked(inp, num_bytes);
    managed_buffer.writePaddingUnchecked(4);

    return true;
}
ScopedTlv::ScopedTlv(ManagedBuffer &managed_buffer, uint16_t tag) :
    m_managed_buffer(managed_buffer),
    m_length_recorder(writeTag(managed_buffer, tag)) { }
