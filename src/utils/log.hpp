#pragma once

#include <string_view>
#include <format>
#include <utility>
#include <fstream>

namespace Utils {

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

std::string getTimeStamp();

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
        writeLine(level, comp, std::format(fmt, std::forward<Args>(args)...));
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

    static void openLogFile(std::string& file_name);

private:
    /* Private functions */
    static void writeLine(LogLevel level, std::string_view comp, std::string_view msg);

    /* Private variables */
    static inline LogLevel m_log_level = LogLevel::Info;
    static inline bool m_log_to_file = true;
    static inline std::ofstream m_log_file;
};

} // namespace Utils