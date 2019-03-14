#ifndef STOPWATCH_HPP_
#define STOPWATCH_HPP_

#include <cstdint>
#include <time.h>

class Stopwatch
{
public:
    Stopwatch();

    int64_t getNanoseconds() const;

private:
    timespec m_start;
    bool m_valid;
};

#endif