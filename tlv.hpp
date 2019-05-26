#ifndef TLV_HPP_
#define TLV_HPP_

#include <cstdint>

#include "buffer_view.hpp"
#include "length_recorder.hpp"

constexpr uint32_t calcBytesAfterPadding(uint32_t num_bytes)
{
    return (num_bytes + 3) & 0xfffffff8;
}

constexpr bool canFitInShortTlv(uint32_t num_bytes)
{
    return num_bytes <= 0xffff;
}

template <typename IntType>
bool writeTlv(BufferView &buffer_view, uint16_t tag, IntType value)
{
    uint32_t space_reqs = 4 + calcBytesAfterPadding(sizeof(IntType));

    if (!buffer_view.hasRoomFor(space_reqs)) {
        return false;
    }

    buffer_view.writeField<uint16_t>(tag);
    buffer_view.writeField<uint16_t>(sizeof(IntType));
    buffer_view.writeField(value);
    buffer_view.writePadding(4);

    return true;
}

bool writeTlv(BufferView &buffer_view, uint16_t tag, const char *inp, uint32_t num_bytes);

#endif

