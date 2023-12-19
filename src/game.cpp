//
// Created by jpc on 16-12-23.
//

#include <iostream>
#include "game.h"

Game::Game(const long size): size(size), board(size * size, TOKEN_EMPTY) {
    Initialize();
    SetToken({0, 0}, TOKEN_WHITE);
    SetToken({size - 1, size - 1}, TOKEN_BLACK);
}

void Game::Initialize() {
    SetToken({size / 2 - 1, size / 2 - 1}, TOKEN_WHITE);
    SetToken({size / 2, size / 2}, TOKEN_WHITE);
    SetToken({size / 2 - 1, size / 2}, TOKEN_BLACK);
    SetToken({size / 2, size / 2 - 1}, TOKEN_BLACK);
}

std::set<long> Game::FindPossibleMoves(const char token) const {
    std::set<long> moves;
    for (long row {0}; row < size; row++) for (long col {0}; col < size; col++){
        if (board[GetOffset({col, row})] == token){
            for (long j = max(row - 1, 0); j < min(row + 2, size); j++)
                for (long i = max(col - 1, 0); i < min(col + 2, size); i++)
                    if (const auto offset {GetOffset({i, j})}; board[offset] == TOKEN_EMPTY)
                        moves.insert(offset);
        }
    }
    return moves;
}

std::ostream &operator<<(std::ostream &os, const Game& that) {
    const std::string horizontal (that.size, '-');
    os << std::endl << '+' << horizontal << '+' << std::endl;
    for (auto row {0}; row < that.size; row++){
        os << '|' << that.board.substr(that.size * row, that.size) << '|' << std::endl;
    }
    os << '+' << horizontal << '+' << std::endl;
    return os;
}

