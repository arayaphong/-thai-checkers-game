#include "GameScenario.h"

GameScenario::GameScenario() {}

void GameScenario::initializeGame() {
    game.initializeStandardGame("White", "Black");
}

Move GameScenario::selectMove(const std::map<Position, std::vector<Move>>& allMoves) {
    // default: pick first available move
    return allMoves.begin()->second.front();
}

void GameScenario::onGameOver() {
    // can log scenario end or cleanup if needed
}
