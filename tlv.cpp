#include "tlv.hpp"

bool writeTlv(ManagedBuffer &managed_buffer, uint16_t tag, const char *inp, uint32_t num_bytes)
{
    uint32_t space_reqs = 8 + calcBytesAfterPadding(num_bytes);

    if (!managed_buffer.hasRoomFor(space_reqs)) {
        return false;
    }

    if (canFitInShortTlv(num_bytes)) {
        uint16_t num_bytes_short = static_cast<uint16_t>(num_bytes);
        managed_buffer.writeField<uint16_t>(tag);
        managed_buffer.writeField<uint16_t>(num_bytes_short);
    } else {
        managed_buffer.writeField<uint32_t>(tag);
        managed_buffer.writeField<uint32_t>(num_bytes);
    }

    managed_buffer.writeData(inp, num_bytes);
    managed_buffer.writePadding(4);

    return true;
}
