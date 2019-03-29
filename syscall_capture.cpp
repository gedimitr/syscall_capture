#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <syscall.h>

#include "clocks.hpp"
#include "header_writer.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "length_recorder.hpp"
#include "managed_buffers.hpp"
#include "thread_id.hpp"
#include "tlv.hpp"

static int hook(long syscall_number, long arg0, long arg1, long arg2, 
	long arg3, long arg4, long arg5, long *result)
{
	static char buffer[128];

	int32_t thread_id = getCurrentThreadId();

	Stopwatch stopwatch;
    *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4, arg5);
    uint64_t nsecs = stopwatch.getNanoseconds();

    sprintf(buffer, "System Call Number: %ld --- Arg0(%lx) Arg1(%lx) Result(%lx) Duration(%ld) ThreadId(%d)\n", syscall_number, arg0, arg1, *result, nsecs, thread_id);
	syscall_no_intercept(SYS_write, 1, buffer, strlen(buffer));

	return 0;
}

static __attribute__((constructor))
void start(void)
{
    //const char *capture_path = getenv("SYSCALL_CAPTURE_PATH");

    char buffer[1024];
    ManagedBuffer manbuf(buffer, 1024);
    
    writeFileHeader(manbuf);

    writeTlv<uint32_t>(manbuf, 0x0345, 45);
    writeTlv<uint32_t>(manbuf, 0x1890, 99);

    manbuf.writeData("abcd", 4);

    {
        LengthRecorder<uint32_t> length_recorder(manbuf);
        manbuf.writeData("efgh", 4);
    }

    syscall_no_intercept(SYS_write, 1, buffer, manbuf.getCurrentPosition());

	intercept_hook_point = &hook;
}
