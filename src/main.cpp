#include <iostream>

#include "Game.h"
#include "resource.h"

LRESULT WindowProcW(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    const auto pGame = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, 0));
    switch(message) {
        case WM_KEYDOWN:{
            break;
        }
        case WM_KEYUP:{
            if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
            break;
        }
        case WM_DESTROY:
            std::cout << "Destroying window" << std::endl;
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDM_FILE_EXIT:
                    DestroyWindow(hWnd);
                    break;
            }
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            const auto hdc {BeginPaint(hWnd, &ps)};
            const auto hGreenBrush {CreateSolidBrush(RGB(0, 0x80, 0))};
            RECT r;
            GetClientRect(hWnd, &r);
            const auto w {r.right - r.left};
            const auto h {r.bottom - r.top};
            const auto e {min(w, h)};
            const auto dx {(w - e) / 2};
            const auto dy {(h - e) / 2};
            const auto s {e / pGame->GetSize()};
            const auto d {s / 10};
            for (int row {0}; row < pGame->GetSize(); row++) {
                for (int col {0}; col < pGame->GetSize(); col++) {
                    const auto x {dx + s * col};
                    const auto y {dy + s * row};
                    SelectObject(hdc, hGreenBrush);
                    Rectangle(hdc, x, y, x + s, y + s);
                    if (pGame->GetToken(col, row) == 'O') {
                        SelectObject(hdc, GetStockObject(WHITE_BRUSH));
                        Ellipse(hdc, x + d, y + d, x + s - d, y + s - d);
                    }
                    if (pGame->GetToken(col, row) == 'X') {
                        SelectObject(hdc, GetStockObject(BLACK_BRUSH));
                        Ellipse(hdc, x + d, y + d, x + s - d, y + s - d);
                    }
                }
            }
            DeleteObject(hGreenBrush);
            EndPaint(hWnd, &ps);
            break;
        }
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
    Game game;
    SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONG_PTR>(&game));

    std::cout << "Showing window" << std::endl;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    std::cout << "Unregistering window class" << std::endl;
    UnregisterClassW(pWindowClassName, hInstance);
    return static_cast<int>(msg.wParam);
}
