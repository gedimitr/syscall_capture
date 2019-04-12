#ifndef SYSCALL_RECORD_HPP
#define SYSCALL_RECORD_HPP

#include <cstdint>
#include <optional>

struct SyscallRecord
{
    int64_t syscall_number{0};
    int64_t args[6] = {0, 0, 0, 0, 0, 0};
    int64_t result{0};
    int errnum{0};

    std::optional<int64_t> entry_timestamp;
    std::optional<int64_t> syscall_duration;
    std::optional<int32_t> thread_id;
};

#endif // SYSCALL_RECORD_HPP
