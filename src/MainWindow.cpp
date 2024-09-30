//
// Created by jpcuv on 27-12-23.
//

#include "MainWindow.h"

#include <iostream>

#include "resource.h"

#include "BoardWindow.h"
#include "main.h"
#include "TestWindow.h"

MainWindow::MainWindow(HINSTANCE hinstance, Game& game): game_(game) {
    handle_ = CreateWindowW(
        CLASS_NAME,
        L"Reversi",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        hinstance,
        nullptr
    );
    ThrowIfNull(handle_);
    SetWindowLongPtr(handle_, 0, reinterpret_cast<LONG_PTR>(this));
    board_window_ = std::make_unique<BoardWindow>(handle_, reinterpret_cast<HMENU>(ID_BOARD_WINDOW), game);
    ShowWindow(board_window_->handle(), SW_SHOWNORMAL);
    test_window_ = std::make_unique<TestWindow>(handle_, reinterpret_cast<HMENU>(ID_TEST_WINDOW));
    ShowWindow(test_window_->handle(), SW_SHOWNORMAL);
}

LRESULT MainWindow::wnd_proc(const UINT message, const WPARAM word_param, const LPARAM long_param) {
    switch(message) {
        case WM_SIZE: {
                const SIZE size {LOWORD(long_param), HIWORD(long_param)};
                const int ratio {20};
                const auto e {min(size.cx, size.cy) * (ratio - 1) / ratio};
                MoveWindow(GetDlgItem(handle_, ID_BOARD_WINDOW), (size.cx - e) / 2, 0, e, e, true);
                MoveWindow(GetDlgItem(handle_, ID_TEST_WINDOW), 0, size.cy * (ratio - 1) / ratio, size.cx, size.cy / ratio, true);
                break;
        }
        case WM_KEYUP:{
                if (word_param == VK_ESCAPE) DestroyWindow(handle_);
                break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
            switch(LOWORD(word_param)) {
                case IDM_FILE_EXIT:
                    DestroyWindow(handle_);
                        break;
                case IDM_FILE_TEST: {
                        test_window_->set_info("This seems to work");
                        break;
                }
                default:
                    break;
            }
            break;
        case WM_DPICHANGED: {
                const auto dpiX {LOWORD(word_param)};
                const auto dpiY {HIWORD(word_param)};
                std::cout << "Dpi (" << dpiX << ", " << dpiY << ")" << std::endl;
                const auto size {reinterpret_cast<RECT*>(long_param)};
                SetWindowPos(handle_, nullptr, size->left, size->top, size->right - size->left, size->bottom - size->top, 0);
                break;
        }
        default:
            break;
    }
    return Window::wnd_proc(message, word_param, long_param);
}


/*
MainWndClass::MainWndClass(HINSTANCE instance_handle): instance_handle_(instance_handle) {
    const WNDCLASSEXW wndClass {
        sizeof(WNDCLASSEXW),
        CS_HREDRAW | CS_VREDRAW,
        WindowProc,
        0,
        0,
        instance_handle,
        nullptr,
        LoadCursor(nullptr, IDC_ARROW),
        static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH)),
        MAKEINTRESOURCEW(IDR_REVERSI_MENU),
        class_name_,
        nullptr,
    };
    if (!RegisterClassExW(&wndClass)) throw std::runtime_error("Cannot register window class");
}

MainWndClass::~MainWndClass() {
    UnregisterClassW(class_name_, instance_handle_);
}

HWND MainWndClass::AddWindow(HINSTANCE instance_handle, Game* game) {
    const auto hwnd = CreateWindowW(
        MainWndClass::class_name_,
        L"Reversi",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        instance_handle,
        nullptr
    );
    if (!hwnd) throw std::runtime_error("Cannot create window");
    const MainWindow main_window {hwnd, game};
    windows_[hwnd] = main_window;
    const auto board_window_handle = BoardWndClass::AddWindow(hwnd, reinterpret_cast<HMENU>(ID_BOARD_WINDOW), game);
    if (!board_window_handle) throw std::runtime_error("Cannot create board window");
    ShowWindow(board_window_handle, SW_SHOWNORMAL);
    const auto test_window_handle = TestWndClass::AddWindow(hwnd, reinterpret_cast<HMENU>(ID_TEST_WINDOW));
    if (!test_window_handle) throw std::runtime_error("Cannot create test window");
    ShowWindow(test_window_handle, SW_SHOWNORMAL);

    return hwnd;
}

LRESULT MainWndClass::WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        case WM_SIZE: {
            const SIZE size {LOWORD(lParam), HIWORD(lParam)};
            const int ratio {20};
            const auto e {min(size.cx, size.cy) * (ratio - 1) / ratio};
            MoveWindow(GetDlgItem(window_handle, ID_BOARD_WINDOW), (size.cx - e) / 2, 0, e, e, true);
            MoveWindow(GetDlgItem(window_handle, ID_TEST_WINDOW), 0, size.cy * (ratio - 1) / ratio, size.cx, size.cy / ratio, true);
            break;
        }
        case WM_KEYUP:{
            if (wParam == VK_ESCAPE) DestroyWindow(window_handle);
            break;
        }
        case WM_DESTROY:
            RemoveWindow(window_handle);
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
*/
