#pragma once
#include <string>
#include <vector>

struct Profile
{
    std::string name;
    std::string subnet;
    std::string proxy;
    bool enableGit = false;
};

struct AppConfig
{
    bool loggingEnabled = false;
    std::string logFile = "autoproxy.log";
    std::vector<Profile> profiles;
};

AppConfig LoadConfig(const std::string& filename);