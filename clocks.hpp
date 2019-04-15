#ifndef TIME_HPP
#define TIME_HPP

#include <cstdint>
#include <ctime>

int64_t getFastclockTimestamp();
int64_t getSecondsSinceEpoch();

class Stopwatch
{
public:
    Stopwatch();

    int64_t getStartTimestamp() const;
    int64_t getElapsedNanoseconds() const;

private:
    timespec m_start;
    bool m_valid;
};

#endif
