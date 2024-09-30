//
// Created by jpcuv on 27-12-23.
//

#include "TestWindow.h"

#include <stdexcept>

#include "main.h"

/*
TestWndClass::TestWndClass(HINSTANCE instance_handle): instance_handle_(instance_handle) {
    const WNDCLASSEXW wndClass {
        sizeof(WNDCLASSEXW),
        0,
        WindowProc,
        0,
        0,
        instance_handle,
        nullptr,
        nullptr,
        static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
        nullptr,
        class_name_,
        nullptr,
    };
    if (!RegisterClassExW(&wndClass)) throw std::runtime_error("Cannot register window class");
}

TestWndClass::~TestWndClass() {
    UnregisterClassW(class_name_, instance_handle_);
}

HWND TestWndClass::AddWindow(HWND parent_window_handle, HMENU identifier) {
    const auto hwnd = CreateWindowW(
        class_name_,
        nullptr,
        WS_CHILD,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        parent_window_handle,
        identifier,  // identifier of child window
        nullptr,
        nullptr
    );
    if (!hwnd) throw std::runtime_error("Cannot create window");
    const TestWindow test_window {hwnd};
    windows_[hwnd] = test_window;
    return hwnd;
}

LRESULT TestWndClass::WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        case WM_DESTROY: {
            RemoveWindow(window_handle);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            const auto hdc {BeginPaint(window_handle, &ps)};
            RECT r;
            GetClientRect(window_handle, &r);
            const SIZE size {static_cast<int>(r.right - r.left), static_cast<int>(r.bottom - r.top)};
            MoveToEx(hdc, 0, 0, nullptr);
            LineTo(hdc, size.cx, 0);
            // auto font {static_cast<HFONT>(GetStockObject(SYSTEM_FONT))};
            auto font { CreateFont(
                size.cy - 1,
                0,
                0,
                0,
                FW_DONTCARE,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY,
                VARIABLE_PITCH,
                TEXT("Consolas"))};
            SelectObject(hdc, font);
            auto& window = GetWindow(window_handle);
            TextOut(hdc, 0, 1, window.GetInfo().c_str(), static_cast<int>(window.GetInfo().size()));
            DeleteObject(font);
            EndPaint(window_handle, &ps);
            return 0;
        }
        default:
            break;
    }
    return DefWindowProcW(window_handle, message, wParam, lParam);
}
*/

TestWindow::TestWindow(const HWND parent_handle, const HMENU identifier) {
    handle_ = CreateWindowW(
        CLASS_NAME,
        nullptr,
        WS_CHILD,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        parent_handle,
        identifier,  // identifier of child window
        nullptr,
        nullptr
    );
    ThrowIfNull(handle_);
    SetWindowLongPtr(handle_, 0, reinterpret_cast<LONG_PTR>(this));
}

LRESULT TestWindow::wnd_proc(const UINT message, const WPARAM word_param, const LPARAM long_param) {
    if (message == WM_PAINT) {
        PAINTSTRUCT ps;
        const auto hdc {BeginPaint(handle_, &ps)};
        RECT r;
        GetClientRect(handle_, &r);
        const SIZE size {static_cast<int>(r.right - r.left), static_cast<int>(r.bottom - r.top)};
        MoveToEx(hdc, 0, 0, nullptr);
        LineTo(hdc, size.cx, 0);
        // auto font {static_cast<HFONT>(GetStockObject(SYSTEM_FONT))};
        auto font { CreateFont(
            size.cy - 1,
            0,
            0,
            0,
            FW_DONTCARE,
            FALSE,
            FALSE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            VARIABLE_PITCH,
            TEXT("Consolas"))};
        SelectObject(hdc, font);
        TextOut(hdc, 0, 1, info_.c_str(), static_cast<int>(info_.size()));
        DeleteObject(font);
        EndPaint(handle_, &ps);
        return 0;
    }
    return Window::wnd_proc(message, word_param, long_param);
}
