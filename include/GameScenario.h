#ifndef GAMESCENARIO_H
#define GAMESCENARIO_H

#include "GameSimulator.h"
#include <map>
#include <vector>

class GameScenario : public GameSimulator {
public:
    GameScenario();
    ~GameScenario() override = default;

protected:
    void initializeGame() override;
    Move selectMove(const std::map<Position, std::vector<Move>>& allMoves) override;
    void onGameOver() override;

private:
    // custom scenario state if needed
};

#endif // GAMESCENARIO_H
