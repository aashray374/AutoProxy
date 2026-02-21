#include "../include/Monitor.h"
#include <windows.h>
#include <iphlpapi.h>
#include <iostream>

#pragma comment(lib, "iphlpapi.lib")

void MonitorNetwork(void (*callback)())
{
    HANDLE handle;
    OVERLAPPED overlapped = {};

    while (true)
    {
        NotifyAddrChange(&handle, &overlapped);
        WaitForSingleObject(handle, INFINITE);

        std::cout << "\nNetwork change detected\n";
        callback();
    }
}