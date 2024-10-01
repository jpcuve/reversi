//
// Created by jpc on 16-12-23.
//

#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>

#include "Position.h"

#define TOKEN_WHITE 'O'
#define TOKEN_BLACK 'X'

class Game {
    int size_;
    std::string board_;
    std::vector<Game> followers_;
    [[nodiscard]] bool IsValid(const Position& p) const { return p.IsValid(size_);}
public:
    explicit Game(int size = 8);
    void clear(){ board_ = std::string(size_ * size_, 0);}
    void initialize();
    [[nodiscard]] int size() const { return size_; }
    [[nodiscard]] size_t get_offset(const Position& p) const { return size_ * p.y + p.x;}
    [[nodiscard]] Position get_position(const size_t offset) const { return {static_cast<int>(offset % size_), static_cast<int>(offset / size_)};}
    void set_token(const Position& p, const char token){ board_[get_offset(p)] = token;}
    [[nodiscard]] char get_token(const Position& p) const { return board_[get_offset(p)];}
    void compute_followers(char token);
    [[nodiscard]] std::vector<Game>& followers() { return followers_;}
    friend std::ostream& operator<<(std::ostream& os, const Game& that);
};



#endif //GAME_H
