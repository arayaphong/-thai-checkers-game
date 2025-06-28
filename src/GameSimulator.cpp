#include "GameSimulator.h"
#include <sstream>
#include <functional>
#include <iomanip>

void GameSimulator::run() {
    initializeGame();
    scenarioCount_ = 0;
    std::cout << "\nLegend: ●=White Pion, ■=White Dame, ○=Black Pion, □=Black Dame\n\n";
    // Start recursive exploration
    auto rootModel = game.clone();
    simulateRecursive(std::move(rootModel), 0);
}

void GameSimulator::simulateRecursive(std::unique_ptr<GameModel> model, int depth) {
    auto allMoves = model->getAllValidMoves();
    if (model->isGameOver() || allMoves.empty()) {
        // Leaf: print final state, hash and wait
        ++scenarioCount_;
        std::cout << "Scenario " << scenarioCount_ << std::endl;
        // Print number of moves made in this scenario path
        std::cout << "Moved: " << depth << std::endl;
        if (model->getWinner() != "")
            std::cout << "Winner: " << model->getWinner() << std::endl;
        else {
            if (model->isInsufficientMaterial()) {
                std::cout << "It's a draw!" << std::endl;
            }
        }
        // Print board
        const auto& board = model->getBoard();
        printBoardGrid(board);
        onGameOver();

        std::cout << "__________________\n\n";
        // std::cout << "Press Enter to continue..." << std::endl;
        // std::cin.get();
        return;
    }
    // Recurse for each possible move
    for (const auto& [pos, moves] : allMoves) {
        for (const auto& mv : moves) {
            auto child = model->clone();
            child->executeMove(mv);
            simulateRecursive(std::move(child), depth+1);
        }
    }
}

void GameSimulator::printBoard() const {
    const auto& board = game.getBoard();
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
                if (board[i][j]->getColor() == "White") {
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
                if (board[i][j]->getColor() == "White") {
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
