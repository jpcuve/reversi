#include <iostream>

#include "Window.h"
#include "BoardWindow.h"
#include "Game.h"
#include "MainWindow.h"
#include "StatusWindow.h"
#include "resource.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    std::cout << "Starting application" << std::endl;
    if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) return 0;
    const std::vector<WNDCLASSEXW> wndClasses = {
        {
            sizeof(WNDCLASSEXW),
            0,
            Window::WindowProcW,
            0,
            sizeof(void *),
            hInstance,
            nullptr,
            nullptr,
            static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
            nullptr,
            StatusWindow::CLASS_NAME,
            nullptr,
        },{
            sizeof(WNDCLASSEXW),
            0,
            Window::WindowProcW,
            0,
            sizeof(void *),
            hInstance,
            nullptr,
            LoadCursor(nullptr, IDC_HAND),
            nullptr,
            nullptr,
            BoardWindow::CLASS_NAME,
            nullptr,
        },{
            sizeof(WNDCLASSEXW),
            CS_HREDRAW | CS_VREDRAW,
            Window::WindowProcW,
            0,
            sizeof(void *),
            hInstance,
            LoadIcon(hInstance, MAKEINTRESOURCE(2)),
            LoadCursor(nullptr, IDC_ARROW),
            static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)),
            MAKEINTRESOURCEW(IDR_REVERSI_MENU),
            MainWindow::CLASS_NAME,
            nullptr,
        }
    };
    for (const auto& wndClass : wndClasses) {
        if (!RegisterClassExW(&wndClass)) return 0;
    }

    std::cout << "Creating window" << std::endl;
    Game game;
    const MainWindow main_window {hInstance, game};

    std::cout << "Showing window" << std::endl;
    ShowWindow(main_window.handle(), nCmdShow);
    UpdateWindow(main_window.handle());
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    for (const auto& wndClass : wndClasses) {
        UnregisterClassW(wndClass.lpszClassName, hInstance);
    }

    return static_cast<int>(msg.wParam);
}

void ThrowIfFailed(HRESULT result) {
    if (result != S_OK && result != S_FALSE) {
        switch (result) {
        case E_INVALIDARG: std::cerr << "Invalid arguments or parameters" << std::endl;
            break;
        case E_FAIL: std::cerr << "Fail" << std::endl;
            break;
        default: std::cerr << "Unrecognized error" << std::endl;
            break;
        }
        throw std::runtime_error("Failed");
    }
}

void ThrowIfNull(void* result) {
    if (!result) throw std::runtime_error("nullptr");
}

void ThrowIfFalse(bool result) {
    if (!result) throw std::runtime_error("false");
}

