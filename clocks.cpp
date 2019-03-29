#include <cassert>
#include "clocks.hpp"

namespace {

const inline uint64_t INVALID_TIME64 = static_cast<uint64_t>(-1);

bool queryClock(clockid_t clock_type, timespec *ts)
{
    int res = clock_gettime(clock_type, ts);
    return (res == 0);
}

bool queryFastClock(timespec *ts)
{
    return queryClock(CLOCK_MONOTONIC, ts);
}

uint64_t calcNanosecondsDifference(const timespec &end, const timespec &start)
{
    int64_t tv_sec_diff = end.tv_sec - start.tv_sec;
    int64_t tv_nsec_diff = end.tv_nsec - start.tv_nsec;

    int64_t diff_nsec = tv_sec_diff * 1'000'000'000 + tv_nsec_diff;
    assert(diff_nsec >= 0);

    return static_cast<uint64_t>(diff_nsec);
}

uint64_t aggregateIntoNanoseconds(const timespec &ts)
{
    int64_t sem_nsecs = ts.tv_sec * 1'000'000'000 + ts.tv_nsec;
    return static_cast<uint64_t>(sem_nsecs);
}

}

uint64_t getSecondsSinceEpoch()
{
    if (timespec ts; clock_gettime(CLOCK_REALTIME, &ts)) {
        return static_cast<uint64_t>(ts.tv_sec);
    } else {
        return INVALID_TIME64;
    }
}

uint64_t getFastclockTimestamp()
{
    if (timespec ts; queryFastClock(&ts)) {
        return aggregateIntoNanoseconds(ts);
    } else {
        return INVALID_TIME64;
    }
}

Stopwatch::Stopwatch()
{
    m_valid = queryFastClock(&m_start);
}

uint64_t Stopwatch::getNanoseconds() const
{
    if (!m_valid) {
        return INVALID_TIME64;
    }

    if (timespec now; queryFastClock(&now)) {
        return calcNanosecondsDifference(now, m_start);
    } else {
        return INVALID_TIME64;
    }
}
