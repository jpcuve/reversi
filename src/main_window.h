//
// Created by jpcuv on 27-12-23.
//

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <unordered_map>

#include "Game.h"


class MainWindow {
public:
    explicit MainWindow(HWND hwnd, Game* game): window_handle_(hwnd), game_(game){}
    MainWindow(): window_handle_(nullptr){}
    [[nodiscard]] HWND GetHandle() const { return window_handle_;}
    [[nodiscard]] Game* GetGame() const { return game_;}
private:
    HWND window_handle_;
    Game* game_;
};

class MainWndClass {
public:
    explicit MainWndClass(HINSTANCE instance_handle);
    ~MainWndClass();
    static HWND AddWindow(HINSTANCE instance_handle, Game* game);
    static void RemoveWindow(HWND window_handle){ windows_.erase(window_handle);}
    static MainWindow& GetWindow(HWND window_handle){ return windows_[window_handle];}
private:
    static inline LPCWSTR class_name_ {L"main"};
    static LRESULT WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam);
    inline static std::unordered_map<HWND, MainWindow> windows_;
    HINSTANCE instance_handle_;
};



#endif //MAIN_WINDOW_H
