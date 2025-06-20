#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "Piece.h"
#include "KingPiece.h"

class Board {
public:
    Board();
    ~Board();
    // Initialize board with default or custom player names
    void initialize();
    void initialize(const std::string& player1Name, const std::string& player2Name);
    void display() const;
    bool movePiece(int startX, int startY, int endX, int endY);
    // Get possible moves (normal and capture) for a piece at the given position
    std::vector<Position> getPossibleMoves(Position pos) const;
    bool isValidMove(int startX, int startY, int endX, int endY) const;

private:
    std::vector<std::vector<Piece*>> grid;
    // Store player names for piece creation and display logic
    std::string player1Name;
    std::string player2Name;
};

#endif // BOARD_H