#include "Window.h"
#include "resource.h"

Window::Window() : hWnd(nullptr), hBrowseButton(nullptr), hRunButton(nullptr),
hFileLabel(nullptr), hFont(nullptr), hInstance(nullptr), hBackgroundBrush(nullptr), hIcon(nullptr) {
}

Window::~Window() {
    if (hFont) DeleteObject(hFont);
    if (hBackgroundBrush) DeleteObject(hBackgroundBrush);
    if (hIcon) DestroyIcon(hIcon);
}

bool Window::Create(HINSTANCE hInstance) {
    this->hInstance = hInstance;

    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(54, 57, 62));
    wc.lpszClassName = L"PrototypeLauncher";
    wc.hIcon = hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_APP_ICON));
    wc.hIconSm = hIcon;

    if (!RegisterClassExW(&wc)) return false;

    hBackgroundBrush = CreateSolidBrush(RGB(54, 57, 62));

    const int windowWidth = 400;
    const int windowHeight = 300;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int xPos = (screenWidth - windowWidth) / 2;
    int yPos = (screenHeight - windowHeight) / 2;

    hWnd = CreateWindowW(
        L"PrototypeLauncher",
        L"Prototype Launcher",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        xPos, yPos, windowWidth, windowHeight,
        nullptr, nullptr, hInstance, this
    );

    return hWnd != nullptr;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Window* pThis = nullptr;

    if (message == WM_NCCREATE) {
        pThis = static_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->hWnd = hWnd;
    }
    else {
        pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        switch (message) {
        case WM_CREATE:
            pThis->CreateControls();
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_BROWSE_BUTTON) {
                pThis->OpenFileDialog();
            }
            else if (LOWORD(wParam) == IDC_RUN_BUTTON) {
                ProcessHandler::LaunchProcess(pThis->selectedFile);
                PostQuitMessage(0);
            }
            break;

        case WM_USER_UPDATE_FILE:
            pThis->UpdateFileLabel(pThis->selectedFile);
            EnableWindow(pThis->hRunButton, TRUE);
            InvalidateRect(pThis->hRunButton, nullptr, TRUE);
            break;

        case WM_CTLCOLORSTATIC:
        {
            HDC hdc = reinterpret_cast<HDC>(wParam);
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkColor(hdc, RGB(54, 57, 62));
            return reinterpret_cast<LRESULT>(pThis->hBackgroundBrush);
        }

        case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT pDIS = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
            if (pDIS->CtlID == IDC_BROWSE_BUTTON || pDIS->CtlID == IDC_RUN_BUTTON) {
                HDC hdc = pDIS->hDC;
                RECT rc = pDIS->rcItem;

                COLORREF bgColor = (pDIS->CtlID == IDC_BROWSE_BUTTON) ?
                    RGB(126, 97, 171) :
                    (IsWindowEnabled(pDIS->hwndItem) ?
                        RGB(76, 175, 80) :
                        RGB(128, 128, 128));

                HBRUSH hBrush = CreateSolidBrush(bgColor);
                FillRect(hdc, &rc, hBrush);
                DeleteObject(hBrush);

                wchar_t buttonText[256];
                GetWindowTextW(pDIS->hwndItem, buttonText, 256);
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkMode(hdc, TRANSPARENT);
                DrawTextW(hdc, buttonText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                if (pDIS->itemState & ODS_FOCUS) {
                    DrawFocusRect(hdc, &rc);
                }
                return TRUE;
            }
        }
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcW(hWnd, message, wParam, lParam);
        }
        return 0;
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

void Window::CreateControls() {
    hFont = CreateFontW(
        16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI"
    );

    CreateWindowW(
        L"STATIC", L"Select the .exe of the Prototype game.",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        0, 50, 400, 20,
        hWnd, nullptr, hInstance, nullptr
    );

    hBrowseButton = CreateWindowW(
        L"BUTTON", L"Browse",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        150, 100, 100, 30,
        hWnd, (HMENU)IDC_BROWSE_BUTTON, hInstance, nullptr
    );

    hFileLabel = CreateWindowW(
        L"STATIC", L"Selected file: ",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        0, 150, 400, 20,
        hWnd, (HMENU)IDC_FILE_LABEL, hInstance, nullptr
    );

    hRunButton = CreateWindowW(
        L"BUTTON", L"Run",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_DISABLED,
        150, 200, 100, 30,
        hWnd, (HMENU)IDC_RUN_BUTTON, hInstance, nullptr
    );

    HWND controls[] = { hBrowseButton, hRunButton, hFileLabel };
    for (auto hwnd : controls) {
        SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
    }

    SetWindowCursor(hBrowseButton, IDC_HAND);
    SetWindowCursor(hRunButton, IDC_HAND);
}

void Window::OpenFileDialog() {
    OPENFILENAMEW ofn = { 0 };
    wchar_t szFile[260] = { 0 };

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(*szFile);
    ofn.lpstrFilter = L"Executable Files (*.exe)\0*.exe\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    std::wstring defaultPath = L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Prototype\\";
    if (GetFileAttributesW((defaultPath + L"prototypef.exe").c_str()) != INVALID_FILE_ATTRIBUTES) {
        ofn.lpstrInitialDir = defaultPath.c_str();
    }

    if (GetOpenFileNameW(&ofn)) {
        selectedFile = szFile;
        PostMessage(hWnd, WM_USER_UPDATE_FILE, 0, 0);
    }
}

void Window::UpdateFileLabel(const std::wstring& filename) {
    std::wstring filenameOnly = filename.substr(filename.find_last_of(L"\\") + 1);
    std::wstring text = L"Selected file: " + filenameOnly;
    SetWindowTextW(hFileLabel, text.c_str());
}

void SetWindowTextColor(HWND hwnd, COLORREF color) {
    SetWindowLongPtr(hwnd, GWLP_USERDATA, color);
    InvalidateRect(hwnd, nullptr, TRUE);
}

void SetWindowBackgroundColor(HWND hwnd, COLORREF color) {
    SetWindowLongPtr(hwnd, GWLP_USERDATA, color);
    InvalidateRect(hwnd, nullptr, TRUE);
}

void SetWindowCursor(HWND hwnd, LPCWSTR cursor) {
    SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursorW(nullptr, cursor));
}