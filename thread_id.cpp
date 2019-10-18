#include <syscall.h>

#include "libsyscall_intercept_hook_point.h"
#include "thread_id.hpp"

namespace {

int32_t queryCurrentThreadId()
{
    return syscall_no_intercept(SYS_gettid);
}

}

int32_t getCurrentThreadId()
{
    static thread_local int32_t thread_id = queryCurrentThreadId();
    return thread_id;
}

int32_t getProcessId()
{
    return syscall_no_intercept(SYS_getpid);
}
