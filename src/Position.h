//
// Created by jpcuv on 20-12-23.
//

#ifndef POSITION_H
#define POSITION_H
#include <ostream>


struct Position {
    Position(const int x, const int y): x{x}, y{y}{}
    Position operator+(const Position& p) const { return Position{x + p.x, y + p.y};}
    Position operator-(const Position& p) const { return Position{x - p.x, y - p.y};}
    Position& operator+=(const Position& p) {x += p.x; y += p.y; return *this;}
    [[nodiscard]] bool IsValid(const unsigned int size) const {return x >= 0 && y >= 0 && x < size && y < size;}
    [[nodiscard]] bool IsZero() const { return x == 0 && y == 0;}
    friend std::ostream& operator<<(std::ostream& os, const Position& that);
    int x {0};
    int y {0};
};



#endif //POSITION_H
