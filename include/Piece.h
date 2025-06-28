#pragma once
#include <string>
#include <string_view>
#include "Position.h"

class Piece {
    std::string color;
    Position position;
    bool isKing = false;

public:
    Piece() = default;
    Piece(std::string_view color, Position pos);
    // Type of piece (Pion or Dame)
    enum class Type { Pion, Dame };
    // Construct with explicit type
    Piece(std::string_view color, Position pos, Type type);
    Piece(const Piece& other) = default;
    
    [[nodiscard]] std::string_view getColor() const noexcept;
    [[nodiscard]] const Position& getPosition() const noexcept;
    void setPosition(Position newPosition) noexcept;
    [[nodiscard]] bool isDame() const noexcept;
    void promote() noexcept;
};
