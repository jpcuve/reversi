//
// Created by jpcuv on 27-12-23.
//

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "Game.h"
#include "Window.h"


class BoardWindow;
class TestWindow;

class MainWindow final: public Window {
    Game& game_;
    std::unique_ptr<BoardWindow> board_window_;
    std::unique_ptr<TestWindow> test_window_;
public:
    static inline const auto CLASS_NAME = L"MainWindow";
    MainWindow(HINSTANCE hinstance, Game& game);
    LRESULT wnd_proc(UINT message, WPARAM word_param, LPARAM long_param) override;
};

#endif //MAIN_WINDOW_H
