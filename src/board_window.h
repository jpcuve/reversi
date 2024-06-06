//
// Created by jpcuv on 27-12-23.
//

#ifndef BOARD_WINDOW_H
#define BOARD_WINDOW_H
#include <unordered_map>

#include "Game.h"


class BoardWindow {
    HWND window_handle_ {nullptr};
    Game* game_ {nullptr};
public:
    BoardWindow() = default;
    BoardWindow(HWND hwnd, Game * game): window_handle_(hwnd), game_(game){}
    [[nodiscard]] Game * game() const { return game_; }
    int edge {0};
    POINT offset {0, 0};
    bool mouse_tracked {false};
    POINT mouse_position {0, 0};
    RECT ConvertToWindow(Position p, const Game* game) const;
    Position ConvertToGame(POINT p, const Game* game) const;
};

class BoardWndClass {
    static inline LPCWSTR class_name_ {L"board"};
    static LRESULT WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam);
    inline static std::unordered_map<HWND, BoardWindow> windows_;
    HINSTANCE instance_handle_;
public:
    explicit BoardWndClass(HINSTANCE instance_handle);
    ~BoardWndClass();
    static HWND AddWindow(HWND parent_window_handle, HMENU identifier, Game* game);
    static void RemoveWindow(HWND window_handle){ windows_.erase(window_handle);}
    static BoardWindow& GetWindow(HWND window_handle){ return windows_[window_handle];}
};



#endif //BOARD_WINDOW_H
