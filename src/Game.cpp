//
// Created by jpc on 16-12-23.
//

#include <iostream>
#include "Game.h"

#include <algorithm>
#include <sstream>

#include "resource.h"

Game::Game(const int size): size_(size), board_(size * size, 0) {
}

void Game::initialize() {
    std::ranges::fill(board_, 0);
    for (auto row {0}; row < 2; row++) for (auto col {0}; col < 2; col++) set_token({size_ / 2 - row, size_ / 2 - col}, (row+ col) % 2 ? TOKEN_BLACK : TOKEN_WHITE);
}

void Game::play(const Position& p) {
    if (is_valid_move(p)) {
        set_token(p, player_);
        for (const auto& dir: DIRECTIONS) {
            const auto count = capture_count(p, dir);
            auto turn {p + dir};
            for (int i = 0; i < count; i++) {
                set_token(turn, player_);
                turn += dir;
            }
        }
        player_ = player_ == TOKEN_WHITE ? TOKEN_BLACK : TOKEN_WHITE;
    }
}

void Game::set_token(const Position& p, const char token) {
    board_[get_offset(p)] = token;
    std::ranges::for_each(listeners_, [&](auto& hwnd) {PostMessage(hwnd, WM_SET_TOKEN, static_cast<WPARAM>(token), static_cast<LPARAM>(get_offset(p)));});
}

bool Game::is_valid_move(const Position& p, const char token) const {
    return std::ranges::any_of(DIRECTIONS, [&](const auto& dir) { return capture_count(p, token, dir) > 0; });
}

int Game::capture_count(const Position& p, const char token, const Position& dir) const {
    int count = 0;
    for (auto considered_position {p + dir}; is_valid(considered_position); considered_position += dir) {
        const auto considered_token = get_token(considered_position);
        if (!considered_token) return 0;
        if (considered_token == token) {
            std::cout << "Count is: " << count << " for direction: " << dir << std::endl;
            return count;
        }
        count++;
    }
    return 0;
}

std::string& Game::get_status() {
    std::stringstream ss;
    ss << "w: " << std::ranges::count_if(board_, [&](const auto& token) { return token == TOKEN_WHITE; });
    ss << ", b: " << std::ranges::count_if(board_, [&](const auto& token) { return token == TOKEN_BLACK; });
    ss << ", " << (player_ == TOKEN_WHITE ? "white" : "black") << "'s turn";
    status_ = ss.str();
    return status_;
}

std::ostream &operator<<(std::ostream &os, const Game& that) {
    const std::string horizontal (that.size_ * 2 + 1, '-');
    os << std::endl << '+' << horizontal << '+' << std::endl;
    for (auto row {0}; row < that.size_; row++){
        os << "| ";
        for (auto col {0}; col < that.size_; col++) {
            const auto token = that.get_token({col, row});
            std::cout << (token == TOKEN_BLACK ? "X " : token == TOKEN_WHITE ? "O " : ". ");
        }
        os  << '|' << std::endl;
    }
    os << '+' << horizontal << '+' << std::endl;
    return os;
}

