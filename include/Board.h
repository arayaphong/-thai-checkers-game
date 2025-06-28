#pragma once
#include "GameModel.h"
#include <memory>
#include <vector>
#include <map>
#include <string_view>

class Board {
    std::unique_ptr<GameModel> ownedModel;
    GameModel* model = nullptr;
    
public:
    Board();
    void setModel(GameModel* m);
    void initialize(const std::vector<std::vector<std::unique_ptr<Piece>>>& initialGrid);
    void initialize(const std::vector<std::vector<Piece*>>& initialGrid); // Overload for raw pointer grid
    void initialize(std::string_view p1, std::string_view p2);
    void setTurn(std::string_view color) noexcept;
    void display() const;
    std::vector<Move> getValidMovesFor(const Position& pos) const;
    std::map<Position, std::vector<Move>> getAllValidMoves() const;
    void executeMove(const Move& move);
    std::vector<std::unique_ptr<Piece>> getMoveablePieces() const;
    [[nodiscard]] std::string_view getCurrentPlayer() const;
};
