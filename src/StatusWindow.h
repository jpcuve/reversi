//
// Created by jpcuv on 27-12-23.
//

#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H
#include <string>

#include "Game.h"
#include "Window.h"


class StatusWindow final: public Window {
    Game& game_;
    std::string info_ {"Reversi"};
public:
    static inline const auto CLASS_NAME = L"TestWindow";
    StatusWindow(HWND parent_handle, HMENU identifier, Game& game);
    LRESULT wnd_proc(UINT message, WPARAM word_param, LPARAM long_param) override;
    void set_info(const std::string& s){ info_ = s; InvalidateRect(handle_, nullptr, true);}
};

#endif //TEST_WINDOW_H
