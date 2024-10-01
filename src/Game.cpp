//
// Created by jpc on 16-12-23.
//

#include <iostream>
#include "Game.h"

Game::Game(const int size): size_(size), board_(size * size, 0) {
    initialize();
}

void Game::initialize() {
    set_token({size_ / 2 - 1, size_ / 2 - 1}, TOKEN_WHITE);
    set_token({size_ / 2, size_ / 2}, TOKEN_WHITE);
    set_token({size_ / 2 - 1, size_ / 2}, TOKEN_BLACK);
    set_token({size_ / 2, size_ / 2 - 1}, TOKEN_BLACK);
}

void Game::compute_followers(const char t) {
    const auto oppositeToken = t == TOKEN_WHITE ? TOKEN_BLACK : TOKEN_WHITE;
    followers_.clear();
    for (int row {0}; row < size_; row++) for (int col {0}; col < size_; col++){
        if (const Position p{col, row}; get_token(p) == oppositeToken){
            for (int deltaRow {-1}; deltaRow <= 1; deltaRow++) for (int deltaCol {-1}; deltaCol <= 1; deltaCol++) {
                const Position delta {deltaCol, deltaRow};
                if (const Position consideredMove {p - delta}; !delta.IsZero() && IsValid(consideredMove) && !get_token(consideredMove)) {
                    auto nextGame = Game(*this);
                    for (Position cur {p + delta}; IsValid(cur); cur += delta) {
                        const auto currentToken =  get_token(cur);
                        if (!currentToken) break;
                        if (currentToken == t) {
                            nextGame.set_token(consideredMove, t);
                            nextGame.set_token(p, t);
                            followers_.push_back(nextGame);
                            break;
                        }
                        nextGame.set_token(cur, t);
                    }
                }
            }
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Game& that) {
    const std::string horizontal (that.size_, '-');
    os << std::endl << '+' << horizontal << '+' << std::endl;
    for (auto row {0}; row < that.size_; row++){
        os << '|';
        for (auto col {0}; col < that.size_; col++) {
            switch(that.get_token({col, row})) {
                case TOKEN_BLACK:
                    std::cout << 'X';
                break;
                case TOKEN_WHITE:
                    std::cout << 'O';
                break;
                default:
                    std::cout << '.';
                break;
            }
        }
        os  << '|' << std::endl;
    }
    os << '+' << horizontal << '+' << std::endl;
    return os;
}

