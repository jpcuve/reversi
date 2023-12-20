//
// Created by jpc on 16-12-23.
//

#ifndef GAME_H
#define GAME_H
#include <string>
#include <set>

#define TOKEN_EMPTY '.'
#define TOKEN_WHITE 'O'
#define TOKEN_BLACK 'X'
#define TOKEN_TEST 'T'

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

class Game {
public:
    explicit Game(int size);
    void Clear(){ board = std::string(size * size, TOKEN_EMPTY);}
    void Initialize();
    [[nodiscard]] int GetSize() const { return size; }
    [[nodiscard]] size_t GetOffset(const Position& p) const { return size * p.y + p.x;}
    [[nodiscard]] Position GetPosition(const size_t offset) const { return {static_cast<int>(offset % size), static_cast<int>(offset / size)};}
    void SetToken(const Position& p, const char token){ board[GetOffset(p)] = token;}
    [[nodiscard]] char GetToken(const Position& p) const { return board[GetOffset(p)];}
    [[nodiscard]] std::set<size_t> FindPossibleMoves(char token) const;
    friend std::ostream& operator<<(std::ostream& os, const Game& that);
private:
    int size;
    std::string board;
    [[nodiscard]] bool IsValid(const Position& p) const { return p.IsValid(size);}
};



#endif //GAME_H
