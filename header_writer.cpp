#include <cstring>
#include <ctime>
#include <sys/utsname.h>
#include <syscall.h>

#include "clocks.hpp"
#include "information_elements.hpp"
#include "length_recorder.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "thread_id.hpp"
#include "tags.hpp"
#include "tlv.hpp"

#include "header_writer.hpp"

namespace {

const char HdrMagicString[4] = {'\x78', '\x06', '\x17', '\xa5'};

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

void writeZeroTerminatedString(ManagedBuffer &managed_buffer, const char *str)
{
    uint32_t str_length = static_cast<uint32_t>(strlen(str));
    managed_buffer.writeData(str, str_length);
    managed_buffer.writeField('\0');
}

void writeUnameStrings(ManagedBuffer &managed_buffer)
{
    utsname uts_name;

    int64_t result = syscall_no_intercept(SYS_uname, &uts_name);
    if (result != 0) {
        return;
    }

    ScopedIE scoped_ie(managed_buffer, IETag::HostUname);

    writeZeroTerminatedString(managed_buffer, uts_name.sysname);
    writeZeroTerminatedString(managed_buffer, uts_name.nodename);
    writeZeroTerminatedString(managed_buffer, uts_name.release);
    writeZeroTerminatedString(managed_buffer, uts_name.version);
    writeZeroTerminatedString(managed_buffer, uts_name.machine);
    writeZeroTerminatedString(managed_buffer, uts_name.domainname);
}

void writeVariableHeaderPart(ManagedBuffer &managed_buffer)
{
    managed_buffer.writeField<uint16_t>(Tag::VariableHeaderPart);
    LengthRecorder<uint16_t> length_recorder(managed_buffer);

    writeUnameStrings(managed_buffer);
}

}

void writeFileHeader(ManagedBuffer &managed_buffer)
{
    managed_buffer.writeData(HdrMagicString, sizeof(HdrMagicString));
    
    const uint8_t version = 1;
    managed_buffer.writeField(version);

    const uint8_t flags = calcHeaderFlags();
    managed_buffer.writeField(flags);

    // Unused octets
    managed_buffer.writeField<uint16_t>(0);

    int32_t thread_id = getCurrentThreadId();
    managed_buffer.writeField(thread_id);

    int64_t sec_since_epoch = getSecondsSinceEpoch();
    managed_buffer.writeField(sec_since_epoch);

    writeVariableHeaderPart(managed_buffer);
}
