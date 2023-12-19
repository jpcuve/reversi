//
// Created by jpc on 16-12-23.
//

#ifndef GAME_H
#define GAME_H
#include <string>
#include <set>

#define TOKEN_EMPTY '.'
#define TOKEN_WHITE 'O'
#define TOKEN_BLACK 'X'
#define TOKEN_TEST 'T'


class Game {
public:
    explicit Game(long size);
    void Clear(){ board = std::string(size * size, TOKEN_EMPTY);}
    void Initialize();
    [[nodiscard]] long GetSize() const { return size; }
    [[nodiscard]] long GetOffset(const POINT p) const { return size * p.y + p.x;}
    [[nodiscard]] POINT GetPoint(const long offset) const { return {offset % size, offset / size};}
    void SetToken(const POINT p, const char token){ board[GetOffset(p)] = token;}
    [[nodiscard]] char GetToken(const POINT p) const { return board[GetOffset(p)];}
    [[nodiscard]] std::set<long> FindPossibleMoves(char token) const;
    friend std::ostream& operator<<(std::ostream& os, const Game& that);
private:
    long size;
    std::string board;
};



#endif //GAME_H
