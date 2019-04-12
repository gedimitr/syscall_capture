#include <syscall.h>

#include "libsyscall_intercept_hook_point.h"
#include "thread_id.hpp"

namespace {

int32_t queryCurrentThreadId()
{
    int64_t res = syscall_no_intercept(SYS_gettid);
    return static_cast<int32_t>(res);
}

}

int32_t getCurrentThreadId()
{
    static thread_local int32_t thread_id = queryCurrentThreadId();
    return thread_id;
}

int32_t getProcessId()
{
    int64_t res = syscall_no_intercept(SYS_getpid);
    return static_cast<int32_t>(res);
}
