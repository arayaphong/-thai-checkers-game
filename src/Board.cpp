#include "Board.h"
#include <iostream>
#include <iomanip>
#include <set>
#include <algorithm>

Board::Board() : model(nullptr) {}

Board::~Board() {}

void Board::setModel(GameModel* m) {
    model = m;
    ownedModel.reset(); // Clear owned model when external model is set
}

void Board::initialize(const std::vector<std::vector<Piece*>>& initialGrid) {
    ownedModel = std::make_unique<GameModel>();
    ownedModel->initializeFromGrid(initialGrid);
    model = ownedModel.get();
}

void Board::initialize(const std::string& p1, const std::string& p2) {
    ownedModel = std::make_unique<GameModel>();
    ownedModel->initializeStandardGame(p1, p2);
    model = ownedModel.get();
}

void Board::setTurn(const std::string& color) {
    if (model) {
        model->setCurrentPlayer(color);
    }
}

void Board::display() const {
    if (!model) return;
    
    auto grid = model->getBoard();
    
    // Get all unique player colors to determine symbols
    std::set<std::string> playerColors;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j]) {
                playerColors.insert(grid[i][j]->getColor());
            }
        }
    }
    
    // Convert to vector for consistent ordering
    std::vector<std::string> players(playerColors.begin(), playerColors.end());
    std::sort(players.begin(), players.end()); // Consistent ordering
    
    // Print column indices
    std::cout << "  ";
    for (int j = 0; j < 8; ++j) {
        std::cout << j << " ";
    }
    std::cout << std::endl;
    
    // Print each row with row index
    for (int i = 0; i < 8; ++i) {
        std::cout << i << " ";
        for (const auto& piece : grid[i]) {
            if (piece) {
                // Determine symbol based on piece color and type
                const char* symbol;
                bool isFirstPlayer = !players.empty() && piece->getColor() == players[0];
                
                if (piece->isDame()) {
                    symbol = isFirstPlayer ? "♛" : "♕";
                } else {
                    symbol = isFirstPlayer ? "●" : "○";
                }
                std::cout << symbol << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

std::vector<Move> Board::getValidMovesFor(const Position& pos) const {
    if (!model) return {};
    return model->getValidMoves(pos);
}

std::map<Position, std::vector<Move>> Board::getAllValidMoves() const {
    if (!model) return {};
    return model->getAllValidMoves();
}

void Board::executeMove(const Move& move) {
    if (model) {
        model->executeMove(move);
    }
}

std::vector<std::unique_ptr<Piece>> Board::getMoveablePieces() const {
    if (!model) return {};
    
    std::vector<std::unique_ptr<Piece>> moveablePieces;
    auto allMoves = model->getAllValidMoves();
    
    for (const auto& [pos, moves] : allMoves) {
        if (!moves.empty()) {
            auto grid = model->getBoard();
            if (grid[pos.x][pos.y]) {
                moveablePieces.push_back(std::make_unique<Piece>(*grid[pos.x][pos.y]));
            }
        }
    }
    
    return moveablePieces;
}

std::string Board::getCurrentPlayer() const {
    return model ? model->getCurrentPlayer() : "";
}