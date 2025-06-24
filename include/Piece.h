#ifndef PIECE_H
#define PIECE_H

#include <string>
#include "Position.h"

class Piece {
public:
    enum class Type { Pion, Dame };
    
    Piece();
    Piece(const Piece& piece) = default;
    Piece(const std::string& color, const Position& position, Type type = Type::Pion)
        : color(color), position(position), type(type) {}
    
    const std::string& getColor() const;
    Position getPosition() const;
    void setPosition(const Position& newPosition);
    
    Type getType() const { return type; }
    void promote() { type = Type::Dame; }
    bool isDame() const { return type == Type::Dame; }
    bool isPion() const { return type == Type::Pion; }
    
    // Capability methods for explicit rule enforcement
    bool canMoveBackward() const { return isDame(); }
    bool canCaptureBackward() const { return isDame(); }
    bool canMoveMultipleSquares() const { return isDame(); }
    
    virtual ~Piece() = default;

private:
    std::string color;
    Position position;
    Type type;
};

#endif // PIECE_H