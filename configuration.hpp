#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

class Configuration
{
public:
    Configuration();

    const char *getOutputFilePath() const;
    bool shouldRecordSyscallEntryTimestamp() const;
    bool shouldRecordSyscallDuration() const;

private:
    static constexpr int MAX_PATH_LENGTH = 255;

    char m_output_file_path[MAX_PATH_LENGTH + 1];
    bool m_record_syscall_entry_timestamp_option;
    bool m_record_syscall_duration_option;
};

#endif // CONFIGURATION_HPP
