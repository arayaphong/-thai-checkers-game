#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include <vector>
#include <memory>
#include <functional>
#include "GameModel.h"
#include "Move.h"

class GameSimulator {
public:
    using MoveSelector = std::function<Move(const GameModel&, const std::vector<Move>&)>;
    
    // Run a single game with given strategies
    std::vector<Move> simulateGame(MoveSelector player1Strategy, MoveSelector player2Strategy);
    
    // Run multiple games
    std::vector<std::vector<Move>> simulateBatch(
        MoveSelector player1Strategy, 
        MoveSelector player2Strategy,
        int gameCount
    );
    
    // Built-in strategies
    static Move randomStrategy(const GameModel& model, const std::vector<Move>& validMoves);
    static Move greedyStrategy(const GameModel& model, const std::vector<Move>& validMoves);
    static Move defensiveStrategy(const GameModel& model, const std::vector<Move>& validMoves);
};

#endif // GAMESIMULATOR_H
