#include "ProcessHandler.h"

bool ProcessHandler::LaunchProcess(const std::wstring& exePath) {
    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION pi = { 0 };

    // Create process in suspended state
    if (!CreateProcessW(
        exePath.c_str(),    // Path to executable
        nullptr,           // Command line arguments
        nullptr,           // Process security attributes
        nullptr,           // Thread security attributes
        FALSE,            // Handle inheritance
        CREATE_SUSPENDED, // Creation flags
        nullptr,          // Environment
        nullptr,          // Current directory
        &si,             // Startup info
        &pi              // Process info
    )) {
        MessageBoxW(nullptr, L"Failed to create process!", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    // Set process affinity to cores 0-3
    DWORD_PTR affinityMask = 0b1111; // First 4 cores
    if (!SetProcessAffinityMask(pi.hProcess, affinityMask)) {
        MessageBoxW(nullptr, L"Failed to set process affinity!", L"Error", MB_OK | MB_ICONERROR);
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return false;
    }

    // Resume the suspended process
    if (ResumeThread(pi.hThread) == -1) {
        MessageBoxW(nullptr, L"Failed to resume process!", L"Error", MB_OK | MB_ICONERROR);
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return false;
    }

    // Clean up handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}