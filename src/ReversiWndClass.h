//
// Created by jpcuv on 20-12-23.
//

#ifndef REVERSIWINDOW_H
#define REVERSIWINDOW_H

#include "pch.h"

class ReversiWndClass {
public:
    explicit ReversiWndClass(HINSTANCE instance_handle);
    ~ReversiWndClass();
    static inline LPCWSTR class_name_ {L"reversi"};
private:
    static LRESULT WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam);
    HINSTANCE instance_handle_;
};





#endif //REVERSIWINDOW_H
