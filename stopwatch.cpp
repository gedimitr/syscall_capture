#include <syscall.h>

#include "libsyscall_intercept_hook_point.h"
#include "stopwatch.hpp"

namespace {


bool querySystemClock(timespec *ts)
{
    int res = syscall_no_intercept(SYS_clock_gettime, CLOCK_MONOTONIC_RAW, ts);
    return (res == 0); 
}

int64_t getNanosecondsDifference(const timespec &end, const timespec &start)
{
    int64_t sec_diff = end.tv_sec - start.tv_sec;
    int64_t nsec_diff = end.tv_nsec - start.tv_nsec;

    return sec_diff * 1'000'000'000 + nsec_diff;
}

}

Stopwatch::Stopwatch()
{
    m_valid = querySystemClock(&m_start);
}

int64_t Stopwatch::getNanoseconds() const
{
    if (!m_valid) {
        return -1;
    }

    timespec now;
    if (!querySystemClock(&now)) {
        return -1;
    }

    return getNanosecondsDifference(now, m_start);
}
