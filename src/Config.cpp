#include "../include/Config.h"
#include <fstream>
#include <sstream>

AppConfig LoadConfig(const std::string& filename){
    AppConfig config;
    config.loggingEnabled = false;

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') &&
            std::getline(iss, value))
        {
            if (key == "subnet")
                config.subnet = value;
            else if (key == "proxy")
                config.proxy = value;
            else if (key == "logging")
                config.loggingEnabled = (value == "true");
            else if (key == "logfile")
                config.logFile = value;
        }
    }

    return config;
}