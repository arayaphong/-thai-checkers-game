#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include <string>
#include "Piece.h"
#include "GameModel.h"

class Board {
private:
    GameModel* model;
    std::unique_ptr<GameModel> ownedModel;
    
public:
    Board();
    ~Board();
    
    // Model management
    void setModel(GameModel* m);
    GameModel* getModel() { return model; }
    const GameModel* getModel() const { return model; }
    
    // Initialization
    void initialize(const std::string& player1, const std::string& player2);
    void initialize(const std::vector<std::vector<Piece*>>& customGrid);
    
    // Display methods (for debugging/visualization)
    void display() const;
    
    // Analytics-focused methods
    std::vector<Move> getValidMovesFor(const Position& pos) const;
    std::map<Position, std::vector<Move>> getAllValidMoves() const;
    void executeMove(const Move& move);  // Wrapper for model->executeMove()
    
    // Legacy compatibility (to be removed)
    void setTurn(const std::string& player);
    std::vector<std::unique_ptr<Piece>> getMoveablePieces() const;
    std::string getCurrentPlayer() const;  // Wrapper for model->getCurrentPlayer()
};

#endif // BOARD_H