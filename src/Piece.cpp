#include "Piece.h"

Piece::Piece(std::string color, Position pos) 
    : color(std::move(color)), position(pos) {}

// Construct with explicit type
Piece::Piece(std::string color, Position pos, Type type)
    : color(std::move(color)), position(pos), isKing(type == Type::Dame) {}

Piece::Piece(const Piece& other)
    : color(other.color), position(other.position), isKing(other.isKing) {}

const std::string& Piece::getColor() const { return color; }
Position Piece::getPosition() const { return position; }
void Piece::setPosition(Position newPosition) { position = newPosition; }
bool Piece::isDame() const { return isKing; }
void Piece::promote() { isKing = true; }
