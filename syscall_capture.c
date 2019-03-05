#include "libsyscall_intercept_hook_point.h"

#include <syscall.h>
#include <stdio.h>
#include <string.h>

static int hook(long syscall_number, long arg0, long arg1, long arg2, 
	long arg3, long arg4, long arg5, long *result)
{
	static char buffer[128];

	*result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4, arg5);

	const char *msg = "System Call Number: %ld --- Arg0(%lx) Arg1(%lx) Result(%lx)\n";
	sprintf(buffer, msg, syscall_number, arg0, arg1, *result);
	syscall_no_intercept(SYS_write, 1, buffer, strlen(buffer));

	return 0;
}

static __attribute__((constructor))
void start(void)
{
	intercept_hook_point = &hook;
}
