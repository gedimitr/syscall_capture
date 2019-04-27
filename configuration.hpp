#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "memory.hpp"

class Configuration
{
public:
    Configuration();

    const char *getOutputFilePath() const;

    bool shouldRecordSyscallEntryTimestamp() const;

    bool shouldRecordSyscallDuration() const;

    bool isMaxCapturedDataLengthDefined() const;
    unsigned int getMaxCapturedDataLength() const;

    Configuration(const Configuration &) = delete;
    Configuration &operator=(const Configuration &) = delete;

private:
    MappedMemory m_output_file_path;
    bool m_record_syscall_entry_timestamp_option{true};
    bool m_record_syscall_duration_option{true};
    unsigned int m_max_captured_data_length{0};

    class ConfigurationLoader;
};

#endif // CONFIGURATION_HPP
