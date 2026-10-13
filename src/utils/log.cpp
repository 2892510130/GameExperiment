#include "log.hpp"

#include <iostream>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

namespace Utils {

std::string getTimeStamp() {
    auto now = std::chrono::system_clock::now();
    return std::format("{:%Y-%m-%d %H:%M:%S}", now);
}

void Log::writeLog(LogLevel level, std::string_view comp, std::string_view msg) {
    if (m_buffer_size <= 1) {
        writeLine(level, comp, msg);
        return;
    }

    bool should_flush = false;

    m_buffer.lock();
    auto& buffer = m_buffer.getData();

    buffer.push_back({ level, std::string(comp), std::string(msg) });

    should_flush = buffer.size() >= m_buffer_size;
    m_buffer.unlock();

    if (should_flush) {
        flushBuffer();
    }
}

void Log::flushBuffer() {
    std::vector<LogLine> lines;

    m_buffer.lock();
    lines.swap(m_buffer.getData());
    m_buffer.unlock();

    for (const auto& line : lines) {
        writeLine(line.m_log_level, line.m_comp_name, line.m_line);
    }
}

void Log::writeLine(LogLevel level, std::string_view comp, std::string_view msg) {
    std::lock_guard<std::mutex> lock(m_output_mutex);
    
    std::string tag = std::format("[{}]", toString(level));
    tag.resize(std::max<size_t>(tag.size(), 9), ' ');

    if (m_log_to_file && m_log_file) {
        m_log_file << "[" << getTimeStamp() << "] "
                   << tag
                   << " [" << comp << "] "
                   << msg << "\n";
    } else {
        std::cout << "\033[90m[" << getTimeStamp() << "]\033[0m " // time stamp
                  << toColor(level) << tag << "\033[0m"           // log level
                  << " [" << comp << "] "                         // comp or module name
                  << msg << "\n";                                 // the message
    }
}

void Log::openLogFile(std::string& file_name) {
    fs::path file_path = fs::path(file_name);
    m_log_file.open(file_path);
    if (!m_log_file) {
        Log::error("FileSystem", "Can not open log file!");
    }
}

} // namespace Utils