#include "Piece.h"
#include <string_view>

Piece::Piece(std::string_view colorArg, Position pos) 
    : color(colorArg), position(pos) {}

// Construct with explicit type
Piece::Piece(std::string_view colorArg, Position pos, Type type)
    : color(colorArg), position(pos), isKing(type == Type::Dame) {}

// Copy constructor is defaulted

std::string_view Piece::getColor() const noexcept { return color; }
const Position& Piece::getPosition() const noexcept { return position; }
void Piece::setPosition(Position newPosition) noexcept { position = newPosition; }
bool Piece::isDame() const noexcept { return isKing; }
void Piece::promote() noexcept { isKing = true; }
