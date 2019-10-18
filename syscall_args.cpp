#include <cassert>
#include <cstring>
#include <limits>
#include <tuple>

#include <sys/stat.h>
#include <poll.h>
#include <signal.h>

#include "configuration.hpp"
#include "information_elements.hpp"
#include "buffer_view.hpp"
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

int64_t getAuxData(const SyscallRecord &syscall_record, AuxDataPtr aux_data_ptr)
{
    if (aux_data_ptr == AUX_NONE) {
        return 0;
    }

    if (aux_data_ptr == AUX_RESULT) {
        return syscall_record.result;
    }

    assert(aux_data_ptr >= AUX_ARG0 && aux_data_ptr <= AUX_ARG5);
    return syscall_record.args[aux_data_ptr];
}

}

ArgumentWriter::ArgumentWriter(const Configuration &configuration, BufferView &buffer_view) :
    m_configuration{configuration},
    m_buffer_view{buffer_view} { }

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
    AuxDataPtr aux_data_ptr = syscall_arg->aux_data_ptr;
    int64_t aux_data = getAuxData(syscall_record, aux_data_ptr);

    switch(arg_type) {
    case ARG_INT:
    case ARG_PTR:
        writeArgInt(arg);
        break;
    case ARG_DATA:
        writeArgData(arg, aux_data);
        break;
    case ARG_STRING:
        writeArgString(arg);
        break;
    case ARG_STAT:
        writeArgStat(arg);
        break;
    case ARG_POLL_FDS:
        writeArgPollFds(arg, aux_data);
        break;
    case ARG_SIGACTION:
        writeArgSigAction(arg, aux_data);
        break;
    default:
        writeArgInt(arg);
    }
}

void ArgumentWriter::writeArgInt(int64_t arg)
{
    ScopedIE ie(m_buffer_view, IETag::ArgInt);
    if (canFitIn<int32_t>(arg)) {
        int32_t short_arg = arg;
        m_buffer_view.writeField(short_arg);
    } else {
        m_buffer_view.writeField(arg);
    }
}

void ArgumentWriter::writeArgData(int64_t arg, int64_t length)
{
    // If the length is a negative number, something wrong has happened and no actual data are conveyed, e.g.
    // a read syscall has failed and no actual data were read. Handle this as equivalent to 0.
    uint32_t u32length = length > 0 ? length : 0;

    uint32_t output_length = calcDataOutputLength(m_configuration, u32length);

    IETagType tag = (output_length == length) ? IETag::ArgFullString
                                              : IETag::ArgPartialString;

    ScopedIE ie(m_buffer_view, tag);

    if (tag == IETag::ArgPartialString) {
        m_buffer_view.writeField<uint32_t>(u32length);
    }

    const char *data = reinterpret_cast<const char *>(arg);
    m_buffer_view.writeData(data, static_cast<uint32_t>(output_length));
}

void ArgumentWriter::writeArgString(int64_t arg)
{
    const char *str = reinterpret_cast<const char *>(arg);
    int64_t length = strlen(str);
    writeArgData(arg, length);
}

void ArgumentWriter::writeArgStat(int64_t arg)
{
    ScopedIE ie(m_buffer_view, IETag::ArgStatStruct);

    struct stat *p_stat = reinterpret_cast<struct stat *>(arg);
    m_buffer_view.writeField(p_stat->st_dev);
    m_buffer_view.writeField(p_stat->st_ino);
    m_buffer_view.writeField(p_stat->st_mode);
    m_buffer_view.writeField(p_stat->st_nlink);
    m_buffer_view.writeField(p_stat->st_uid);
    m_buffer_view.writeField(p_stat->st_gid);
    m_buffer_view.writeField(p_stat->st_rdev);
    m_buffer_view.writeField(p_stat->st_size);
    m_buffer_view.writeField(p_stat->st_blksize);
    m_buffer_view.writeField(p_stat->st_blocks);
    m_buffer_view.writeField(p_stat->st_atim);
    m_buffer_view.writeField(p_stat->st_mtim);
    m_buffer_view.writeField(p_stat->st_ctim);
}

void ArgumentWriter::writeArgPollFds(int64_t arg, int64_t num_fds)
{
    ScopedIE ie(m_buffer_view, IETag::ArgPollFds);

    struct pollfd *p_pollfd = reinterpret_cast<struct pollfd *>(arg);
    for (int64_t i = 0; i < num_fds; i++) {
        m_buffer_view.writeField(p_pollfd->fd);
        m_buffer_view.writeField(p_pollfd->events);
        m_buffer_view.writeField(p_pollfd->revents);
        p_pollfd += 1;
    }
}

void ArgumentWriter::writeArgSigAction(int64_t arg, int64_t sigsetsize)
{
    ScopedIE ie(m_buffer_view, IETag::ArgSigAction);

    if (arg) {
        struct sigaction *p_sigaction = reinterpret_cast<struct sigaction *>(arg);

        m_buffer_view.writeField(p_sigaction->sa_handler);
        m_buffer_view.writeField(p_sigaction->sa_sigaction);

        const char *mask_data = reinterpret_cast<const char *>(&p_sigaction->sa_mask);
        uint32_t mask_size = static_cast<uint32_t>(sigsetsize);
        m_buffer_view.writeData(mask_data, mask_size);

        m_buffer_view.writeField(p_sigaction->sa_flags);
        m_buffer_view.writeField(p_sigaction->sa_restorer);
    }
}

void ArgumentWriter::writeArgIntP(int64_t arg)
{
    ScopedIE ie(m_buffer_view, IETag::ArgIntP);

    uint64_t *p_int = reinterpret_cast<uint64_t *>(arg);
    m_buffer_view.writeField(p_int);
    m_buffer_view.writeField(*p_int);
}
