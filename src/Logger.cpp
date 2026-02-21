#include "../include/Logger.h"
#include <fstream>
#include <ctime>

static bool g_enabled = false;
static std::string g_logFile;

void InitLogger(bool enabled,
                const std::string &file)
{
    g_enabled = enabled;
    g_logFile = file;
}

void Log(const std::string &message)
{
    if (!g_enabled)
        return;

    std::ofstream file(g_logFile, std::ios::app);

    std::time_t now = std::time(nullptr);
    file << std::ctime(&now) << " : "
         << message << "\n";
}