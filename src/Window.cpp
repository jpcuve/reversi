//
// Created by jpcuv on 30-09-24.
//

#include "Window.h"

LRESULT Window::WindowProc(HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
    if (const auto window_long_ptr {GetWindowLongPtr(hWnd, 0)}) {
        return reinterpret_cast<Window*>(window_long_ptr)->wnd_proc(message, wParam, lParam);
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

