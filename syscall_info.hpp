#ifndef SYSCALL_INFO_HPP_
#define SYSCALL_INFO_HPP_

#include <cstdint>

enum ArgType {
    ARG_UNUSED,
    ARG_INT,
    ARG_DATA,
    ARG_STRING,
    ARG_STAT,
    ARG_POLL_FDS,
    ARG_PTR,
    ARG_SIGACTION,
    ARG_INTP
};

enum AuxDataPtr {
    AUX_ARG0,
    AUX_ARG1,
    AUX_ARG2,
    AUX_ARG3,
    AUX_ARG4,
    AUX_ARG5,
    AUX_NONE,
    AUX_RESULT,
};

struct SyscallArg {
    const char *name{nullptr};
    ArgType type{ARG_UNUSED};
    AuxDataPtr aux_data_ptr{AUX_NONE};
};

struct SyscallDescription {
    const char *name;
    struct SyscallArg args[6];
};

extern const SyscallDescription syscall_table[];

int getNumberOfArguments(int64_t syscall_number);

const SyscallArg *getSyscallArg(int64_t syscall_number, int arg_num);

#endif
