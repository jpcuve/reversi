//
// Created by jpcuv on 20-12-23.
//

#ifndef MESSAGEWNDCLASS_H
#define MESSAGEWNDCLASS_H



class MessageWndClass {
public:
    explicit MessageWndClass(HINSTANCE instance_handle);
    ~MessageWndClass();
    static inline LPCWSTR class_name_ {L"message"};
private:
    static LRESULT WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam);
    HINSTANCE instance_handle_;
};



#endif //MESSAGEWNDCLASS_H
