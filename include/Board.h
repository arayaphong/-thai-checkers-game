#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "Piece.h"
#include "KingPiece.h"

class States;  // forward

class Board {
public:
    Board();
    ~Board();
    void initialize(const std::vector<std::vector<Piece*>>& grid);
    void initialize(const std::string &player1Name, const std::string &player2Name);
    void display() const;
    std::vector<std::vector<Position>> getChoices(const Piece& piece) const;
    // Gather all possible move paths (simple + capture) for a piece
    States getStates(const Piece& piece);
    std::vector<Piece> getMoveablePieces(const std::string& playerName) const;
    std::string getCurrentPlayer() const;

private:
    friend class States;  // allow States to access grid and private fields
    void clearBoard();
    std::vector<std::vector<Piece*>> grid;
    // Store player names for piece creation and display logic
    std::string player1Name;
    std::string player2Name;
    std::string currentPlayer;
    States* currentStates = nullptr;  // pointer to last States
};

// State container for move paths and board updates
class States {
public:
    // Return raw choice paths
    const std::vector<std::vector<Position>>& getChoices() const { return choices; }
    // Apply chosen path back to the board
    void selectMove(const std::vector<Position>& path);

private:
    friend class Board;
    Board* board = nullptr;
    const Piece* piece = nullptr;
    std::vector<std::vector<Position>> choices;
    bool hasCapture = false;
};

#endif // BOARD_H