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
    void initialize(const std::string& player1Name, const std::string& player2Name);
    void display() const;
    std::vector<std::vector<Position>> getTargetPositions(const Piece& piece) const;
    std::vector<Piece> getMoveablePieces(const std::string& playerName) const;
    std::string getCurrentPlayer() const;
    
private:
    bool movePiece(int startX, int startY, int endX, int endY);
    bool isValidMove(int startX, int startY, int endX, int endY) const;
    std::vector<std::vector<Piece*>> grid;
    // Store player names for piece creation and display logic
    std::string player1Name;
    std::string player2Name;
    std::string currentPlayer;
};

#endif // BOARD_H