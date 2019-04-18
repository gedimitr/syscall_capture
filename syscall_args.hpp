#ifndef SYSCALL_ARGS_HPP
#define SYSCALL_ARGS_HPP

#include <cstdint>

class ManagedBuffer;

void writeSyscallArgumentInt(ManagedBuffer &managed_buffer, int64_t arg);
void writeSyscallArgumentData(ManagedBuffer &managed_buffer, int64_t arg, int64_t size);

#endif // SYSCALL_ARGS_HPP
