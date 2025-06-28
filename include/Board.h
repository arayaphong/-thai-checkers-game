#pragma once
#include "GameModel.h"
#include <memory>
#include <vector>
#include <map>

class Board {
    std::unique_ptr<GameModel> ownedModel;
    GameModel* model = nullptr;
    
public:
    Board();
    void setModel(GameModel* m);
    void initialize(const std::vector<std::vector<std::unique_ptr<Piece>>>& initialGrid);
    void initialize(const std::vector<std::vector<Piece*>>& initialGrid); // Overload for raw pointer grid
    void initialize(const std::string& p1, const std::string& p2);
    void setTurn(const std::string& color);
    void display() const;
    std::vector<Move> getValidMovesFor(const Position& pos) const;
    std::map<Position, std::vector<Move>> getAllValidMoves() const;
    void executeMove(const Move& move);
    std::vector<std::unique_ptr<Piece>> getMoveablePieces() const;
    std::string getCurrentPlayer() const;
};
