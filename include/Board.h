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
    std::vector<std::vector<Position>> getPossibleMoves(const Piece& piece) const;
    bool isValidMove(int startX, int startY, int endX, int endY) const;
    std::vector<Position> getPiecesCanMove(const std::string& playerName) const;
    Piece* getPieceAt(const Position& pos) const;
    std::string getCurrentPlayer() const;

private:
    std::vector<std::vector<Piece*>> grid;
    // Store player names for piece creation and display logic
    std::string player1Name;
    std::string player2Name;
    std::string currentPlayer;
};

#endif // BOARD_H