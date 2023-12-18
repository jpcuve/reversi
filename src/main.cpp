#include <iostream>

#include "game.h"
#include "resource.h"

LRESULT BoardWindowProcW(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    const auto pGame = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, 0));
    switch(message) {
        case WM_CREATE: {
            const auto pCreateStruct {reinterpret_cast<CREATESTRUCTW*>(lParam)};
            SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            const auto hdc {BeginPaint(hWnd, &ps)};
            const auto hGreenBrush {CreateSolidBrush(RGB(0, 0x80, 0))};
            const auto hRedBrush {CreateSolidBrush(RGB(0xFF, 0, 0))};
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
                    const auto token {pGame->GetToken({col, row})};
                    if (token != TOKEN_EMPTY){
                        switch(token){
                            case TOKEN_WHITE:
                                SelectObject(hdc, GetStockObject(WHITE_BRUSH));
                                break;
                            case TOKEN_BLACK:
                                SelectObject(hdc, GetStockObject(BLACK_BRUSH));
                                break;
                            case TOKEN_TEST:
                                SelectObject(hdc, hRedBrush);
                                break;
                            default:
                                break;
                        }
                        Ellipse(hdc, x + d, y + d, x + s - d, y + s - d);
                    }
                }
            }
            DeleteObject(hGreenBrush);
            EndPaint(hWnd, &ps);
            break;
        }
        default:
            break;
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

LRESULT ReversiWindowProcW(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    const auto pGame = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, 0));
    switch(message) {
        case WM_CREATE: {
            const auto pCreateStruct {reinterpret_cast<CREATESTRUCTW*>(lParam)};
            SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            std::cout << "Create main window" << std::endl;
            auto hBoard {CreateWindowW(
                L"board",
                nullptr,
                WS_CHILD,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                hWnd,
                reinterpret_cast<HMENU>(1),  // identifier of child window
                nullptr,
                pCreateStruct->lpCreateParams)};
            ShowWindow(hBoard, SW_SHOWNORMAL);
            std::cout << "Child window created: " << hBoard << std::endl;
            return 0;
        }
        case WM_SIZE: {
            auto w {LOWORD(lParam)};
            auto h {HIWORD(lParam)};
            auto e {min(w, h)};
            auto hBoard {FindWindowExW(hWnd, nullptr, L"board", nullptr)};
            if (hBoard) MoveWindow(hBoard, (w - e) / 2, (h - e) / 2, e, e, true);
            break;
        }
        case WM_KEYUP:{
            if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDM_FILE_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    break;
            }
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

    std::cout << "Registering window classes" << std::endl;
    const WNDCLASSEXW reversiWndClass {
            sizeof(WNDCLASSEXW),
            CS_HREDRAW | CS_VREDRAW,
            ReversiWindowProcW,
            0,
            sizeof(void *),
            hInstance,
            nullptr,
            LoadCursor(nullptr, IDC_ARROW),
            static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)),
            MAKEINTRESOURCEW(IDR_REVERSI_MENU),
            L"reversi",
            nullptr,
    };
    if (!RegisterClassExW(&reversiWndClass)) return 0;
    const WNDCLASSEXW boardWndClass {
        sizeof(WNDCLASSEXW),
        0,
        BoardWindowProcW,
        0,
        sizeof(void *),
        hInstance,
        nullptr,
        LoadCursor(nullptr, IDC_HAND),
        nullptr,
        nullptr,
        L"board",
        nullptr,
    };
    if (!RegisterClassExW(&boardWndClass)) return 0;

    std::cout << "Creating window" << std::endl;
    Game game {10};
    auto moves {game.FindPossibleMoves(TOKEN_BLACK)};
    for (auto offset: moves){
        auto p {game.GetPoint(offset)};
        game.SetToken(p, TOKEN_TEST);
    }
    std::cout << game << std::endl;
    auto hWnd = CreateWindowW(
            L"reversi",
            L"Reversi",
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            nullptr,
            nullptr,
            hInstance,
            &game);
    if (!hWnd) return 0;

    std::cout << "Showing window" << std::endl;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    std::cout << "Unregistering window classes" << std::endl;
    UnregisterClassW(boardWndClass.lpszClassName, hInstance);
    UnregisterClassW(reversiWndClass.lpszClassName, hInstance);
    return static_cast<int>(msg.wParam);
}
