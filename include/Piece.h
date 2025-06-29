#pragma once
#include "Position.h"

class Piece {
    bool isBlack;
    Position position;
    bool isKing = false;

public:
    Piece() = default;
    Piece(bool isBlackPiece, Position pos);
    // Type of piece (Pion or Dame)
    enum class Type { Normal, Dame };
    // Construct with explicit type
    Piece(bool isBlackPiece, Position pos, Type type);
    Piece(const Piece& other) = default;
    
    [[nodiscard]] bool isBlackPiece() const noexcept;
    [[nodiscard]] const Position& getPosition() const noexcept;
    void setPosition(Position newPosition) noexcept;
    [[nodiscard]] bool isDame() const noexcept;
    void promote() noexcept;
};
