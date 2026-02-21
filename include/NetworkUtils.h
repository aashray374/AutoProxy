#pragma once
#include <string>

std::string GetLocalIPv4();
bool IsInSubnet(const std::string& ip,const std::string& cidr);