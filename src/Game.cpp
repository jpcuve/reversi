//
// Created by jpc on 16-12-23.
//

#include "Game.h"

Game::Game() {
    for (int col {0}; col < size; col++)
        for (int row {0}; row < size; row++)
            board.push_back('_');
    SetToken({size / 2 - 1, size / 2 - 1}, 'O');
    SetToken({size / 2, size / 2}, 'O');
    SetToken({size / 2 - 1, size / 2}, 'X');
    SetToken({size / 2, size / 2 - 1}, 'X');
};
