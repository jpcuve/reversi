//
// Created by jpcuv on 27-12-23.
//

#include "BoardWindow.h"

#include "main.h"


BoardWindow::BoardWindow(const HWND parent_handle, const HMENU identifier, Game& game): game_(game) {
    handle_ = CreateWindowW(
        BoardWindow::CLASS_NAME,
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

/*
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

LRESULT BoardWndClass::WindowProc(HWND window_handle, const UINT message, const WPARAM wParam, const LPARAM lParam) {
    auto& board_window = windows_[window_handle];
    const auto game {board_window.game()};
    switch(message) {
        case WM_SIZE: {
            const SIZE size {LOWORD(lParam), HIWORD(lParam)};
            board_window.edge = {min(size.cx, size.cy)};
            board_window.offset = { (size.cx - board_window.edge) / 2, (size.cy - board_window.edge) / 2};
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            RECT dummy;
            const auto hdc {BeginPaint(window_handle, &ps)};
            const auto hGreenBrush {CreateSolidBrush(RGB(0, 0x80, 0))};
            const auto tile_edge {board_window.edge / game->GetSize()};
            const auto tile_padding{tile_edge / 10};
            for (int row {0}; row < game->GetSize(); row++) for (int col {0}; col < game->GetSize(); col++) {
                const auto r {board_window.ConvertToWindow({col, row}, game)};
                if (IntersectRect(&dummy, &r, &ps.rcPaint)) {  // only draw if tile is in clipping region
                    SelectObject(hdc, hGreenBrush);
                    Rectangle(hdc, r.left, r.top, r.right, r.bottom);
                    if (board_window.mouse_tracked && PtInRect(&r, board_window.mouse_position) && !game->GetToken({col, row})) {
                        for (auto& follower: game->GetFollowers()) {
                            if (follower.GetToken({row, col})) {
                                SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
                                SelectObject(hdc, GetStockObject(WHITE_PEN));
                                Ellipse(hdc, r.left + tile_padding, r.top + tile_padding, r.right - tile_padding, r.bottom - tile_padding);
                                SelectObject(hdc, GetStockObject(BLACK_PEN));
                            }
                        }
                    }
                    if (const auto token {game->GetToken({col, row})}; token){
                        SelectObject(hdc, GetStockObject(token == TOKEN_WHITE ? WHITE_BRUSH : BLACK_BRUSH));
                        Ellipse(hdc, r.left + tile_padding, r.top + tile_padding, r.right - tile_padding, r.bottom - tile_padding);
                    }
                }
            }
            DeleteObject(hGreenBrush);
            EndPaint(window_handle, &ps);
            return 0;
        }
        case WM_MOUSEMOVE: {
            if (!board_window.mouse_tracked) {
                TRACKMOUSEEVENT event {
                    sizeof(TRACKMOUSEEVENT),
                    TME_LEAVE,
                    window_handle,
                    0,
                };
                board_window.mouse_tracked = TrackMouseEvent(&event);
            }
            RECT r;
            r = board_window.ConvertToWindow(board_window.ConvertToGame(board_window.mouse_position, game), game);
            InvalidateRect(window_handle, &r, true);
            board_window.mouse_position = {LOWORD(lParam), HIWORD(lParam)};
            r = board_window.ConvertToWindow(board_window.ConvertToGame(board_window.mouse_position, game), game);
            InvalidateRect(window_handle, &r, true);
            break;
        }
        case WM_MOUSELEAVE: {
            board_window.mouse_tracked = false;
            InvalidateRect(window_handle, nullptr, true);
            break;
        }
        default:
            break;
    }
    return DefWindowProcW(window_handle, message, wParam, lParam);
}
*/
