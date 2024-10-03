//
// Created by jpc on 16-12-23.
//

#include <iostream>
#include "Game.h"

#include "resource.h"

Game::Game(const int size): size_(size), board_(size * size, 0) {
}

void Game::initialize() {
    for (int row {0}; row < size_; row++) for (int col {0}; col < size_; col++) set_token({col, row}, 0);
    set_token({size_ / 2 - 1, size_ / 2 - 1}, TOKEN_WHITE);
    set_token({size_ / 2, size_ / 2}, TOKEN_WHITE);
    set_token({size_ / 2 - 1, size_ / 2}, TOKEN_BLACK);
    set_token({size_ / 2, size_ / 2 - 1}, TOKEN_BLACK);
}

void Game::set_token(const Position& p, const char token) {
    board_[get_offset(p)] = token;
    for (const auto& hwnd: listeners_) PostMessage(hwnd, WM_SET_TOKEN, static_cast<WPARAM>(token), static_cast<LPARAM>(get_offset(p)));
}

bool Game::is_valid_move(const Position& p, const char token) const {
    for (const auto& dir: DIRECTIONS) {
        if (capture_count(p, token, dir) > 0) return true;
    }
    return false;
}

int Game::capture_count(const Position& p, const char token, const Position& dir) const {
    if (token != TOKEN_BLACK && token != TOKEN_WHITE) return 0;
    int count = 0;
    for (auto considered_position {p + dir}; considered_position.is_valid(size_); considered_position += dir) {
        std::cout << "Considering: " << considered_position << std::endl;
        if (get_token(considered_position) == token) return count;
        count++;
    }
    return 0;
}

void Game::compute_followers(const char t) {
    const auto oppositeToken = t == TOKEN_WHITE ? TOKEN_BLACK : TOKEN_WHITE;
    followers_.clear();
    for (int row {0}; row < size_; row++) for (int col {0}; col < size_; col++){
        if (const Position p{col, row}; get_token(p) == oppositeToken){
            for (int deltaRow {-1}; deltaRow <= 1; deltaRow++) for (int deltaCol {-1}; deltaCol <= 1; deltaCol++) {
                const Position delta {deltaCol, deltaRow};
                if (const Position consideredMove {p - delta}; !delta.is_zero() && is_valid(consideredMove) && !get_token(consideredMove)) {
                    auto nextGame = Game(*this);
                    for (Position cur {p + delta}; is_valid(cur); cur += delta) {
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
    const std::string horizontal (that.size_ * 2 + 1, '-');
    os << std::endl << '+' << horizontal << '+' << std::endl;
    for (auto row {0}; row < that.size_; row++){
        os << "| ";
        for (auto col {0}; col < that.size_; col++) {
            const auto token = that.get_token({col, row});
            std::cout << (token == TOKEN_BLACK ? "X " : (token == TOKEN_WHITE ? "O " : ". "));
        }
        os  << '|' << std::endl;
    }
    os << '+' << horizontal << '+' << std::endl;
    return os;
}

