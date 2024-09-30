//
// Created by jpcuv on 27-12-23.
//

#ifndef BOARD_WINDOW_H
#define BOARD_WINDOW_H

#include "Game.h"
#include "Window.h"


class BoardWindow final: public Window {
    Game& game_;
    int edge_ {0};
    POINT offset_ {0, 0};
    bool mouse_tracked_ {false};
    POINT mouse_position_ {0, 0};
    RECT convert_to_window(Position p, const Game& game) const;
    Position convert_to_game(POINT p, const Game& game) const;
public:
    BoardWindow(HWND parent_handle, HMENU identifier, Game& game);
    static inline const auto CLASS_NAME = L"BoardWindow";
    LRESULT wnd_proc(UINT message, WPARAM word_param, LPARAM long_param) override;
};

#endif //BOARD_WINDOW_H
