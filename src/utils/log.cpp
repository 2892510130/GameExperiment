#include "log.hpp"

#include <iostream>
#include <chrono>

namespace Utils {

std::string getTimeStamp() {
    auto now = std::chrono::system_clock::now();
    return std::format("{:%Y-%m-%d %H:%M:%S}", now);
}

void Log::printMessage(LogLevel level, std::string_view comp, std::string_view msg) {
    std::string tag = std::format("{:<9}", std::format("[{}]", toString(level)));
    std::cout << "\033[90m[" << getTimeStamp() << "]\033[0m " // time stamp
              << toColor(level) << tag << "\033[0m"           // log level
              << " [" << comp << "] "                         // comp or module name
              << msg << "\n";                                 // the message
}

} // namespace Utils