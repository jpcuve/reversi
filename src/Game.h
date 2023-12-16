//
// Created by jpc on 16-12-23.
//

#ifndef GAME_H
#define GAME_H
#include <vector>


class Game {
public:
    Game();
    int GetSize() const { return size; }
    void SetToken(int col, int row, char token){ board[size * row + col] = token;}
    char GetToken(int col, int row){ return board[size * row + col];}
private:
    int size {8};
    std::vector<char> board;
};



#endif //GAME_H
