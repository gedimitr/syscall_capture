#include <cstring>
#include <ctime>
#include <sys/utsname.h>
#include <syscall.h>

#include "clocks.hpp"
#include "length_recorder.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "thread_id.hpp"
#include "tags.hpp"
#include "tlv.hpp"

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

void writeString(ManagedBuffer &managed_buffer, uint16_t tag, const char *str)
{
    uint64_t str_length = strlen(str);
    writeTlv(managed_buffer, tag, str, static_cast<uint32_t>(str_length));
}

void writeUnameStrings(ManagedBuffer &managed_buffer)
{
    utsname uts_name;

    int64_t result = syscall_no_intercept(SYS_uname, &uts_name);
    if (result != 0) {
        return;
    }

    writeString(managed_buffer, Tag::UnameSysname, uts_name.sysname);
    writeString(managed_buffer, Tag::UnameNodename, uts_name.nodename);
    writeString(managed_buffer, Tag::UnameRelease, uts_name.release);
    writeString(managed_buffer, Tag::UnameVersion, uts_name.version);
    writeString(managed_buffer, Tag::UnameMachine, uts_name.machine);
    writeString(managed_buffer, Tag::UnameDomainName, uts_name.domainname);
}

void writeVariableHeaderPart(ManagedBuffer &managed_buffer)
{
    managed_buffer.writeFieldBigEndianUnchecked<uint16_t>(Tag::VariableHeaderPart);
    LengthRecorder<uint16_t> length_recorder(managed_buffer);

    writeUnameStrings(managed_buffer);
}

}

void writeFileHeader(ManagedBuffer &managed_buffer)
{
    managed_buffer.writeFieldBigEndianUnchecked(HdrMagicNumber);
    
    const uint8_t version = 1;
    managed_buffer.writeFieldUnchecked(version);

    const uint8_t flags = calcHeaderFlags();
    managed_buffer.writeFieldUnchecked(flags);

    // Unused octets
    managed_buffer.writeFieldUnchecked<uint16_t>(0);

    uint32_t thread_id = getCurrentThreadId();
    managed_buffer.writeFieldUnchecked(thread_id);

    uint64_t sec_since_epoch = getSecondsSinceEpoch();
    managed_buffer.writeFieldUnchecked(sec_since_epoch);

    writeVariableHeaderPart(managed_buffer);
}
