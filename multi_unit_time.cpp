#include <cassert>

#include "multi_unit_time.hpp"

namespace {

enum TimeUnit {
    Nanosec,
    Microsec,
    Millisec,
    Sec
};

constexpr uint8_t NUM_ENCODED_VALUE_BITS = 30;

constexpr uint32_t encodeTimeUnit(TimeUnit unit)
{
    return static_cast<uint32_t>(unit) << NUM_ENCODED_VALUE_BITS;
}

constexpr uint32_t calcMaximumValueInBits(uint8_t num_bits)
{
    return (1 << num_bits) - 1;
}

constexpr uint32_t MAX_ENCODED_VALUE = calcMaximumValueInBits(NUM_ENCODED_VALUE_BITS);

constexpr int64_t calcNanosecondsPerUnit(TimeUnit unit)
{
    if (unit == Sec) {
        return 1'000'000'000;
    }

    if (unit == Millisec) {
        return 1'000'000;
    }

    if (unit == Microsec) {
        return 1'000;
    }

    assert(unit == Nanosec);
    return 1;
}

constexpr int64_t calcMaxEncodableNanosecondsForUnit(TimeUnit unit)
{
    return calcNanosecondsPerUnit(unit) * MAX_ENCODED_VALUE;
}

uint32_t convertNanosecondsToUnit(int64_t nsec, TimeUnit unit)
{
    return nsec / calcNanosecondsPerUnit(unit);
}

uint32_t encodeNanosecondsUsingUnit(int64_t nanoseconds, TimeUnit unit)
{
    assert(nanoseconds <= calcMaxEncodableNanosecondsForUnit(unit));

    uint32_t unit_subfield = encodeTimeUnit(unit);
    uint32_t value_subfield = convertNanosecondsToUnit(nanoseconds, unit);

    return unit_subfield | value_subfield;
}

}

uint32_t encodeMultiUnitTime(int64_t nanoseconds)
{
    if (nanoseconds <= calcMaxEncodableNanosecondsForUnit(Nanosec)) {
        return encodeNanosecondsUsingUnit(nanoseconds, Nanosec);
    }
    if (nanoseconds <= calcMaxEncodableNanosecondsForUnit(Microsec)) {
        return encodeNanosecondsUsingUnit(nanoseconds, Microsec);
    }

    if (nanoseconds <= calcMaxEncodableNanosecondsForUnit(Millisec)) {
        return encodeNanosecondsUsingUnit(nanoseconds, Millisec);
    }

    if (nanoseconds <= calcMaxEncodableNanosecondsForUnit(Sec)) {
        return encodeNanosecondsUsingUnit(nanoseconds, Sec);
    }

    return encodeTimeUnit(Sec) | MAX_ENCODED_VALUE;
}

int64_t decodeMultiUnitTime(uint32_t encoded_time)
{
    uint32_t unit_indicator = encoded_time >> NUM_ENCODED_VALUE_BITS;
    TimeUnit unit = static_cast<TimeUnit>(unit_indicator);

    int64_t time_value = encoded_time & MAX_ENCODED_VALUE;
    return time_value * calcNanosecondsPerUnit(unit);
}
