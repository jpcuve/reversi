//
// Created by jpcuv on 20-12-23.
//

#include "ReversiWndClass.h"

#include <iostream>
#include <stdexcept>

#include "Game.h"
#include "resource.h"

ReversiWndClass::ReversiWndClass(HINSTANCE instance_handle): instance_handle_(instance_handle) {
    const WNDCLASSEXW reversiWndClass {
        sizeof(WNDCLASSEXW),
        CS_HREDRAW | CS_VREDRAW,
        WindowProc,
        0,
        sizeof(void *),
        instance_handle,
        nullptr,
        LoadCursor(nullptr, IDC_ARROW),
        static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)),
        MAKEINTRESOURCEW(IDR_REVERSI_MENU),
        class_name_,
        nullptr,
    };
    if (!RegisterClassExW(&reversiWndClass)) throw std::runtime_error("Cannot register window class");
}

ReversiWndClass::~ReversiWndClass() {
    UnregisterClassW(class_name_, instance_handle_);
}

LRESULT ReversiWndClass::WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        case WM_CREATE: {
            const auto pCreateStruct {reinterpret_cast<CREATESTRUCTW*>(lParam)};
            SetWindowLongPtr(window_handle, 0, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            const auto board_window_handle {CreateWindowW(
                L"board",
                nullptr,
                WS_CHILD,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                window_handle,
                reinterpret_cast<HMENU>(1),  // identifier of child window
                nullptr,
                pCreateStruct->lpCreateParams)};
            if (!board_window_handle) throw std::runtime_error("Cannot create board window");
            ShowWindow(board_window_handle, SW_SHOWNORMAL);
            return 0;
        }
        case WM_SIZE: {
            const auto w {LOWORD(lParam)};
            const auto h {HIWORD(lParam)};
            const auto e {min(w, h)};
            if (const auto hBoard {FindWindowExW(window_handle, nullptr, L"board", nullptr)}; hBoard)
                MoveWindow(hBoard, (w - e) / 2, (h - e) / 2, e, e, true);
            break;
        }
        case WM_KEYUP:{
            if (wParam == VK_ESCAPE) DestroyWindow(window_handle);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDM_FILE_EXIT:
                    DestroyWindow(window_handle);
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
            SetWindowPos(window_handle, nullptr, size->left, size->top, size->right - size->left, size->bottom - size->top, 0);
            break;
        }
        default:
            break;
    }
    return DefWindowProcW(window_handle, message, wParam, lParam);
}



