#ifndef TIME_HPP
#define TIME_HPP

#include <cstdint>
#include <ctime>

uint64_t getFastclockTimestamp();
uint64_t getSecondsSinceEpoch();

class Stopwatch
{
public:
    Stopwatch();

    uint64_t getNanoseconds() const;

private:
    timespec m_start;
    bool m_valid;
};

#endif
