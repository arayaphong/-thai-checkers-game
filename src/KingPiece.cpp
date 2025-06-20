#include "KingPiece.h"

KingPiece::KingPiece() : Piece() {}

KingPiece::KingPiece(const std::string& color, Position position)
    : Piece(color, position) {}

// Implement king-specific methods here if needed
