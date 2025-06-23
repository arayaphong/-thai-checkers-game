#ifndef STATES_H
#define STATES_H

#include <vector>
#include "Piece.h"

class Board; // forward declaration

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

#endif // STATES_H
