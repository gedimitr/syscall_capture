#include <cstdlib>
#include <cstring>
#include <optional>
#include <string_view>
#include <tuple>

#include "configuration.hpp"

namespace {

constexpr std::string_view TRUE_STR("true");

const char *CONFIG_ENVVAR = "SYSCAP_CONF";

constexpr std::string_view OUTPUT_PATH("OUTPUT_PATH");
constexpr std::string_view RECORD_TIMES("RECORD_TIMES");
constexpr std::string_view RECORD_DURATION("RECORD_DURATION");
constexpr std::string_view MAX_DATA_LENGTH("MAX_DATA_LENGTH");

class StringSplitter
{
public:
    StringSplitter(const std::string_view &string, char separator);

    void advance();
    bool isValid() const;

    std::string_view getSubstring() const;

private:
    void adjustLengthUntilSeparator();

    const std::string_view &m_string;
    char m_separator;
    unsigned long m_start;
    unsigned long m_length;
};

StringSplitter::StringSplitter(const std::string_view &string, char separator) :
    m_string(string),
    m_separator(separator),
    m_start(0),
    m_length(0)
{
    adjustLengthUntilSeparator();
}

void StringSplitter::advance()
{
    m_start += m_length + 1;
    adjustLengthUntilSeparator();
}

bool StringSplitter::isValid() const
{
    return m_start < m_string.length();
}

std::string_view StringSplitter::getSubstring() const
{
    if (isValid()) {
        return std::string_view(m_string.data() + m_start, m_length);
    } else {
        return std::string_view();
    }
}

void StringSplitter::adjustLengthUntilSeparator()
{
    if (m_start >= m_string.length()) {
        return;
    }

    unsigned long separator_index = m_string.find(m_separator, m_start);
    if (separator_index != std::string_view::npos) {
        m_length = separator_index - m_start;
    } else {
        m_length = m_string.length() - m_start;
    }
}

std::tuple<std::string_view, std::string_view> splitKeyValueString(const std::string_view &key_value_string)
{
    unsigned long index = key_value_string.find('=');
    if (index != std::string_view::npos) {
        std::string_view key = std::string_view(key_value_string.data(), index);

        unsigned long value_length = key_value_string.length() - index - 1;
        std::string_view value = std::string_view(key_value_string.data() + index + 1, value_length);

        return std::make_tuple(key, value);
    } else {
        return std::make_tuple(key_value_string, std::string_view());
    }
}

bool isTrueString(const std::string_view &string)
{
    return string == TRUE_STR;
}

std::optional<long> parseInteger(const std::string_view &string)
{
    long v = 0;

    for (char c : string) {
        if (isdigit(c)) {
            v = 10 * v + (c - '0');
        } else {
            return std::optional<long>();
        }
    }

    return std::optional<long>(v);
}

}

class Configuration::ConfigurationLoader
{
public:
    ConfigurationLoader(Configuration &configuration);

    void processString(const std::string_view &conf_string);

private:
    void loadConfigurationKeyValue(const std::string_view &conf_key, const std::string_view &conf_value);

    Configuration &m_configuration;
};

Configuration::ConfigurationLoader::ConfigurationLoader(Configuration &configuration) :
    m_configuration{configuration} { }

void Configuration::ConfigurationLoader::processString(const std::string_view &conf_string)
{
    for (StringSplitter walker(conf_string, ':'); walker.isValid(); walker.advance()) {
        std::string_view key_value_string = walker.getSubstring();
        auto [key, value] = splitKeyValueString(key_value_string);
        loadConfigurationKeyValue(key, value);
    }
}

void Configuration::ConfigurationLoader::loadConfigurationKeyValue(const std::string_view &conf_key, const std::string_view &conf_value)
{
    if (conf_key == OUTPUT_PATH) {
        unsigned long value_length = conf_value.length();
        if (value_length > 0) {
            MappedMemory &mem = m_configuration.m_output_file_path;
            mem.allocate(value_length + 1);
            memcpy(mem.data(), conf_value.data(), value_length);
            static_cast<char *>(mem.data())[value_length + 1] = '\0';
        }
    } else if (conf_key == RECORD_TIMES) {
        m_configuration.m_record_syscall_times_option = isTrueString(conf_value);
    } else if (conf_key == MAX_DATA_LENGTH) {
        std::optional<long> parsed_int = parseInteger(conf_value);
        if (parsed_int) {
            m_configuration.m_max_captured_data_length = static_cast<unsigned int>(*parsed_int);
        }
    }
}

Configuration::Configuration()
{
    const char *conf_string = std::getenv(CONFIG_ENVVAR);

    if (conf_string) {
        ConfigurationLoader conf_loader(*this);
        conf_loader.processString(conf_string);
    }
}

const char *Configuration::getOutputFilePath() const
{
    return static_cast<const char *>(m_output_file_path.data());
}

bool Configuration::shouldRecordSyscallTimes() const
{
    return m_record_syscall_times_option;
}

bool Configuration::isMaxCapturedDataLengthDefined() const
{
    return m_max_captured_data_length > 0;
}

unsigned int Configuration::getMaxCapturedDataLength() const
{
    return m_max_captured_data_length;
}

