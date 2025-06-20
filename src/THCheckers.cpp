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

// Get possible moves for piece at given position, delegate to Board
std::vector<Position> THCheckers::getPossibleMoves(const Position& pos) const {
    return board.getPossibleMoves(pos);
}