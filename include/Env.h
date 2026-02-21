#pragma once
#include <string>

struct EnvConfig
{
    std::string gitUser;
    std::string gitPass;
};

EnvConfig LoadEnv(const std::string& filename);