#include <cassert>

#include "configuration.hpp"
#include "execution_state.hpp"
#include "file_write_permit.hpp"
#include "file_writer.hpp"
#include "segments.hpp"
#include "syscall_args.hpp"
#include "syscall_record.hpp"

#include "syscall_writer.hpp"

namespace {

enum SyscallRecordFlag
{
    FLAG_RESULT = 0x01,
    FLAG_THREAD_ID = 0x02,
    FLAG_TIMESTAMP = 0x04,
    FLAG_DURATION = 0x08
};

uint8_t encodeFlags(const SyscallRecord &syscall_record)
{
    uint8_t flags = 0x00;

    if (syscall_record.result != 0) {
        flags |= FLAG_RESULT;
    }

    if (syscall_record.thread_id) {
        flags |= FLAG_THREAD_ID;
    }

    if (syscall_record.entry_timestamp) {
        flags |= FLAG_TIMESTAMP;
    }

    if (syscall_record.syscall_duration) {
        flags |= FLAG_DURATION;
    }

    return flags;
}

}

SyscallWriter::SyscallWriter(const Configuration &configuration, FileWriter &file_writer) :
    m_configuration{configuration},
    m_file_writer{file_writer} { }

void SyscallWriter::write(const SyscallRecord &syscall_record)
{
    FileWritePermit permit(m_file_writer);

    BufferView &buffer_view = permit.getBufferView();
    ScopedSegment scoped_segment(buffer_view, SegmentTag::CapturedSyscall);

    assert(syscall_record.syscall_number <= 0xffff);
    uint16_t short_syscall_number = static_cast<uint16_t>(syscall_record.syscall_number);
    buffer_view.writeField(short_syscall_number);

    uint8_t flags = encodeFlags(syscall_record);
    buffer_view.writeField(flags);

    buffer_view.writeField<uint8_t>(0x00);

    if (syscall_record.result) {
        buffer_view.writeField(syscall_record.result);
    }

    if (syscall_record.thread_id) {
        buffer_view.writeField(syscall_record.thread_id.value());
    }

    if (syscall_record.entry_timestamp) {
        buffer_view.writeField(syscall_record.entry_timestamp.value());
    }

    if (syscall_record.syscall_duration) {
        float float_duration = static_cast<float>(syscall_record.syscall_duration.value());
        buffer_view.writeField(float_duration);
    }

    ArgumentWriter arg_writer(m_configuration, buffer_view);
    arg_writer.writeArgs(syscall_record);
}
