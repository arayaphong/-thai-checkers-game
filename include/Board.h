#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <memory>
#include "Piece.h"

class States;  // forward

class Board {
public:
    Board();
    ~Board();
    void initialize(const std::vector<std::vector<Piece*>>& grid);
    void initialize(const std::string &player1Name, const std::string &player2Name);
    void display() const;
    void setTurn(const std::string& color);
    void movePiece(const std::vector<Position>& path);
    States getStates(const Piece& piece) const;
    std::vector<std::unique_ptr<Piece>> getMoveablePieces() const;
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
    // Apply chosen path back to the board
    void selectMove(const std::vector<Position>& path);
    const std::vector<std::vector<Position>>& getChoices() const { return choices; }

private:
    friend class Board;
    Board* board = nullptr;
    const Piece* piece = nullptr;
    std::vector<std::vector<Position>> choices;
    bool hasCapture = false;
};

#endif // BOARD_H