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
        std::cout << "\n=== Scenario " << scenarioCount_ << " ===" << std::endl;
        // Print stage reset per scenario
        std::cout << "Stage: " << depth << std::endl;
        if (model->getWinner() != "")
            std::cout << "Winner: " << model->getWinner() << std::endl;
        else
            std::cout << "It's a draw!" << std::endl;
        // Print board
        const auto& board = model->getBoard();
        printBoardGrid(board);
        // Hash move sequence
        std::stringstream ss;
        for (const auto& mv : model->getMoveHistory()) {
            ss << mv.from.toString() << "->" << mv.path.back().toString() << ";";
        }
        size_t hash = std::hash<std::string>()(ss.str());
        // format hash into 4 hex groups of 16 bits
        std::ostringstream hoss;
        hoss << std::hex << std::uppercase << std::setfill('0')
             << std::setw(4) << ((hash >> 48) & 0xFFFF) << '-'
             << std::setw(4) << ((hash >> 32) & 0xFFFF) << '-'
             << std::setw(4) << ((hash >> 16) & 0xFFFF) << '-'
             << std::setw(4) << (hash & 0xFFFF);
        std::cout << "Scenario hash: " << hoss.str() << std::endl;
        onGameOver();
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
