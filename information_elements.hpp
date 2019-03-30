#ifndef INFORMATION_ELEMENTS_HPP
#define INFORMATION_ELEMENTS_HPP

#include <cstdint>

#include "scoped_tlv.hpp"

typedef uint16_t IETagType;
typedef uint16_t IELengthType;

typedef ScopedTlv<IETagType, IELengthType> ScopedIE;

struct IETag
{
// Variable Header Part
constexpr static IETagType HostUname = 0x0001;

// System Call Arguments
constexpr static IETagType UInt = 0x0001;
constexpr static IETagType FullString = 0x0002;
constexpr static IETagType PartialString = 0x0003;

};

#endif // INFORMATION_ELEMENTS_HPP
