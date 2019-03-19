#include <sys/utsname.h>
#include <syscall.h>

#include "length_recorder.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "thread_id.hpp"

#include "header_writer.hpp"

namespace {

const uint32_t HdrMagicNumber = 0x780617a5;

bool isBigEndian()
{
    const uint32_t test_value = 0x01020304;

    const char *first_byte = reinterpret_cast<const char *>(&test_value);
    return (*first_byte == 0x01);
}

uint8_t calcHeaderFlags()
{
    uint8_t flags = 0;

    if (isBigEndian()) {
        flags |= 0x01;
    }

    return flags;
}

}

HeaderWriter::HeaderWriter(ManagedBuffer &managed_buffer) :
    m_managed_buffer(managed_buffer) { }

void HeaderWriter::write()
{
    m_managed_buffer.writeFieldBigEndianUnchecked(HdrMagicNumber);
    
    const uint8_t version = 1;
    m_managed_buffer.writeFieldUnchecked(version);

    const uint8_t flags = calcHeaderFlags();
    m_managed_buffer.writeFieldUnchecked(flags);

    // Unused octets
    m_managed_buffer.writeFieldUnchecked<uint16_t>(0);

    uint32_t thread_id = getCurrentThreadId();
    m_managed_buffer.writeFieldUnchecked(thread_id);
}
