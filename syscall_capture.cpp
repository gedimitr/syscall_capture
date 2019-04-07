#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <syscall.h>
#include <unistd.h>

#include "clocks.hpp"
#include "configuration.hpp"
#include "file_writer.hpp"
#include "header_writer.hpp"
#include "information_elements.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "length_recorder.hpp"
#include "managed_buffers.hpp"
#include "scoped_tlv.hpp"
#include "thread_id.hpp"
#include "tlv.hpp"

static int hook(long syscall_number, long arg0, long arg1, long arg2, 
	long arg3, long arg4, long arg5, long *result)
{
	static char buffer[128];

	int32_t thread_id = getCurrentThreadId();

	Stopwatch stopwatch;
    *result = syscall_no_intercept(syscall_number, arg0, arg1, arg2, arg3, arg4, arg5);
    int64_t nsecs = stopwatch.getNanoseconds();

    sprintf(buffer, "System Call Number: %ld --- Arg0(%lx) Arg1(%lx) Result(%lx) Duration(%ld) ThreadId(%d)\n", syscall_number, arg0, arg1, *result, nsecs, thread_id);
	syscall_no_intercept(SYS_write, 1, buffer, strlen(buffer));

	return 0;
}

static __attribute__((constructor))
void start(void)
{
    Configuration configuration;
    FileWriter file_writer(configuration);
    file_writer.openOutputFile();

    ManagedBuffer &manbuf = file_writer.getManagedBuffer();

    {
        ScopedIE ie(manbuf, 12);
        manbuf.writeField((uint16_t)0x9999);
    }

    writeFileHeader(manbuf);

    writeTlv<uint32_t>(manbuf, 0x0345, 45);
    writeTlv<uint32_t>(manbuf, 0x1890, 99);

    manbuf.writeData("abcd", 4);

    {
        ScopedTlv<uint16_t, uint16_t> tlv(manbuf, 56);
        manbuf.writeData("efgh", 4);
    }

	intercept_hook_point = &hook;
}
