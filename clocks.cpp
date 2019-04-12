#include <cassert>
#include "clocks.hpp"

namespace {

inline const int64_t INVALID_TIME64 = -1;
inline const int64_t NSECS_PER_SEC = 1'000'000'000L;

bool queryClock(clockid_t clock_type, timespec *ts)
{
    int res = clock_gettime(clock_type, ts);
    return (res == 0);
}

bool queryFastClock(timespec *ts)
{
    return queryClock(CLOCK_MONOTONIC, ts);
}

int64_t calcNanosecondsDifference(const timespec &end, const timespec &start)
{
    int64_t tv_sec_diff = end.tv_sec - start.tv_sec;
    int64_t tv_nsec_diff = end.tv_nsec - start.tv_nsec;

    return tv_sec_diff * NSECS_PER_SEC + tv_nsec_diff;
}

int64_t aggregateIntoNanoseconds(const timespec &ts)
{
    return ts.tv_sec * NSECS_PER_SEC + ts.tv_nsec;
}

} // anonymous namespace

int64_t getSecondsSinceEpoch()
{
    if (timespec ts; queryClock(CLOCK_REALTIME, &ts)) {
        return ts.tv_sec;
    } else {
        return INVALID_TIME64;
    }
}

int64_t getFastclockTimestamp()
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

int64_t Stopwatch::getStartTimestamp() const
{
    if (m_valid) {
        return m_start.tv_sec * 1'000'000'000L + m_start.tv_nsec;
    } else {
        return INVALID_TIME64;
    }
}

int64_t Stopwatch::getNanoseconds() const
{
    if (!m_valid) {
        return INVALID_TIME64;
    }

    if (timespec now; queryFastClock(&now)) {
        int64_t nsec_diff = calcNanosecondsDifference(now, m_start);
        assert(nsec_diff >= 0);
        return nsec_diff;
    } else {
        return INVALID_TIME64;
    }
}
