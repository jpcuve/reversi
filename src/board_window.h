//
// Created by jpcuv on 27-12-23.
//

#ifndef BOARD_WINDOW_H
#define BOARD_WINDOW_H

#include "Game.h"
#include "Window.h"


class BoardWindow final: public Window {
    Game* game_;
    int edge_ {0};
    POINT offset_ {0, 0};
    bool mouse_tracked_ {false};
    POINT mouse_position_ {0, 0};
    RECT convert_to_window(Position p, const Game* game) const;
    Position convert_to_game(POINT p, const Game* game) const;
public:
    BoardWindow(HWND parent_handle, HMENU identifier, Game * game);
    static inline const auto CLASS_NAME = L"BoardWindow";
    LRESULT wnd_proc(UINT message, WPARAM word_param, LPARAM long_param) override;
    // [[nodiscard]] Game * game() const { return game_; }
};

/*
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
*/



#endif //BOARD_WINDOW_H
