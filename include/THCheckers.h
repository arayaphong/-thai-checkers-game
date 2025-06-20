#ifndef THCHECKERS_H
#define THCHECKERS_H

#include "Board.h"
#include <vector>  // for possible moves return type
#include "Piece.h"  // for Position
#include <string>

class THCheckers {
public:
    THCheckers();
    // Construct with custom player names
    THCheckers(const std::string& p1, const std::string& p2);
    void startGame();
    void displayBoard() const;
    std::vector<Position> getPiecesCanMove() const;
    std::vector<std::vector<Position>> getPossibleMoves(const Piece& piece) const;
    std::vector<std::vector<Position>> getPossibleMoves(const Position& pos) const;
    void makeMove();
    bool checkWinCondition() const;

private:
    Board board;
    bool gameOver;
    std::string player1Name;
    std::string player2Name;
};

#endif // THCHECKERS_H