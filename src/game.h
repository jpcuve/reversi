//
// Created by jpc on 16-12-23.
//

#ifndef GAME_H
#define GAME_H
#include <vector>


class Game {
public:
    Game(int size);
    int GetSize() const { return size; }
    void SetToken(POINT p, char token){ board[size * p.y + p.x] = token;}
    char GetToken(POINT p){ return board[size * p.y + p.x];}
private:
    int size;
    std::vector<char> board;
};



#endif //GAME_H
