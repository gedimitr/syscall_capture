#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <syscall.h>

#include "clocks.hpp"
#include "configuration.hpp"
#include "execution_state.hpp"
#include "file_writer.hpp"
#include "header_writer.hpp"
#include "information_elements.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "length_recorder.hpp"
#include "managed_buffers.hpp"
#include "scoped_tlv.hpp"
#include "syscall_record.hpp"
#include "syscall_writer.hpp"
#include "thread_id.hpp"
#include "tlv.hpp"

namespace {

#define AlignedStaticPlaceHolder(T, name) alignas(alignof(T)) static char name[sizeof(T)]

Configuration *getConfiguration()
{
    AlignedStaticPlaceHolder(Configuration, configuration);
    return reinterpret_cast<Configuration *>(configuration);
}

FileWriter *getFileWriter()
{
    AlignedStaticPlaceHolder(FileWriter, file_writer);
    return reinterpret_cast<FileWriter *>(file_writer);
}

ExecutionState *getExecutionState()
{
    AlignedStaticPlaceHolder(ExecutionState, execution_state);
    return reinterpret_cast<ExecutionState *>(execution_state);
}

void outputSyscallToStdout(const SyscallRecord &record)
{
    static char buffer[128];

    sprintf(buffer, "System Call Number: %ld --- Arg0(%lx) Arg1(%lx) Result(%lx)\n",
            record.syscall_number,
            record.args[0],
            record.args[1],
            record.result);
    syscall_no_intercept(SYS_write, 1, buffer, strlen(buffer));
}

}

static int hook(long syscall_number, long arg0, long arg1, long arg2, 
	long arg3, long arg4, long arg5, long *result)
{
    SyscallWriter syscall_writer(*getConfiguration(), *getExecutionState());
    SyscallRecord record = syscall_writer.invokeAndRecord(syscall_number, arg0, arg1, arg2, arg3, arg4, arg5);

    outputSyscallToStdout(record);

    *result = record.result;
    errno = record.errnum;
	return 0;
}

static __attribute__((constructor))
void start(void)
{
    Configuration *configuration = new (getConfiguration()) Configuration;
    FileWriter *file_writer = new(getFileWriter()) FileWriter(*configuration);
    new(getExecutionState()) ExecutionState;

    file_writer->openOutputFile();

    ManagedBuffer &manbuf = file_writer->getManagedBuffer();

    writeFileHeader(manbuf);

	intercept_hook_point = &hook;
}

static __attribute__((destructor))
void finish(void)
{
    getExecutionState()->~ExecutionState();
    getFileWriter()->~FileWriter();
    getConfiguration()->~Configuration();
}
