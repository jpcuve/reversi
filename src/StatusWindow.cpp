//
// Created by jpcuv on 27-12-23.
//

#include "StatusWindow.h"

#include "main.h"

StatusWindow::StatusWindow(const HWND parent_handle, const HMENU identifier) {
    handle_ = CreateWindowW(
        CLASS_NAME,
        nullptr,
        WS_CHILD | WS_VISIBLE,
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

LRESULT StatusWindow::wnd_proc(const UINT message, const WPARAM word_param, const LPARAM long_param) {
    if (message == WM_PAINT) {
        PAINTSTRUCT ps;
        const auto hdc {BeginPaint(handle_, &ps)};
        RECT r;
        GetClientRect(handle_, &r);
        const SIZE size {static_cast<int>(r.right - r.left), static_cast<int>(r.bottom - r.top)};
        MoveToEx(hdc, 0, 0, nullptr);
        LineTo(hdc, size.cx, 0);
        // auto font {static_cast<HFONT>(GetStockObject(SYSTEM_FONT))};
        const auto font { CreateFont(
            size.cy - 1,
            0,
            0,
            0,
            FW_BOLD,
            FALSE,
            FALSE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            VARIABLE_PITCH,
            TEXT("Arial"))};
        SelectObject(hdc, font);
        TextOut(hdc, 0, 1, info_.c_str(), static_cast<int>(info_.size()));
        DeleteObject(font);
        EndPaint(handle_, &ps);
        return 0;
    }
    return Window::wnd_proc(message, word_param, long_param);
}
