#include <cstring>
#include <limits>
#include <tuple>

#include "configuration.hpp"
#include "information_elements.hpp"
#include "managed_buffers.hpp"
#include "syscall_args.hpp"
#include "syscall_info.hpp"
#include "syscall_record.hpp"

#include "syscall_args.hpp"

namespace {

template<typename T>
bool canFitIn(int64_t value)
{
    return std::numeric_limits<T>::min() <= value && value <= std::numeric_limits<T>::max();
}

uint32_t calcDataOutputLength(const Configuration &configuration, uint32_t length)
{
    uint32_t max_length = configuration.isMaxCapturedDataLengthDefined() ? configuration.getMaxCapturedDataLength()
                                                                        : maxPayloadData;

    uint32_t output_length = (length <= max_length) ? length : max_length;

    if (output_length > maxPayloadData - sizeof(uint32_t)) {
        output_length = maxPayloadData - sizeof (uint32_t);
    }

    return output_length;
}

}

ArgumentWriter::ArgumentWriter(const Configuration &configuration, ManagedBuffer &managed_buffer) :
    m_configuration(configuration),
    m_managed_buffer(managed_buffer) { }

void ArgumentWriter::writeArgs(const SyscallRecord &syscall_record)
{
    for (int i = 0; i < getNumberOfArguments(syscall_record.syscall_number); i++) {
        writeArg(syscall_record, i);
    }
}

void ArgumentWriter::writeArg(const SyscallRecord &syscall_record, int i)
{
    const SyscallArg *syscall_arg = getSyscallArg(syscall_record.syscall_number, i);
    if (!syscall_arg) {
        return;
    }
    ArgType arg_type = syscall_arg->type;

    int64_t arg = syscall_record.args[i];
    int64_t aux_arg = syscall_record.args[syscall_arg->aux_arg];

    switch(arg_type) {
    case ARG_INT:
        writeArgInt(arg);
        break;
    case ARG_DATA:
        writeArgData(arg, aux_arg);
        break;
    case ARG_STRING:
        writeArgString(arg);
        break;
    default:
        writeArgInt(arg);
    }
}

void ArgumentWriter::writeArgInt(int64_t arg)
{
    ScopedIE ie(m_managed_buffer, IETag::ArgInt);
    if (canFitIn<int32_t>(arg)) {
        int32_t short_arg = static_cast<int32_t>(arg);
        m_managed_buffer.writeField(short_arg);
    } else {
        m_managed_buffer.writeField(arg);
    }
}

void ArgumentWriter::writeArgData(int64_t arg, int64_t length)
{
    uint32_t u32length = static_cast<uint32_t>(length);
    uint32_t output_length = calcDataOutputLength(m_configuration, u32length);

    IETagType tag = (output_length == length) ? IETag::ArgFullString
                                              : IETag::ArgPartialString;

    ScopedIE ie(m_managed_buffer, tag);

    if (tag == IETag::ArgPartialString) {
        m_managed_buffer.writeField<uint32_t>(u32length);
    }

    const char *data = reinterpret_cast<const char *>(arg);
    m_managed_buffer.writeData(data, static_cast<uint32_t>(output_length));
}

void ArgumentWriter::writeArgString(int64_t arg)
{
    const char *str = reinterpret_cast<const char *>(arg);
    int64_t length = static_cast<int64_t>(strlen(str));
    writeArgData(arg, length);
}
