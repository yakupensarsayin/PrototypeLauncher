#pragma once
#include "Main.h"
#include "ProcessHandler.h"

class Window {
public:
    Window();
    ~Window();
    bool Create(HINSTANCE hInstance);
    HWND GetHandle() const { return hWnd; }

private:
    HWND hWnd;
    HWND hBrowseButton;
    HWND hRunButton;
    HWND hFileLabel;
    HFONT hFont;
    std::wstring selectedFile;
    HINSTANCE hInstance;
    HBRUSH hBackgroundBrush;
    HICON hIcon;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void CreateControls();
    void UpdateFileLabel(const std::wstring& filename);
    void OpenFileDialog();

    void SetWindowTextColor(HWND hwnd, COLORREF color);
    void SetWindowBackgroundColor(HWND hwnd, COLORREF color);
    void SetWindowCursor(HWND hwnd, LPCWSTR cursor);
};