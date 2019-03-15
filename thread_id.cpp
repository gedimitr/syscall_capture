#include <syscall.h>

#include "libsyscall_intercept_hook_point.h"
#include "thread_id.hpp"

int32_t getCurrentThreadId()
{
    static thread_local int32_t thread_id = syscall_no_intercept(SYS_gettid);
    return thread_id;
}
