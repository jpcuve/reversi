//
// Created by jpcuv on 27-12-23.
//

#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H
#include <string>
#include <unordered_map>


class TestWindow {
public:
    void SetInfo(const std::string& s){ info_ = s;}
    [[nodiscard]] std::string& GetInfo() { return info_;}
private:
    std::string info_;
};

class TestWndClass {
public:
    [[explicit]] TestWndClass(HINSTANCE instance_handle);
    ~TestWndClass();
    static HWND AddWindow(HWND parent_window_handle, HMENU identifier);
    static TestWindow& GetWindow(HWND window_handle){ return windows_[window_handle];}
private:
    static inline LPCWSTR class_name_ {L"test"};
    static LRESULT WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam);
    inline static std::pmr::unordered_map<HWND, TestWindow> windows_;
    HINSTANCE instance_handle_;
};



#endif //TEST_WINDOW_H
