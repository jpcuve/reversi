//
// Created by jpcuv on 27-12-23.
//

#include "BoardWindow.h"

#include "main.h"


BoardWindow::BoardWindow(const HWND parent_handle, const HMENU identifier, Game& game): game_(game) {
    handle_ = CreateWindowW(
        BoardWindow::CLASS_NAME,
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

LRESULT BoardWindow::wnd_proc(UINT message, WPARAM word_param, LPARAM long_param) {
    switch(message) {
        case WM_SIZE: {
            const SIZE size {LOWORD(long_param), HIWORD(long_param)};
            edge_ = {min(size.cx, size.cy)};
            offset_ = { (size.cx - edge_) / 2, (size.cy - edge_) / 2};
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            RECT dummy;
            const auto hdc {BeginPaint(handle_, &ps)};
            const auto hGreenBrush {CreateSolidBrush(RGB(0, 0x80, 0))};
            const auto tile_edge {edge_ / game_.GetSize()};
            const auto tile_padding{tile_edge / 10};
            for (int row {0}; row < game_.GetSize(); row++) for (int col {0}; col < game_.GetSize(); col++) {
                const auto r {convert_to_window({col, row}, game_)};
                if (IntersectRect(&dummy, &r, &ps.rcPaint)) {  // only draw if tile is in clipping region
                    SelectObject(hdc, hGreenBrush);
                    Rectangle(hdc, r.left, r.top, r.right, r.bottom);
                    if (mouse_tracked_ && PtInRect(&r, mouse_position_) && !game_.GetToken({col, row})) {
                        for (auto& follower: game_.GetFollowers()) {
                            if (follower.GetToken({row, col})) {
                                SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
                                SelectObject(hdc, GetStockObject(WHITE_PEN));
                                Ellipse(hdc, r.left + tile_padding, r.top + tile_padding, r.right - tile_padding, r.bottom - tile_padding);
                                SelectObject(hdc, GetStockObject(BLACK_PEN));
                            }
                        }
                    }
                    if (const auto token {game_.GetToken({col, row})}; token){
                        SelectObject(hdc, GetStockObject(token == TOKEN_WHITE ? WHITE_BRUSH : BLACK_BRUSH));
                        Ellipse(hdc, r.left + tile_padding, r.top + tile_padding, r.right - tile_padding, r.bottom - tile_padding);
                    }
                }
            }
            DeleteObject(hGreenBrush);
            EndPaint(handle_, &ps);
            return 0;
        }
        case WM_MOUSEMOVE: {
            if (!mouse_tracked_) {
                TRACKMOUSEEVENT event {
                    sizeof(TRACKMOUSEEVENT),
                    TME_LEAVE,
                    handle_,
                    0,
                };
                mouse_tracked_ = TrackMouseEvent(&event);
            }
            RECT r;
            r = convert_to_window(convert_to_game(mouse_position_, game_), game_);
            InvalidateRect(handle_, &r, true);
            mouse_position_ = {LOWORD(long_param), HIWORD(long_param)};
            r = convert_to_window(convert_to_game(mouse_position_, game_), game_);
            InvalidateRect(handle_, &r, true);
            break;
        }
        case WM_MOUSELEAVE: {
            mouse_tracked_ = false;
            InvalidateRect(handle_, nullptr, true);
            break;
        }
        default:
            break;
    }
    return Window::wnd_proc(message, word_param, long_param);
}

RECT BoardWindow::convert_to_window(const Position p, const Game& game) const {
    const auto tile_edge = edge_ / game.GetSize();
    const POINT lt {offset_.x + p.x * tile_edge, offset_.y + p.y * tile_edge};
    return {lt.x, lt.y, lt.x + tile_edge, lt.y + tile_edge};
}

Position BoardWindow::convert_to_game(const POINT p, const Game& game) const {
    const auto tile_edge = edge_ / game.GetSize();
    return {static_cast<int>(p.x - offset_.x) / tile_edge, static_cast<int>(p.y - offset_.y) / tile_edge };
}
