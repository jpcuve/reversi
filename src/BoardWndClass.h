//
// Created by jpcuv on 20-12-23.
//

#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include "pch.h"
class BoardWndClass {
public:
    explicit BoardWndClass(HINSTANCE instance_handle);
    ~BoardWndClass();
    static inline LPCWSTR class_name_ {L"board"};
private:
    static LRESULT WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam);
    HINSTANCE instance_handle_;
};




#endif //BOARDWINDOW_H
