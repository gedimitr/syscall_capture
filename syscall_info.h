#ifndef SYSCALL_INFO
#define SYSCALL_INFO

enum ArgType {
    ARG_UNUSED,
    ARG_INT,
    ARG_DATA,
    ARG_STRING,
    ARG_STAT,
    ARG_POLL_FD,
    ARG_PTR
};

struct SyscallArg {
    const char *name;
    enum ArgType type;
};

struct SyscallDescription {
    const char *name;
    struct SyscallArg args[6];
};

extern struct SyscallDescription syscall_table[];

#endif