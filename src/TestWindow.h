//
// Created by jpcuv on 27-12-23.
//

#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H
#include <string>

#include "Window.h"


class TestWindow final: public Window {
    std::string info_ {"Reversi"};
public:
    static inline const auto CLASS_NAME = L"TestWindow";
    TestWindow(HWND parent_handle, HMENU identifier);
    LRESULT wnd_proc(UINT message, WPARAM word_param, LPARAM long_param) override;
    void set_info(const std::string& s){ info_ = s; InvalidateRect(handle_, nullptr, true);}
};

/*
class TestWndClass {
public:
    explicit TestWndClass(HINSTANCE instance_handle);
    ~TestWndClass();
    static HWND AddWindow(HWND parent_window_handle, HMENU identifier);
    static void RemoveWindow(HWND window_handle){ windows_.erase(window_handle);}
    static TestWindow& GetWindow(HWND window_handle){ return windows_[window_handle];}
private:
    static inline LPCWSTR class_name_ {L"test"};
    static LRESULT WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam);
    inline static std::unordered_map<HWND, TestWindow> windows_;
    HINSTANCE instance_handle_;
};
*/



#endif //TEST_WINDOW_H
