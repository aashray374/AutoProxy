#include "../include/NetworkUtils.h"
#include "../include/ProxyManager.h"
#include "../include/Monitor.h"
#include "../include/Config.h"
#include "../include/Logger.h"
#include "../include/Env.h"
#include "../include/GitProxy.h"

#include <iostream>

AppConfig config;
EnvConfig env;

void UpdateProxy()
{
    std::string ip = GetLocalIPv4();

    if (ip.empty())
    {
        Log("No IP detected");
        SetProxy(false, config.proxy);

        if (config.enableGit)
            SetGitProxy(false, config.proxy,
                        env.gitUser, env.gitPass);

        return;
    }

    bool inside = IsInSubnet(ip, config.subnet);

    SetProxy(inside, config.proxy);

    if (config.enableGit)
        SetGitProxy(inside, config.proxy,
                    env.gitUser, env.gitPass);
}

int main()
{
    config = LoadConfig("config.ini");
    env = LoadEnv(".env");

    InitLogger(config.loggingEnabled,
               config.logFile);

    Log("Application started");

    UpdateProxy();
    MonitorNetwork(UpdateProxy);

    return 0;
}