//
// Created by jpcuv on 27-12-23.
//

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <unordered_map>

#include "Game.h"


class MainWindow {
    HWND window_handle_ {nullptr};
    Game* game_ {nullptr};
public:
    MainWindow() = default;
    explicit MainWindow(HWND hwnd, Game* game): window_handle_(hwnd), game_(game){}
};

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



#endif //MAIN_WINDOW_H
