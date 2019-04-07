#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <syscall.h>

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

namespace {

alignas(alignof(Configuration))
char configuration[sizeof(Configuration)];

alignas(alignof(FileWriter))
char file_writer[sizeof(FileWriter)];

Configuration *getConfiguration()
{
    return reinterpret_cast<Configuration *>(configuration);
}

FileWriter *getFileWriter()
{
    return reinterpret_cast<FileWriter *>(file_writer);
}

}

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
    Configuration *configuration = new (getConfiguration()) Configuration;
    FileWriter *file_writer = new(getFileWriter()) FileWriter(*configuration);
    file_writer->openOutputFile();

    ManagedBuffer &manbuf = file_writer->getManagedBuffer();

    writeFileHeader(manbuf);

	intercept_hook_point = &hook;
}

static __attribute__((destructor))
void finish(void)
{
    getFileWriter()->~FileWriter();
    getConfiguration()->~Configuration();
}
