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

/*
class MainWndClass {
    static inline LPCWSTR class_name_ {L"main"};
    static LRESULT WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam);
    inline static std::unordered_map<HWND, MainWindow> windows_;
    HINSTANCE instance_handle_;
public:
    explicit MainWndClass(HINSTANCE instance_handle);
    ~MainWndClass();
    static HWND AddWindow(HINSTANCE instance_handle, Game* game);
    static void RemoveWindow(HWND window_handle){ windows_.erase(window_handle);}
    static MainWindow& GetWindow(HWND window_handle){ return windows_[window_handle];}
};
*/



#endif //MAIN_WINDOW_H
