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
public:
    explicit Game(int size = 8);
    void Clear(){ board_ = std::string(size_ * size_, 0);}
    void Initialize();
    [[nodiscard]] int GetSize() const { return size_; }
    [[nodiscard]] size_t GetOffset(const Position& p) const { return size_ * p.y + p.x;}
    [[nodiscard]] Position GetPosition(const size_t offset) const { return {static_cast<int>(offset % size_), static_cast<int>(offset / size_)};}
    void SetToken(const Position& p, const char token){ board_[GetOffset(p)] = token;}
    [[nodiscard]] char GetToken(const Position& p) const { return board_[GetOffset(p)];}
    void SetInfo(const std::string& info){ info_ = info; }
    [[nodiscard]] std::string& GetInfo() { return info_;}
    void ComputeFollowers(char token);
    [[nodiscard]] std::vector<Game>& GetFollowers() { return followers_;}
    friend std::ostream& operator<<(std::ostream& os, const Game& that);
private:
    int size_;
    std::string board_;
    std::vector<Game> followers_;
    std::string info_ {"text"};
    [[nodiscard]] bool IsValid(const Position& p) const { return p.IsValid(size_);}
};



#endif //GAME_H
