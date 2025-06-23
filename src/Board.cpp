#include "Board.h"
#include <iostream>
#include <iomanip>

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
                    symbol = (piece->getColor() == "Player1" || piece->getColor() == "P1") ? "♛" : "♕";
                } else {
                    symbol = (piece->getColor() == "Player1" || piece->getColor() == "P1") ? "●" : "○";
                }
                std::cout << symbol << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

void Board::displayMoveHistory() const {
    if (!model) return;
    
    auto history = model->getMoveHistory();
    std::cout << "Move History:\n";
    for (size_t i = 0; i < history.size(); ++i) {
        const auto& move = history[i];
        std::cout << i + 1 << ". " << move.player << ": ";
        std::cout << "(" << move.from.x << "," << move.from.y << ") -> ";
        for (const auto& pos : move.path) {
            std::cout << "(" << pos.x << "," << pos.y << ") ";
        }
        if (move.isCapture()) {
            std::cout << "[captured " << move.captureCount() << "]";
        }
        std::cout << "\n";
    }
}

void Board::displayStatistics() const {
    if (!model) return;
    
    std::cout << "Game Statistics:\n";
    std::cout << "Current Player: " << model->getCurrentPlayer() << "\n";
    std::cout << "Piece Count:\n";
    // This assumes we know the player names
    // In practice, we'd get these from the model
    std::cout << "  Player1: " << model->getPieceCount("Player1") << "\n";
    std::cout << "  Player2: " << model->getPieceCount("Player2") << "\n";
    std::cout << "Total Moves: " << model->getMoveHistory().size() << "\n";
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