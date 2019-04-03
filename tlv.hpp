#ifndef TLV_HPP_
#define TLV_HPP_

#include <cstdint>

#include "managed_buffers.hpp"
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
bool writeTlv(ManagedBuffer &managed_buffer, uint16_t tag, IntType value)
{
    uint32_t space_reqs = 4 + calcBytesAfterPadding(sizeof(IntType));

    if (!managed_buffer.hasRoomFor(space_reqs)) {
        return false;
    }

    managed_buffer.writeField<uint16_t>(tag);
    managed_buffer.writeField<uint16_t>(sizeof(IntType));
    managed_buffer.writeField(value);
    managed_buffer.writePadding(4);

    return true;
}

bool writeTlv(ManagedBuffer &managed_buffer, uint16_t tag, const char *inp, uint32_t num_bytes);

#endif

