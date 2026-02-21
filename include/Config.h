#pragma once
#include <string>

struct AppConfig{
    std::string subnet;
    std::string proxy;
    bool loggingEnabled;
    std::string logFile;
};

AppConfig LoadConfig(const std::string& filename);