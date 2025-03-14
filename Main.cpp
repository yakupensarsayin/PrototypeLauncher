#include "Main.h"
#include "Window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    Window appWindow;

    if (!appWindow.Create(hInstance)) {
        return 1;
    }

    // Show and update window
    ShowWindow(appWindow.GetHandle(), nCmdShow);
    UpdateWindow(appWindow.GetHandle());

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}