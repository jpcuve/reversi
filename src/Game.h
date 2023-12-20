//
// Created by jpc on 16-12-23.
//

#ifndef GAME_H
#define GAME_H
#include <string>
#include <set>

#include "Position.h"

#define TOKEN_EMPTY '.'
#define TOKEN_WHITE 'O'
#define TOKEN_BLACK 'X'
#define TOKEN_TEST 'T'

class Game {
public:
    explicit Game(int size);
    void Clear(){ board = std::string(size * size, TOKEN_EMPTY);}
    void Initialize();
    [[nodiscard]] int GetSize() const { return size; }
    [[nodiscard]] size_t GetOffset(const Position& p) const { return size * p.y + p.x;}
    [[nodiscard]] Position GetPosition(const size_t offset) const { return {static_cast<int>(offset % size), static_cast<int>(offset / size)};}
    void SetToken(const Position& p, const char token){ board[GetOffset(p)] = token;}
    [[nodiscard]] char GetToken(const Position& p) const { return board[GetOffset(p)];}
    [[nodiscard]] std::set<size_t> FindPossibleMoves(char token) const;
    friend std::ostream& operator<<(std::ostream& os, const Game& that);
private:
    int size;
    std::string board;
    [[nodiscard]] bool IsValid(const Position& p) const { return p.IsValid(size);}
};



#endif //GAME_H
