#include "../include/NetworkUtils.h"
#include <windows.h>
#include <iphlpapi.h>
#include <winsock2.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

std::string GetLocalIPv4(){
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD size = sizeof(adapterInfo);

    if (GetAdaptersInfo(adapterInfo, &size) == ERROR_SUCCESS)
    {
        PIP_ADAPTER_INFO p = adapterInfo;

        while (p)
        {
            if (p->Type == IF_TYPE_IEEE80211 ||
                p->Type == MIB_IF_TYPE_ETHERNET)
            {
                if (std::string(p->IpAddressList.IpAddress.String) != "0.0.0.0")
                    return p->IpAddressList.IpAddress.String;
            }
            p = p->Next;
        }
    }

    return "";
}

bool IsInSubnet(const std::string& ip,const std::string& cidr){
    size_t slash = cidr.find('/');
    std::string network = cidr.substr(0, slash);
    int prefix = std::stoi(cidr.substr(slash + 1));

    uint32_t ipAddr = ntohl(inet_addr(ip.c_str()));
    uint32_t netAddr = ntohl(inet_addr(network.c_str()));

    uint32_t mask = 0xFFFFFFFF << (32 - prefix);

    return (ipAddr & mask) == (netAddr & mask);
}