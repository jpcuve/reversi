//
// Created by jpc on 16-12-23.
//

#include <iostream>
#include "game.h"

Game::Game(const int size): size(size), board(size * size, TOKEN_EMPTY) {
    Initialize();
}

void Game::Initialize() {
    SetToken({size / 2 - 1, size / 2 - 1}, TOKEN_WHITE);
    SetToken({size / 2, size / 2}, TOKEN_WHITE);
    SetToken({size / 2 - 1, size / 2}, TOKEN_BLACK);
    SetToken({size / 2, size / 2 - 1}, TOKEN_BLACK);
}

std::set<size_t> Game::FindPossibleMoves(const char token) const {
    std::set<size_t> moves;
    for (int row {0}; row < size; row++) for (int col {0}; col < size; col++){
        if (const Position p{col, row}; board[GetOffset(p)] == token){
            for (int deltaRow {-1}; deltaRow <= 1; deltaRow++)
                for (int deltaCol {-1}; deltaCol <= 1; deltaCol++) {
                    const Position delta {deltaCol, deltaRow};
                    if (const Position consideredMove {p - delta}; !delta.IsZero() && IsValid(consideredMove) && GetToken(consideredMove) == TOKEN_EMPTY) {
                        for (Position cur {p + delta}; IsValid(cur); cur += delta) {
                            const auto currentToken =  GetToken(cur);
                            if (currentToken == TOKEN_EMPTY) {
                                break;
                            }
                            if ((token == TOKEN_BLACK && currentToken == TOKEN_WHITE) || (token == TOKEN_WHITE && currentToken == TOKEN_BLACK)) {
                                moves.insert(GetOffset(consideredMove));
                                break;
                            }
                        }
                    }
                }
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

