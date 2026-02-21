#include "../include/Config.h"
#include <fstream>
#include <sstream>

AppConfig LoadConfig(const std::string& filename)
{
    AppConfig config;
    std::ifstream file(filename);
    std::string line;

    Profile currentProfile;
    bool insideProfile = false;
    bool insideGlobal = false;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        // Section detection
        if (line.front() == '[' && line.back() == ']')
        {
            std::string section = line.substr(1, line.size() - 2);

            if (section == "Global")
            {
                if (insideProfile)
                    config.profiles.push_back(currentProfile);

                insideGlobal = true;
                insideProfile = false;
            }
            else
            {
                if (insideProfile)
                    config.profiles.push_back(currentProfile);

                currentProfile = Profile();
                currentProfile.name = section;

                insideProfile = true;
                insideGlobal = false;
            }

            continue;
        }

        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') &&
            std::getline(iss, value))
        {
            if (insideGlobal)
            {
                if (key == "logging")
                    config.loggingEnabled = (value == "true");
                else if (key == "logfile")
                    config.logFile = value;
            }
            else if (insideProfile)
            {
                if (key == "subnet")
                    currentProfile.subnet = value;
                else if (key == "proxy")
                    currentProfile.proxy = value;
                else if (key == "enable_git")
                    currentProfile.enableGit = (value == "true");
            }
        }
    }

    if (insideProfile)
        config.profiles.push_back(currentProfile);

    return config;
}