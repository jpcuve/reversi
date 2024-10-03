//
// Created by jpcuv on 27-12-23.
//

#include "BoardWindow.h"

#include <iostream>

#include "main.h"
#include "resource.h"


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
        identifier, // identifier of child window
        nullptr,
        nullptr
    );
    ThrowIfNull(handle_);
    SetWindowLongPtr(handle_, 0, reinterpret_cast<LONG_PTR>(this));
    game.attach(handle_);
}

LRESULT BoardWindow::wnd_proc(UINT message, WPARAM word_param, LPARAM long_param) {
    switch (message) {
    case WM_SIZE:
        {
            const SIZE size{LOWORD(long_param), HIWORD(long_param)};
            edge_ = {min(size.cx, size.cy)};
            offset_ = {(size.cx - edge_) / 2, (size.cy - edge_) / 2};
            break;
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT dummy;
            const auto hdc{BeginPaint(handle_, &ps)};
            const auto hGreenBrush{CreateSolidBrush(RGB(0, 0x80, 0))};
            const auto tile_edge{edge_ / game_.size()};
            const auto tile_padding{tile_edge / 10};
            for (int row{0}; row < game_.size(); row++)
                for (int col{0}; col < game_.size(); col++) {
                    const Position position {col, row};
                    const auto r{convert_to_window(position)};
                    if (IntersectRect(&dummy, &r, &ps.rcPaint)) {
                        // only draw if tile is in clipping region
                        SelectObject(hdc, hGreenBrush);
                        Rectangle(hdc, r.left, r.top, r.right, r.bottom);
                        if (mouse_tracked_ && PtInRect(&r, mouse_position_) && game_.is_valid_move(position, TOKEN_WHITE)) {
                            SelectObject(hdc, GetStockObject(NULL_PEN));
                            SelectObject(hdc, GetStockObject(GRAY_BRUSH));
                            Ellipse(hdc, r.left + tile_padding, r.top + tile_padding, r.right - tile_padding,
                                    r.bottom - tile_padding);
                            SelectObject(hdc, GetStockObject(BLACK_PEN));
                        }
                        if (const auto token{game_.get_token(position)}; token) {
                            SelectObject(hdc, GetStockObject(token == TOKEN_WHITE ? WHITE_BRUSH : BLACK_BRUSH));
                            Ellipse(hdc, r.left + tile_padding, r.top + tile_padding, r.right - tile_padding,
                                    r.bottom - tile_padding);
                        }
                    }
                }
            DeleteObject(hGreenBrush);
            EndPaint(handle_, &ps);
            return 0;
        }
    case WM_MOUSEMOVE:
        {
            std::cout << "Mouse moved" << std::endl;
            if (!mouse_tracked_) {
                TRACKMOUSEEVENT event{
                    sizeof(TRACKMOUSEEVENT),
                    TME_LEAVE,
                    handle_,
                    0,
                };
                mouse_tracked_ = TrackMouseEvent(&event);
            }
            invalidate_position(convert_to_game(mouse_position_));
            mouse_position_ = {LOWORD(long_param), HIWORD(long_param)};
            invalidate_position(convert_to_game(mouse_position_));
            break;
        }
    case WM_MOUSELEAVE:
        {
            mouse_tracked_ = false;
            invalidate_position(convert_to_game(mouse_position_));
            break;
        }
    case WM_SET_TOKEN:
        {
            const auto token{static_cast<char>(word_param)};
            const auto position{game_.get_position(static_cast<size_t>(long_param))};
            std::cout << "Set token message received: " << token << " " << position << std::endl;
            invalidate_position(position);
        }
    default:
        break;
    }
    return Window::wnd_proc(message, word_param, long_param);
}

RECT BoardWindow::convert_to_window(const Position p) const {
    const auto tile_edge = edge_ / game_.size();
    const POINT lt{offset_.x + p.x * tile_edge, offset_.y + p.y * tile_edge};
    return {lt.x, lt.y, lt.x + tile_edge, lt.y + tile_edge};
}

Position BoardWindow::convert_to_game(const POINT p) const {
    const auto tile_edge = edge_ / game_.size();
    return {static_cast<int>(p.x - offset_.x) / tile_edge, static_cast<int>(p.y - offset_.y) / tile_edge};
}

void BoardWindow::invalidate_position(const Position& p) const {
    RECT r;
    r = convert_to_window(p);
    InvalidateRect(handle_, &r, true);
}
