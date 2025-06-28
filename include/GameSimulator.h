#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include "GameModel.h"
#include "Move.h"
#include "Position.h"
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <array>

class GameSimulator {
public:
    virtual ~GameSimulator() = default;
    void run();

protected:
    GameModel game;
    int scenarioCount_ = 0;
    void printBoard() const;
    virtual void initializeGame() = 0;
    virtual Move selectMove(const std::map<Position, std::vector<Move>>& allMoves) = 0;
    virtual void onGameOver() = 0;

    // Recursive exploration helper
    void simulateRecursive(std::unique_ptr<GameModel> model, int depth = 0);
    // Board-printing helper for arbitrary grid
    static void printBoardGrid(const std::array<std::array<Piece*, 8>, 8>& board);
};

#endif // GAMESIMULATOR_H
