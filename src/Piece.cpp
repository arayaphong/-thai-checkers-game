#include "Piece.h"

Piece::Piece(bool isBlackPiece, Position pos) 
    : isBlack(isBlackPiece), position(pos) {}

// Construct with explicit type
Piece::Piece(bool isBlackPiece, Position pos, Type type)
    : isBlack(isBlackPiece), position(pos), isKing(type == Type::Dame) {}

// Copy constructor is defaulted

bool Piece::isBlackPiece() const noexcept { return isBlack; }
const Position& Piece::getPosition() const noexcept { return position; }
void Piece::setPosition(Position newPosition) noexcept { position = newPosition; }
bool Piece::isDame() const noexcept { return isKing; }
void Piece::promote() noexcept { isKing = true; }
