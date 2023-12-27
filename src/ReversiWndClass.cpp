//
// Created by jpcuv on 20-12-23.
//

#include "ReversiWndClass.h"

#include <iostream>
#include <stdexcept>

#include "BoardWndClass.h"
#include "Game.h"
#include "MessageWndClass.h"
#include "resource.h"
#include "test_window.h"

#define ID_BOARD_WINDOW 1
#define ID_INFO_WINDOW 2
#define ID_TEST_WINDOW 3

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
    const auto game = reinterpret_cast<Game*>(GetWindowLongPtr(window_handle, 0));
    switch(message) {
        case WM_CREATE: {
            const auto pCreateStruct {reinterpret_cast<CREATESTRUCTW*>(lParam)};
            const auto g = static_cast<Game*>(pCreateStruct->lpCreateParams);
            SetWindowLongPtr(window_handle, 0, reinterpret_cast<LONG_PTR>(g));
            const auto board_window_handle {CreateWindowW(
                BoardWndClass::class_name_,
                nullptr,
                WS_CHILD,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                window_handle,
                reinterpret_cast<HMENU>(ID_BOARD_WINDOW),  // identifier of child window
                nullptr,
                pCreateStruct->lpCreateParams)};
            if (!board_window_handle) throw std::runtime_error("Cannot create board window");
//            g->AddListener(board_window_handle);
            ShowWindow(board_window_handle, SW_SHOWNORMAL);
            /*
            const auto message_window_handle {CreateWindowW(
                MessageWndClass::class_name_,
                nullptr,
                WS_CHILD,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                window_handle,
                reinterpret_cast<HMENU>(ID_INFO_WINDOW),  // identifier of child window
                nullptr,
                pCreateStruct->lpCreateParams)};
            if (!message_window_handle) throw std::runtime_error("Cannot create board window");
//            g->AddListener(message_window_handle);
            ShowWindow(message_window_handle, SW_SHOWNORMAL);
            */
            const auto test_window_handle = TestWndClass::AddWindow(window_handle, (HMENU) ID_TEST_WINDOW);
            if (!test_window_handle) throw std::runtime_error("Cannot create test window");
            ShowWindow(test_window_handle, SW_SHOWNORMAL);
            return 0;
        }
        case WM_SIZE: {
            const auto w {LOWORD(lParam)};
            const auto h {HIWORD(lParam)};
            const int ratio {20};
            const auto e {min(w, h) * (ratio - 1) / ratio};
            if (const auto hBoard {FindWindowExW(window_handle, nullptr, BoardWndClass::class_name_, nullptr)}; hBoard)
                MoveWindow(hBoard, (w - e) / 2, 0, e, e, true);
            /*
            if (const auto hMessage {FindWindowExW(window_handle, nullptr, MessageWndClass::class_name_, nullptr)}; hMessage)
                MoveWindow(hMessage, 0, h * (ratio - 1) / ratio, w, h / ratio, true);
            */
            MoveWindow(GetDlgItem(window_handle, 3), 0, h * (ratio - 1) / ratio, w, h / ratio, true);
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
                case IDM_FILE_TEST: {
                    auto& test_window = TestWndClass::GetWindow(GetDlgItem(window_handle, ID_TEST_WINDOW));
                    test_window.SetInfo("This seems to work");
                    break;
                }
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



