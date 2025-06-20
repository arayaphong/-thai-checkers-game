#ifndef KINGPIECE_H
#define KINGPIECE_H

#include "Piece.h"

class KingPiece : public Piece {
public:
    KingPiece();
    KingPiece(const std::string& color, Position position);
    Type getType() const override { return Type::King; }  // override type
    // Add king-specific methods here if needed
};

#endif // KINGPIECE_H
