//
// Created by jpcuv on 30-09-24.
//

#include "Window.h"

LRESULT Window::WindowProc(HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
    LONG_PTR window_long_ptr = GetWindowLongPtr(hWnd, 0);
    if (window_long_ptr) {
        return ((Window*) window_long_ptr)->wnd_proc(message, wParam, lParam);
        /*
        const auto window = reinterpret_cast<Window *>(window_long_ptr);
        if (window) {
            return window->wnd_proc(message, wParam, lParam);
        }
    */
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

