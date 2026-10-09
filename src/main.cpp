#include <iostream>

#include "utils/log.hpp"

using namespace Utils;

int main(int argc, char** argv) {
    Log::debug("render", "x = {}", 42);
    Log::info("render", "x = {}", 42);
    Log::warning("render", "x = {}", 42);
    Log::error("render", "x = {}", 42);

    std::string file_path = "log.txt";
    Log::openLogFile(file_path);
    Log::debug("render", "x = {}", 42);
    Log::info("render", "x = {}", 42);
    Log::warning("render", "x = {}", 42);
    Log::error("render", "x = {}", 42);
}