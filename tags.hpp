#ifndef TAGS_HPP_
#define TAGS_HPP_

#include <cstdint>

namespace Tag
{

inline const uint16_t VariableHeaderPart = 0x0001;

// uname strings
inline const uint16_t UnameSysname = 0x0002;
inline const uint16_t UnameNodename = 0x0003;
inline const uint16_t UnameRelease = 0x0004;
inline const uint16_t UnameVersion = 0x0005;
inline const uint16_t UnameMachine = 0x0006;
inline const uint16_t UnameDomainName = 0x0007;

}

#endif