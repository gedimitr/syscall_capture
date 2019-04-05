#ifndef MULTI_UNIT_TIME_HPP
#define MULTI_UNIT_TIME_HPP

#include <cstdint>

/*
 * Multi Unit Time
 *
 * Multi Unit Time fields encode time using different time units, in order to increase the range of time
 * values that can be stored, but with (increasingly) reduced precision. Multi Unit Time fields use 32 bit
 * words, out of which:
 * - The most significant 2 bits indicate the time unit
 * - The rest (30) of the bits hold the time value, in the previously indicated units
 *
 * The supported time units are:
 * - 00: Nanoseconds
 * - 01: Microseconds
 * - 10: Milliseconds
 * - 11: Seconds
 *
 * Out of range time values are encoded as all ones (0xffff ffff). This means that the maximum time value that
 * can be encoded is 1073741822 seconds (~34 years).
 */
uint32_t encodeMultiUnitTime(int64_t nanoseconds);
int64_t decodeMultiUnitTime(uint32_t encoded_time);

#endif // MULTI_UNIT_TIME_HPP
