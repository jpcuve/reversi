//
// Created by jpcuv on 30-09-24.
//

#ifndef WINDOW_H
#define WINDOW_H

class Window {
protected:
  HWND handle_ {nullptr};
public:
  static LRESULT WindowProcW(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  virtual ~Window() = default;
  virtual LRESULT wnd_proc(const UINT message, const WPARAM word_param, const LPARAM long_param) { return DefWindowProcW(handle_, message, word_param, long_param); }
  [[nodiscard]] HWND handle() const { return handle_; }
};



#endif //WINDOW_H
