#include <tuple>

#include "information_elements.hpp"
#include "managed_buffers.hpp"

#include "syscall_args.hpp"

void writeSyscallArgumentInt(ManagedBuffer &managed_buffer, int64_t arg)
{
    ScopedIE ie(managed_buffer, IETag::ArgInt);
    managed_buffer.writeField(arg);
}

void writeSyscallArgumentData(ManagedBuffer &managed_buffer, int64_t arg, int32_t size)
{
    const char *data = reinterpret_cast<const char *>(arg);
    auto [tag, ie_length] = size > 0xffff ? std::make_tuple(IETag::ArgPartialString, 0xffff)
                                          : std::make_tuple(IETag::ArgFullString, size);
    ScopedIE ie(managed_buffer, tag);
    managed_buffer.writeData(data, static_cast<uint32_t>(ie_length));
}
