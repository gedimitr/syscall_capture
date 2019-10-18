#include <cstring>
#include <ctime>
#include <sys/utsname.h>
#include <syscall.h>

#include "clocks.hpp"
#include "information_elements.hpp"
#include "length_recorder.hpp"
#include "libsyscall_intercept_hook_point.h"
#include "segments.hpp"
#include "thread_id.hpp"
#include "tlv.hpp"

#include "header_writer.hpp"

namespace {

const char HdrMagicString[4] = {'\x78', '\x06', '\x17', '\xa5'};

bool isBigEndian()
{
    const uint32_t test_value = 0x01020304;

    const uint8_t *first_byte = reinterpret_cast<const uint8_t *>(&test_value);
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

void writeZeroTerminatedString(BufferView &buffer_view, const char *str)
{
    uint32_t str_length = strlen(str);
    buffer_view.writeData(str, str_length);
    buffer_view.writeField('\0');
}

void writeUnameStrings(BufferView &buffer_view)
{
    utsname uts_name;

    int64_t result = syscall_no_intercept(SYS_uname, &uts_name);
    if (result != 0) {
        return;
    }

    ScopedIE scoped_ie(buffer_view, IETag::HostUname);

    writeZeroTerminatedString(buffer_view, uts_name.sysname);
    writeZeroTerminatedString(buffer_view, uts_name.nodename);
    writeZeroTerminatedString(buffer_view, uts_name.release);
    writeZeroTerminatedString(buffer_view, uts_name.version);
    writeZeroTerminatedString(buffer_view, uts_name.machine);
    writeZeroTerminatedString(buffer_view, uts_name.domainname);
}

void writeVariableHeaderPart(BufferView &buffer_view)
{
    ScopedSegment variable_header_part(buffer_view, SegmentTag::VariableHeaderPart);
    writeUnameStrings(buffer_view);
}

}

void writeFileHeader(BufferView &buffer_view)
{
    buffer_view.writeData(HdrMagicString, sizeof(HdrMagicString));
    
    const uint8_t version = 1;
    buffer_view.writeField(version);

    const uint8_t flags = calcHeaderFlags();
    buffer_view.writeField(flags);

    // Unused octets
    buffer_view.writeField<uint16_t>(0);

    int32_t thread_id = getCurrentThreadId();
    buffer_view.writeField(thread_id);

    int64_t sec_since_epoch = getSecondsSinceEpoch();
    buffer_view.writeField(sec_since_epoch);

    int64_t fast_clock_ref = getFastclockTimestamp();
    buffer_view.writeField(fast_clock_ref);

    writeVariableHeaderPart(buffer_view);
}
