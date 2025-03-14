#pragma once
#include "Main.h"

class ProcessHandler {
public:
    static bool LaunchProcess(const std::wstring& exePath);
};