#ifndef GAMEANALYZER_H
#define GAMEANALYZER_H

#include <vector>
#include <map>
#include <string>
#include "GameModel.h"
#include "Move.h"

struct GameStats {
    int totalMoves;
    int totalCaptures;
    std::map<std::string, int> capturesPerPlayer;
    std::map<std::string, int> movesPerPlayer;
    std::string winner;
    int gameLength;
};

class GameAnalyzer {
public:
    // Analyze a single game
    GameStats analyzeGame(const std::vector<Move>& moveHistory);
    
    // Analyze multiple games
    std::map<std::string, double> analyzeWinRates(const std::vector<GameStats>& games);
    double averageGameLength(const std::vector<GameStats>& games);
    std::map<Position, int> analyzeHotspots(const std::vector<std::vector<Move>>& allGames);
    
    // Pattern analysis
    std::vector<std::vector<Move>> findCommonOpenings(const std::vector<std::vector<Move>>& allGames, int depth = 3);
    std::map<std::string, double> analyzeCaptureEfficiency(const std::vector<GameStats>& games);
};

#endif // GAMEANALYZER_H
