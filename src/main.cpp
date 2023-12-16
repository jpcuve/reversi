#include <iostream>
#include <set>
#include "resource.h"

struct WindowData {
    std::set<WPARAM> keys {};
};

LRESULT WindowProcW(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    const auto pWindowData = reinterpret_cast<WindowData *>(GetWindowLongPtr(hWnd, 0));
    switch(message) {
        case WM_KEYDOWN:{
            pWindowData->keys.insert(wParam);
            break;
        }
        case WM_KEYUP:{
            pWindowData->keys.erase(wParam);
            if (wParam == VK_ESCAPE) PostQuitMessage(0);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_DPICHANGED: {
            const auto dpiX {LOWORD(wParam)};
            const auto dpiY {HIWORD(wParam)};
            std::cout << "Dpi (" << dpiX << ", " << dpiY << ")" << std::endl;
            const auto size {reinterpret_cast<RECT*>(lParam)};
            SetWindowPos(hWnd, nullptr, size->left, size->top, size->right - size->left, size->bottom - size->top, 0);
            break;
        }
        default:
            break;
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    std::cout << "Starting application" << std::endl;
    if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) return 0;

    std::cout << "Registering window class" << std::endl;
    auto pWindowClassName = L"reversi";
    WNDCLASSEXW wcex {
            sizeof(WNDCLASSEXW),
            CS_HREDRAW | CS_VREDRAW,
            WindowProcW,
            0,
            sizeof(void *),
            hInstance,
            nullptr,
            LoadCursor(nullptr, IDC_ARROW),
            static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)),
            MAKEINTRESOURCEW(IDR_REVERSI_MENU),
            pWindowClassName,
            nullptr,
    };
    if (!RegisterClassExW(&wcex)) return 0;

    std::cout << "Creating window" << std::endl;
    SIZE screenSize {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
    SIZE windowSize {640, 480};
    auto hWnd = CreateWindowW(
            pWindowClassName,
            L"Reversi",
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
            (screenSize.cx - windowSize.cx) / 2,
            (screenSize.cy - windowSize.cy) / 2,
            windowSize.cx,
            windowSize.cy,
            nullptr,
            nullptr,
            hInstance,
            nullptr);
    if (!hWnd) return 0;
    WindowData windowData;
    SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONG_PTR>(&windowData));

    std::cout << "Showing window" << std::endl;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    MSG msg;
    auto done = false;
    while (!done) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        done = msg.message == WM_QUIT;
    }
    std::cout << "Unregistering window class" << std::endl;
    UnregisterClassW(pWindowClassName, hInstance);
    return static_cast<int>(msg.wParam);
}
