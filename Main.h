#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <string>

// Window messages
#define WM_USER_UPDATE_FILE WM_USER + 1

// Control IDs
#define IDC_BROWSE_BUTTON 1001
#define IDC_RUN_BUTTON 1002
#define IDC_FILE_LABEL 1003