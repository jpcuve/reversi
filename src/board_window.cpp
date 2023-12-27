//
// Created by jpcuv on 27-12-23.
//

#include "board_window.h"

#include <stdexcept>


BoardWndClass::BoardWndClass(HINSTANCE instance_handle): instance_handle_(instance_handle) {
    const WNDCLASSEXW wndClass {
        sizeof(WNDCLASSEXW),
        0,
        WindowProc,
        0,
        0,
        instance_handle,
        nullptr,
        LoadCursor(nullptr, IDC_HAND),
        nullptr,
        nullptr,
        class_name_,
        nullptr,
    };
    if (!RegisterClassExW(&wndClass)) throw std::runtime_error("Cannot register window class");
}

BoardWndClass::~BoardWndClass() {
    UnregisterClassW(class_name_, instance_handle_);
}

HWND BoardWndClass::AddWindow(HWND parent_window_handle, HMENU identifier, Game* game) {
    const auto hwnd = CreateWindowW(
        BoardWndClass::class_name_,
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
    const BoardWindow board_window {hwnd, game};
    windows_[hwnd] = board_window;
    return hwnd;
}

LRESULT BoardWndClass::WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam) {
    static Position mouse{-1, -1};
    auto& window = windows_[window_handle];
    auto game = window.GetGame();
    switch(message) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            const auto hdc {BeginPaint(window_handle, &ps)};
            const auto hGreenBrush {CreateSolidBrush(RGB(0, 0x80, 0))};
            RECT r;
            GetClientRect(window_handle, &r);
            const auto w {static_cast<int>(r.right - r.left)};
            const auto h {static_cast<int>(r.bottom - r.top)};
            const auto e {min(w, h)};
            const Position delta {(w - e) / 2, (h - e) / 2};
            const auto s {e / game->GetSize()};
            const auto d {s / 10};
            for (int row {0}; row < game->GetSize(); row++) {
                for (int col {0}; col < game->GetSize(); col++) {
                    const Position p {delta.x + s * col, delta.y + s * row};
                    SelectObject(hdc, hGreenBrush);
                    Rectangle(hdc, p.x, p.y, p.x + s, p.y + s);
                    if (mouse == p / s && !game->GetToken({col, row}) && !mouse.IsNegative()) {
                        for (auto& follower: game->GetFollowers()) {
                            if (follower.GetToken({row, col})) {
                                SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
                                SelectObject(hdc, GetStockObject(WHITE_PEN));
                                Ellipse(hdc, p.x + d, p.y + d, p.x + s - d, p.y + s - d);
                                SelectObject(hdc, GetStockObject(BLACK_PEN));
                                break;
                            }
                        }
                    }
                    if (const auto token {game->GetToken({col, row})}; token){
                        SelectObject(hdc, GetStockObject(token == TOKEN_WHITE ? WHITE_BRUSH : BLACK_BRUSH));
                        Ellipse(hdc, p.x + d, p.y + d, p.x + s - d, p.y + s - d);
                    }
                }
            }
            DeleteObject(hGreenBrush);
            EndPaint(window_handle, &ps);
            break;
        }
        case WM_MOUSEMOVE: {
            if (mouse.IsNegative()) {
                TRACKMOUSEEVENT event {
                    sizeof(TRACKMOUSEEVENT),
                    TME_LEAVE,
                    window_handle,
                    0,
                };
                TrackMouseEvent(&event);
            }
            RECT r;
            GetClientRect(window_handle, &r);
            const auto edge {static_cast<int>(r.right) / game->GetSize()};
            Position m {LOWORD(lParam), HIWORD(lParam)};
            m = m / edge;
            r.left = min(mouse.x, m.x) * edge;
            r.right = (max(mouse.x, m.x) + 1) * edge;
            r.top = min(mouse.y, m.y) * edge;
            r.bottom = (max(mouse.y, m.y) + 1) * edge;
            InvalidateRect(window_handle, &r, true);
            mouse = m;
            break;
        }
        case WM_MOUSELEAVE:
            InvalidateRect(window_handle, nullptr, true);
            mouse = {-1, -1};
        break;
        default:
            break;
    }
    return DefWindowProcW(window_handle, message, wParam, lParam);
}
