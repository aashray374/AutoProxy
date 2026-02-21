#include "../include/ProxyManager.h"
#include "../include/Logger.h"
#include "../include/Tray.h"
#include <windows.h>
#include <wininet.h>
#include <iostream>

#pragma comment(lib, "wininet.lib")

void SetProxy(bool enable,const std::string& proxyServer){
    HKEY hKey;

    if (RegOpenKeyExA(HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
        0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS){
        std::cout << "Registry open failed\n";
        return;
    }

    DWORD value = enable ? 1 : 0;

    RegSetValueExA(hKey, "ProxyEnable",
        0, REG_DWORD,
        (BYTE*)&value, sizeof(value));

    if (enable){
        RegSetValueExA(hKey, "ProxyServer",
            0, REG_SZ,
            (BYTE*)proxyServer.c_str(),
            proxyServer.size() + 1);
    }

    RegCloseKey(hKey);

    InternetSetOption(NULL,
        INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
    InternetSetOption(NULL,
        INTERNET_OPTION_REFRESH, NULL, 0);

    Log(enable ? "Proxy Enabled"
           : "Proxy Disabled");
    

    std::cout << (enable ? "Proxy ENABLED\n"
                         : "Proxy DISABLED\n");
}