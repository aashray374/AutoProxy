#pragma once
#include <string>

void InitLogger(bool enabled,const std::string& file);

void Log(const std::string& message);