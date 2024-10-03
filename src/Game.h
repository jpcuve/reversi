//
// Created by jpc on 16-12-23.
//

#ifndef GAME_H
#define GAME_H
#include <string>
#include <unordered_set>

#include "Position.h"

static constexpr char TOKEN_WHITE = 1;
static constexpr char TOKEN_BLACK = -1;

class Game {
    int size_;
    std::string board_;
    std::unordered_set<HWND> listeners_;
    [[nodiscard]] bool is_valid(const Position& p) const { return p.is_valid(size_);}
    char player_ {TOKEN_WHITE};
public:
    explicit Game(int size = 8);
    void initialize();
    void play(const Position& p);
    [[nodiscard]] int size() const { return size_; }
    [[nodiscard]] size_t get_offset(const Position& p) const { return size_ * p.y + p.x;}
    [[nodiscard]] Position get_position(const size_t offset) const { return {static_cast<int>(offset % size_), static_cast<int>(offset / size_)};}
    void set_token(const Position& p, char token);
    [[nodiscard]] char get_token(const Position& p) const { return board_[get_offset(p)];}
    void attach(HWND hwnd){ listeners_.insert(hwnd); }
    void detach(HWND hwnd){ listeners_.erase(hwnd); }
    [[nodiscard]] bool is_valid_move(const Position& p) const;
    [[nodiscard]] int capture_count(const Position& p, const Position& dir) const;
    friend std::ostream& operator<<(std::ostream& os, const Game& that);
};



#endif //GAME_H
