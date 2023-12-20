//
// Created by jpcuv on 20-12-23.
//

#include "BoardWndClass.h"

#include <stdexcept>

#include "Game.h"

BoardWndClass::BoardWndClass(HINSTANCE instance_handle): instance_handle_(instance_handle){
    const WNDCLASSEXW boardWndClass {
        sizeof(WNDCLASSEXW),
        0,
        WindowProc,
        0,
        sizeof(void *),
        instance_handle,
        nullptr,
        LoadCursor(nullptr, IDC_HAND),
        nullptr,
        nullptr,
        class_name_,
        nullptr,
    };
    if (!RegisterClassExW(&boardWndClass)) throw std::runtime_error("Cannot register window class");
}

BoardWndClass::~BoardWndClass() {
    UnregisterClassW(class_name_, instance_handle_);
}

LRESULT BoardWndClass::WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam) {
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
            const auto hGreenBrush {CreateSolidBrush(RGB(0, 0x80, 0))};
            const auto hRedBrush {CreateSolidBrush(RGB(0xFF, 0, 0))};
            RECT r;
            GetClientRect(window_handle, &r);
            const auto w {static_cast<int>(r.right - r.left)};
            const auto h {static_cast<int>(r.bottom - r.top)};
            const auto e {min(w, h)};
            const auto dx {(w - e) / 2};
            const auto dy {(h - e) / 2};
            const auto s {e / game->GetSize()};
            const auto d {s / 10};
            for (int row {0}; row < game->GetSize(); row++) {
                for (int col {0}; col < game->GetSize(); col++) {
                    const auto x {dx + s * col};
                    const auto y {dy + s * row};
                    SelectObject(hdc, hGreenBrush);
                    Rectangle(hdc, x, y, x + s, y + s);
                    if (const auto token {game->GetToken({col, row})}; token != TOKEN_EMPTY){
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
            EndPaint(window_handle, &ps);
            break;
        }
        default:
            break;
    }
    return DefWindowProcW(window_handle, message, wParam, lParam);
}

