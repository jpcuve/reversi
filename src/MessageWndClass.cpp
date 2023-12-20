//
// Created by jpcuv on 20-12-23.
//

#include "MessageWndClass.h"

#include <stdexcept>

#include "Game.h"

MessageWndClass::MessageWndClass(HINSTANCE instance_handle): instance_handle_(instance_handle){
    const WNDCLASSEXW boardWndClass {
        sizeof(WNDCLASSEXW),
        0,
        WindowProc,
        0,
        sizeof(void *),
        instance_handle,
        nullptr,
        LoadCursor(nullptr, IDC_HAND),
        static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)),
        nullptr,
        class_name_,
        nullptr,
    };
    if (!RegisterClassExW(&boardWndClass)) throw std::runtime_error("Cannot register window class");
}

MessageWndClass::~MessageWndClass() {
    UnregisterClassW(class_name_, instance_handle_);
}

LRESULT MessageWndClass::WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam) {
    const auto game = reinterpret_cast<Game*>(GetWindowLongPtr(window_handle, 0));
    switch(message) {
        case WM_CREATE: {
            const auto pCreateStruct {reinterpret_cast<CREATESTRUCTW*>(lParam)};
            SetWindowLongPtr(window_handle, 0, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            const auto hdc {BeginPaint(window_handle, &ps)};
            RECT r;
            GetClientRect(window_handle, &r);
            const auto w {static_cast<int>(r.right - r.left)};
            const auto h {static_cast<int>(r.bottom - r.top)};
            EndPaint(window_handle, &ps);
            break;
        }
        default:
            break;
    }
    return DefWindowProcW(window_handle, message, wParam, lParam);
}
