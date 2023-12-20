//
// Created by jpc on 16-12-23.
//

#ifndef GAME_H
#define GAME_H
#include <string>
#include <set>

#include "Position.h"
#include "resource.h"

#define TOKEN_EMPTY '.'
#define TOKEN_WHITE 'O'
#define TOKEN_BLACK 'X'
#define TOKEN_TEST 'T'

class Game {
public:
    explicit Game(int size);
    void Clear(){ board_ = std::string(size_ * size_, TOKEN_EMPTY);}
    void Initialize();
    [[nodiscard]] int GetSize() const { return size_; }
    [[nodiscard]] size_t GetOffset(const Position& p) const { return size_ * p.y + p.x;}
    [[nodiscard]] Position GetPosition(const size_t offset) const { return {static_cast<int>(offset % size_), static_cast<int>(offset / size_)};}
    void SetToken(const Position& p, const char token){ board_[GetOffset(p)] = token;}
    [[nodiscard]] char GetToken(const Position& p) const { return board_[GetOffset(p)];}
    void SetInfo(const std::string info){ info_ = info; Notify(WM_INFO_CHANGED, 0, 0);}
    [[nodiscard]] std::string GetInfo() const { return info_;}
    void AddListener(HWND window_handle){ window_handles.insert(window_handle);}
    void RemoveListener(HWND window_handle){ window_handles.erase(window_handle);}
    [[nodiscard]] std::set<size_t> FindPossibleMoves(char token) const;
    friend std::ostream& operator<<(std::ostream& os, const Game& that);
private:
    int size_;
    std::string board_;
    std::string info_ {"text"};
    std::set<HWND> window_handles;
    [[nodiscard]] bool IsValid(const Position& p) const { return p.IsValid(size_);}
    void Notify(UINT message, WPARAM wParam, LPARAM lParam) const;
};



#endif //GAME_H
