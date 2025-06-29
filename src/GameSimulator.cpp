#include "GameSimulator.h"
#include <sstream>
#include <functional>
#include <iomanip>
#include <iostream>

namespace {
    std::string gameResultToString(GameResult result) {
        switch (result) {
            case GameResult::ONGOING: return "Game Ongoing";
            case GameResult::BLACK_WINS: return "Black";
            case GameResult::WHITE_WINS: return "White";
            case GameResult::DRAW: return "Draw";
            default: return "Unknown";
        }
    }
}

void GameSimulator::run() {
    initializeGame();
    scenarioCount = 0;
    std::cout << "\nLegend: ●=White Pion, ■=White Dame, ○=Black Pion, □=Black Dame\n\n";
    
    // Start recursive exploration
    auto rootModel = game.clone();
    simulateRecursive(std::move(rootModel));
}

void GameSimulator::simulateRecursive(std::unique_ptr<GameModel> model) {    
    auto allMoves = model->getAllValidMoves();
    if (model->isGameOver()) {
        ++scenarioCount;

        std::cout << "Scenario " << scenarioCount << std::endl;
        std::cout << " Winner: " << gameResultToString(model->getWinner()) << std::endl;
        printBoardGrid(model->getBoard());
        std::cout << " -----------------\n\n";

        const auto& board = model->getBoard();
        onGameOver();

        // std::cout << "Press Enter to continue..." << std::endl;
        // std::cin.get();

        // if (scenarioCount > 2) exit(0);
        return;
    }
    // Recurse for each possible move
    for (const auto& [pos, moves] : allMoves) {
        for (const auto& mv : moves) {
            auto child = model->clone();
            child->executeMove(mv);
            simulateRecursive(std::move(child));
        }
    }
}

// Static board-printing helper
void GameSimulator::printBoardGrid(const std::array<std::array<Piece*, 8>, 8>& board) {
    // Column indices header
    std::cout << "   ";
    for (char col = 'A'; col <= 'H'; ++col) {
        std::cout << col << " ";
    }
    std::cout << "\n";

    // Iterate rows top-down
    for (int i = 0; i < 8; ++i) {
        std::cout << " " << i + 1 << " ";
        for (int j = 0; j < 8; ++j) {
            std::string symbol = " ";
            if ((i + j) % 2 == 1) symbol = ".";
            if (board[i][j]) {
                if (!board[i][j]->isBlackPiece()) {
                    symbol = board[i][j]->isDame() ? "■" : "●";
                } else {
                    symbol = board[i][j]->isDame() ? "□" : "○";
                }
            }
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
}
