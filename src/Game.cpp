//
// Created by jpc on 16-12-23.
//

#include <iostream>
#include "Game.h"

Game::Game(const int size): size_(size), board_(size * size, 0) {
    Initialize();
}

void Game::Initialize() {
    SetToken({size_ / 2 - 1, size_ / 2 - 1}, TOKEN_WHITE);
    SetToken({size_ / 2, size_ / 2}, TOKEN_WHITE);
    SetToken({size_ / 2 - 1, size_ / 2}, TOKEN_BLACK);
    SetToken({size_ / 2, size_ / 2 - 1}, TOKEN_BLACK);
    SetInfo("Board initialized");
}

std::set<size_t> Game::FindPossibleMoves(const char token) const {
    const auto oppositeToken = token == TOKEN_WHITE ? TOKEN_BLACK : TOKEN_WHITE;
    std::set<size_t> moves;
    for (int row {0}; row < size_; row++) for (int col {0}; col < size_; col++){
        if (const Position p{col, row}; GetToken(p) == token){
            for (int deltaRow {-1}; deltaRow <= 1; deltaRow++) for (int deltaCol {-1}; deltaCol <= 1; deltaCol++) {
                const Position delta {deltaCol, deltaRow};
                if (const Position consideredMove {p - delta}; !delta.IsZero() && IsValid(consideredMove) && !GetToken(consideredMove)) {
                    auto nextGame = Game(*this);
                    nextGame.SetToken(consideredMove, token);
                    for (Position cur {p + delta}; IsValid(cur); cur += delta) {
                        const auto currentToken =  GetToken(cur);
                        if (!currentToken) break;
                        if (currentToken == oppositeToken) {
                            moves.insert(GetOffset(consideredMove));
                            break;
                        }
                        nextGame.SetToken(cur, token);
                    }
                }
            }
        }
    }
    return moves;
}

std::ostream &operator<<(std::ostream &os, const Game& that) {
    const std::string horizontal (that.size_, '-');
    os << std::endl << '+' << horizontal << '+' << std::endl;
    for (auto row {0}; row < that.size_; row++){
        os << '|' << that.board_.substr(that.size_ * row, that.size_) << '|' << std::endl;
    }
    os << '+' << horizontal << '+' << std::endl;
    return os;
}

