#include "libsyscall_intercept_hook_point.h"

#include <syscall.h>

static int hook(long syscall_number, long arg0, long arg1, long arg2, 
	long arg3, long arg4, long arg5, long *result)
{
	const char *msg = "ping\n";
	syscall_no_intercept(SYS_write, 1, msg, sizeof(msg));
}

static __attribute__((constructor))
void start(void)
{
	intercept_hook_point = &hook;
}
