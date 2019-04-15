#ifndef INFORMATION_ELEMENTS_HPP
#define INFORMATION_ELEMENTS_HPP

#include <cstdint>

#include "scoped_tlv.hpp"

typedef uint16_t IETagType;
typedef uint16_t IELengthType;

typedef ScopedTlv<IETagType, IELengthType> ScopedIE;

namespace IETag
{
// Variable Header Part
constexpr IETagType HostUname = 0x0001;

// Captured Syscall
constexpr IETagType ArgUInt = 0x0001;
constexpr IETagType ArgFullString = 0x0002;
constexpr IETagType ArgPartialString = 0x0003;
}

#endif // INFORMATION_ELEMENTS_HPP
