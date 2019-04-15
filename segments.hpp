#ifndef SEGMENTS_HPP
#define SEGMENTS_HPP

#include <cstdint>

#include "scoped_tlv.hpp"

typedef uint32_t SegmentTagType;
typedef uint32_t SegmentLengthType;

typedef ScopedTlv<SegmentTagType, SegmentLengthType> ScopedSegment;

namespace SegmentTag
{
constexpr SegmentTagType VariableHeaderPart = 0x0001;
constexpr SegmentTagType CapturedSyscall = 0x0002;
constexpr SegmentTagType Filler = 0x0003;
}

#endif // SEGMENTS_HPP
