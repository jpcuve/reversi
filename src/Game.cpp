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

bool Game::is_valid_move(const Position& p) const {
    for (const auto& dir: DIRECTIONS) {
        if (capture_count(p, dir) > 0) return true;
    }
    return false;
}

int Game::capture_count(const Position& p, const Position& dir) const {
    int count = 0;
    for (auto considered_position {p + dir}; considered_position.is_valid(size_); considered_position += dir) {
        const auto considered_token = get_token(considered_position);
        if (!considered_token) return 0;
        if (considered_token == player_) {
            std::cout << "Count is: " << count << " for direction: " << dir << std::endl;
            return count;
        }
        count++;
    }
    return 0;
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

