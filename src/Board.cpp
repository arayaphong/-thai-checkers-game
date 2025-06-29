#include "Board.h"
#include <iostream>
#include <set>
#include <algorithm>
#include <string_view>

Board::Board() = default;

void Board::setModel(GameModel* m) {
    model = m;
    ownedModel.reset();
}

void Board::initialize(const std::vector<std::vector<std::unique_ptr<Piece>>>& initialGrid) {
    ownedModel = std::make_unique<GameModel>();
    ownedModel->initializeFromGrid(initialGrid);
    model = ownedModel.get();
}

void Board::initialize() {
    ownedModel = std::make_unique<GameModel>();
    ownedModel->initializeStandardGame();
    model = ownedModel.get();
}

// Add overload to accept raw pointer grid
void Board::initialize(const std::vector<std::vector<Piece*>>& rawGrid) {
    ownedModel = std::make_unique<GameModel>();
    // Convert raw pointer grid to unique_ptr grid
    std::vector<std::vector<std::unique_ptr<Piece>>> initialGrid;
    initialGrid.resize(rawGrid.size());
    for (size_t i = 0; i < rawGrid.size(); ++i) {
        initialGrid[i].resize(rawGrid[i].size());
        for (size_t j = 0; j < rawGrid[i].size(); ++j) {
            if (rawGrid[i][j]) {
                initialGrid[i][j] = std::make_unique<Piece>(*rawGrid[i][j]);
            }
        }
    }
    ownedModel->initializeFromGrid(initialGrid);
    model = ownedModel.get();
}

void Board::setTurn(bool isBlack) noexcept {
    if (model) model->setCurrentPlayer(isBlack);
}

void Board::display() const {
    if (!model) return;
    
    const auto& grid = model->getBoard();
    
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
                if (piece->isDame()) {
                    symbol = piece->isBlackPiece() ? "♛" : "♕";
                } else {
                    symbol = piece->isBlackPiece() ? "●" : "○";
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
            const auto& grid = model->getBoard();
            if (grid[pos.x][pos.y]) {
                moveablePieces.push_back(std::make_unique<Piece>(*grid[pos.x][pos.y]));
            }
        }
    }
    
    return moveablePieces;
}

bool Board::isBlacksTurn() const {
    return model ? model->isBlacksTurnFunc() : false;
}