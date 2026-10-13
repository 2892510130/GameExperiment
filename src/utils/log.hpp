#pragma once

#include <string_view>
#include <format>
#include <utility>
#include <fstream>
#include <vector>

#include "synchronised.hpp"

namespace Utils {

// Some basic functions
std::string getTimeStamp();

// Log level enum class
enum class LogLevel: int {
    Debug   = 0,
    Info    = 1,
    Warning = 2,
    Error   = 3
};

inline constexpr std::string_view toColor(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "\033[36m";
        case LogLevel::Info:    return "\033[32m";
        case LogLevel::Warning: return "\033[33m";
        case LogLevel::Error:   return "\033[31m";
    }
    return "\033[0m";
}

inline constexpr std::string_view toString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error:   return "ERROR";
    }
    return "?";
}

// Log data components
struct LogLine
{
    LogLevel m_log_level;
    std::string m_comp_name;
    std::string m_line;
};


// Static log class
class Log {
public:
    /* Public functions */
    template <typename... Args>
    static void log(
        LogLevel level,
        std::string_view comp,
        std::format_string<Args...> fmt,
        Args&&... args
    ) {
        if (level < m_log_level) return;
        writeLog(level, comp, std::format(fmt, std::forward<Args>(args)...));
    }

#define LOG(NAME, LEVEL)                                    \
    template <typename... Args>                             \
    static void NAME(                                       \
        std::string_view comp,                              \
        std::format_string<Args...> fmt,                    \
        Args&&... args                                      \
    ) {                                                     \
        log(LEVEL, comp, fmt, std::forward<Args>(args)...); \
    }

    LOG(debug,   LogLevel::Debug);
    LOG(info,    LogLevel::Info);
    LOG(warning, LogLevel::Warning);
    LOG(error,   LogLevel::Error);
#undef LOG

    static void setLogLevel(int log_level) {
        if (log_level < 0) log_level = 0;
        if (log_level > 3) log_level = 3;
        m_log_level = static_cast<LogLevel>(log_level);
    }

    static LogLevel getLogLevel() {
        return m_log_level;
    }

    static void setBufferSize(int buffer_size) {
        m_buffer_size = buffer_size;
    }

    static void openLogFile(std::string& file_name);

    static void flushBuffer();

private:
    /* Private functions */
    static void writeLine(LogLevel level, std::string_view comp, std::string_view msg);
    static void writeLog(LogLevel level, std::string_view comp, std::string_view msg);

    /* Private variables */
    static inline LogLevel m_log_level = LogLevel::Info;
    static inline bool m_log_to_file = true;
    static inline std::ofstream m_log_file;
    static inline Synchronised<std::vector<LogLine>> m_buffer;
    static inline size_t m_buffer_size = 1;
    static inline std::mutex m_output_mutex;
};

} // namespace Utils