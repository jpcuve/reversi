//
// Created by jpc on 16-12-23.
//

#include <iostream>
#include "game.h"

Game::Game(long size): size(size), board(size * size, TOKEN_EMPTY) {
    SetToken({0, 0}, TOKEN_WHITE);
    SetToken({size - 1, size - 1}, TOKEN_BLACK);
    SetToken({size / 2 - 1, size / 2 - 1}, TOKEN_WHITE);
    SetToken({size / 2, size / 2}, TOKEN_WHITE);
    SetToken({size / 2 - 1, size / 2}, TOKEN_BLACK);
    SetToken({size / 2, size / 2 - 1}, TOKEN_BLACK);
}

std::set<long> Game::FindPossibleMoves(char token) {
    std::set<long> moves;
    for (long row {0}; row < size; row++) for (long col {0}; col < size; col++){
        if (board[GetOffset({col, row})] == token){
            for (long j = max(row - 1, 0); j < min(row + 2, size); j++) for (long i = max(col - 1, 0); i < min(col + 2, size); i++){
                auto offset {GetOffset({i, j})};
                if (board[offset] == TOKEN_EMPTY) moves.insert(offset);
            }
        }
    }
    std::cout << "Possible moves for '" << token << "':" << std::endl;
    for (auto offset: moves){
        auto p {GetPoint(offset)};
        SetToken(p, TOKEN_TEST);
        std::cout << " (" << p.x << ", " << p.y << ")" << std::endl;
    }
    return moves;
}
