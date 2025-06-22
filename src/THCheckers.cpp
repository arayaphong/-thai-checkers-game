#include "THCheckers.h"
#include <iostream>

// Default constructor delegates to custom constructor with default names
THCheckers::THCheckers()
    : THCheckers("Player1", "Player2") {}

// Construct with custom player names
THCheckers::THCheckers(const std::string& p1, const std::string& p2)
    : board(), player1Name(p1), player2Name(p2), gameOver(false) {}

void THCheckers::startGame() {
    // Initialize board with configured player names
    board.initialize(player1Name, player2Name);
}

void THCheckers::displayBoard() const {
    board.display();
}

void THCheckers::makeMove() {
    // No-op stub move
}

bool THCheckers::checkWinCondition() const {
    return true; // Immediately end game
}

std::vector<Piece> THCheckers::getMoveablePieces() const {
    // Get all pieces that can move for the current player
    return board.getMoveablePieces(board.getCurrentPlayer());
}

// Get possible move sequences for piece at given position, delegate to Board
std::vector<std::vector<Position>> THCheckers::getTargetPositions(const Piece& piece) const {
    return board.getTargetPositions(piece);
}
