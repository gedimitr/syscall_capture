#include <cassert>

#include "syscall_info.hpp"

const SyscallDescription syscall_table[] = {
    {"read", {{"fd", ARG_INT}, 
              {"buf", ARG_DATA, 2},
              {"count", ARG_INT}}},
    {"write", {{"fd", ARG_INT},
               {"buf", ARG_DATA, 2},
               {"count", ARG_INT}}},
    {"open", {{"pathname", ARG_STRING},
              {"flags", ARG_INT},
              {"mode", ARG_INT}}},
    {"close", {{"fd", ARG_INT}}},
    {"stat", {{"pathname", ARG_STRING},
              {"statbuf", ARG_STAT}}},
    {"fstat", {{"fd", ARG_INT},
               {"statbuf", ARG_STAT}}},
    {"lstat", {{"pathname", ARG_STRING},
               {"statbuf", ARG_STAT}}},
    {"poll", {{"fds", ARG_POLL_FDS, 1},
              {"nfds", ARG_INT},
              {"timeout", ARG_INT}}},
    {"lseek", {{"fd", ARG_INT},
               {"offset", ARG_INT},
               {"whence", ARG_INT}}},
    {"mmap", {{"addr", ARG_PTR},
              {"length", ARG_INT},
              {"prot", ARG_INT},
              {"flags", ARG_INT},
              {"fd", ARG_INT},
              {"offset", ARG_INT}}},
    {"mprotect", {{"addr", ARG_PTR},
                  {"len", ARG_INT},
                  {"prot", ARG_INT}}},
    {"munmap", {{"addr", ARG_PTR},
                {"length", ARG_INT}}},
    {"brk", {{"addr", ARG_PTR}}},
    {"rt_sigaction", {{"sig", ARG_INT},
                     {"act", ARG_SIGACTION, 3},
                     {"oact", ARG_SIGACTION, 3},
                     {"sigsetsize", ARG_INT}}},
    {"rt_sigprocmask", {{"how", ARG_INT},
                        {"set", ARG_INTP},
                        {"oset", ARG_INTP},
                        {"sigsetsize", ARG_INT}}}
};

namespace {

constexpr int getNumberOfSyscallsDefined()
{
    return sizeof(syscall_table) / sizeof(SyscallDescription);
}

constexpr bool isSyscallDescribed(int64_t syscall_number)
{
    return syscall_number <= getNumberOfSyscallsDefined();
}

constexpr const SyscallDescription *getSyscallDescription(int64_t syscall_number)
{
    if (isSyscallDescribed(syscall_number)) {
        return &syscall_table[syscall_number];
    } else {
        return nullptr;
    }
}

constexpr int countSyscallArguments(int64_t syscall_number)
{
    const SyscallDescription *syscall_descr = getSyscallDescription(syscall_number);

    if (syscall_descr == nullptr) {
        return 0;
    }

    for (int i = 0; i < 6; i++) {
        if (syscall_descr->args[i].type == ARG_UNUSED) {
            return i;
        }
    }

    return 6;
}

}

int getNumberOfArguments(int64_t syscall_number)
{
    if (syscall_number > getNumberOfSyscallsDefined()) {
        return 0;
    }

    return countSyscallArguments(syscall_number);
}

const SyscallArg *getSyscallArg(int64_t syscall_number, int arg_num)
{
    const SyscallDescription *syscall_descr = getSyscallDescription(syscall_number);
    if (syscall_descr) {
        return &syscall_descr->args[arg_num];
    } else {
        return nullptr;
    }
}
