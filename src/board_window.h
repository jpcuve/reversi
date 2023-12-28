//
// Created by jpcuv on 27-12-23.
//

#ifndef BOARD_WINDOW_H
#define BOARD_WINDOW_H
#include <unordered_map>

#include "Game.h"


class BoardWindow {
public:
    BoardWindow(HWND hwnd, Game* game): window_handle_(hwnd), game_(game){}
    BoardWindow(): window_handle_(nullptr), game_(nullptr){}
    [[nodiscard]] HWND GetHandle() const { return window_handle_;}
    [[nodiscard]] Game* GetGame() const { return game_;}
    void SetEdge(const int edge){ edge_ = edge;}
    [[nodiscard]] int GetEdge() const { return edge_;}
    void SetOffset(const POINT p){ offset_ = p;}
    [[nodiscard]] POINT GetOffset() const { return offset_;}
    void SetMouseTracked(const bool tracked){ mouse_tracked_ = tracked;}
    [[nodiscard]] bool IsMouseTracked() const { return mouse_tracked_;}
    void SetMousePosition(const POINT p){ mouse_position_ = p;}
    [[nodiscard]] POINT GetMousePosition() const { return mouse_position_;}
    RECT ConvertToWindow(Position p, const Game* game) const;
    Position ConvertToGame(POINT p, const Game* game) const;
private:
    HWND window_handle_;
    Game* game_;
    int edge_ {0};
    POINT offset_ {0, 0};
    bool mouse_tracked_ {false};
    POINT mouse_position_ {0, 0};
};

class BoardWndClass {
public:
    explicit BoardWndClass(HINSTANCE instance_handle);
    ~BoardWndClass();
    static HWND AddWindow(HWND parent_window_handle, HMENU identifier, Game* game);
    static void RemoveWindow(HWND window_handle){ windows_.erase(window_handle);}
    static BoardWindow& GetWindow(HWND window_handle){ return windows_[window_handle];}
private:
    static inline LPCWSTR class_name_ {L"board"};
    static LRESULT WindowProc(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam);
    inline static std::unordered_map<HWND, BoardWindow> windows_;
    HINSTANCE instance_handle_;
};



#endif //BOARD_WINDOW_H
