//
// Created by jpc on 16-12-23.
//

#ifndef GAME_H
#define GAME_H
#include <string>
#include <set>

#define TOKEN_EMPTY ' '
#define TOKEN_WHITE 'O'
#define TOKEN_BLACK 'X'
#define TOKEN_TEST 'T'


class Game {
public:
    Game(long size);
    long GetSize() const { return size; }
    long GetOffset(POINT p) const { return size * p.y + p.x;}
    POINT GetPoint(long offset) const {return {offset % size, offset / size};}
    void SetToken(POINT p, char token){ board[GetOffset(p)] = token;}
    char GetToken(POINT p){ return board[GetOffset(p)];}
    std::set<long> FindPossibleMoves(char token);
private:
    long size;
    std::string board;
};



#endif //GAME_H
