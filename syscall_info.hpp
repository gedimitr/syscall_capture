#ifndef SYSCALL_INFO_HPP_
#define SYSCALL_INFO_HPP_

#include <cstdint>

enum ArgType {
    ARG_UNUSED,
    ARG_INT,
    ARG_DATA,
    ARG_STRING,
    ARG_STAT,
    ARG_POLL_FD,
    ARG_PTR,
    ARG_SIGACTION,
    ARG_INTP
};

struct SyscallArg {
    const char *name;
    enum ArgType type;
};

struct SyscallDescription {
    const char *name;
    struct SyscallArg args[6];
};

extern const SyscallDescription syscall_table[];

int getNumberOfArguments(int64_t syscall_number);

ArgType getSyscallArgType(int64_t syscall_number, int arg_num);

#endif
