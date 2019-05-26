#ifndef INFORMATION_ELEMENTS_HPP
#define INFORMATION_ELEMENTS_HPP

#include <cstdint>
#include <limits>

#include "scoped_tlv.hpp"

typedef uint16_t IETagType;
typedef uint16_t IELengthType;

typedef ScopedTlv<IETagType, IELengthType> ScopedIE;

namespace IETag
{
// Variable Header Part
constexpr IETagType HostUname = 0x0001;

// Captured Syscall
constexpr IETagType ArgInt = 0x0001;
constexpr IETagType ArgFullString = 0x0002;
constexpr IETagType ArgPartialString = 0x0003;
constexpr IETagType ArgStatStruct = 0x0004;
constexpr IETagType ArgPollFds = 0x0005;
constexpr IETagType ArgSigAction = 0x0006;
constexpr IETagType ArgIntP = 0x0007;

}

constexpr int64_t maxPayloadData = std::numeric_limits<IELengthType>::max();

#endif // INFORMATION_ELEMENTS_HPP
