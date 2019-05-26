#include "tlv.hpp"

bool writeTlv(BufferView &buffer_view, uint16_t tag, const char *inp, uint32_t num_bytes)
{
    uint32_t space_reqs = 8 + calcBytesAfterPadding(num_bytes);

    if (!buffer_view.hasRoomFor(space_reqs)) {
        return false;
    }

    if (canFitInShortTlv(num_bytes)) {
        uint16_t num_bytes_short = static_cast<uint16_t>(num_bytes);
        buffer_view.writeField<uint16_t>(tag);
        buffer_view.writeField<uint16_t>(num_bytes_short);
    } else {
        buffer_view.writeField<uint32_t>(tag);
        buffer_view.writeField<uint32_t>(num_bytes);
    }

    buffer_view.writeData(inp, num_bytes);
    buffer_view.writePadding(4);

    return true;
}
