#include "../include/Env.h"
#include <fstream>
#include <sstream>

EnvConfig LoadEnv(const std::string &filename)
{
    EnvConfig env;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') &&
            std::getline(iss, value))
        {
            if (key == "GIT_PROXY_USER")
                env.gitUser = value;
            else if (key == "GIT_PROXY_PASS")
                env.gitPass = value;
        }
    }

    return env;
}