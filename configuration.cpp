#include <cstdlib>
#include <cstring>

#include "configuration.hpp"

namespace {

const char *OUTPUT_PATH_ENVVAR = "SYSCALL_CAPTURE_OUTPUT_PATH";
const char *RECORD_SYSCALL_ENTRY_TIMESTAMP_ENVVAR = "SYSCALL_CAPTURE_RECORD_ENTRY_TS";
const char *RECORD_SYSCALL_DURATION_ENVVAR = "SYSCALL_CAPTURE_RECORD_DURATION";

bool isEnvVarDefined(const char *envvar) {
    return getenv(envvar);
}

}

Configuration::Configuration() :
    m_output_file_path{'\0'},
    m_record_syscall_entry_timestamp_option(isEnvVarDefined(RECORD_SYSCALL_ENTRY_TIMESTAMP_ENVVAR)),
    m_record_syscall_duration_option(isEnvVarDefined(RECORD_SYSCALL_DURATION_ENVVAR))
{
    const char *output_file = getenv(OUTPUT_PATH_ENVVAR);
    if (output_file && strlen(output_file) < MAX_PATH_LENGTH) {
        strcpy(m_output_file_path, output_file);
    }
}

const char *Configuration::getOutputFilePath() const
{
    return m_output_file_path;
}

bool Configuration::shouldRecordSyscallEntryTimestamp() const
{
    return m_record_syscall_entry_timestamp_option;
}

bool Configuration::shouldRecordSyscallDuration() const
{
    return m_record_syscall_duration_option;
}
