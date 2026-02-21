#include "../include/NetworkUtils.h"
#include "../include/ProxyManager.h"
#include "../include/Monitor.h"
#include "../include/Config.h"
#include "../include/Logger.h"

#include <iostream>

AppConfig config;

void UpdateProxy()
{
    std::string ip = GetLocalIPv4();

    if (ip.empty())
    {
        Log("No IP detected");
        SetProxy(false, config.proxy);
        return;
    }

    Log("Current IP: " + ip);

    bool inside = IsInSubnet(ip, config.subnet);

    Log(std::string("Inside subnet: ")
        + (inside ? "YES" : "NO"));

    SetProxy(inside, config.proxy);
}

int main()
{
    config = LoadConfig("config.ini");

    InitLogger(config.loggingEnabled,
               config.logFile);

    Log("Application started");

    UpdateProxy();
    MonitorNetwork(UpdateProxy);

    return 0;
}