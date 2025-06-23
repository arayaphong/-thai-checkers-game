#ifndef PIECE_H
#define PIECE_H

#include <string>
#include "Position.h"

class Piece {
public:
    enum class Type { Regular, Dame };
    
    Piece();
    Piece(const Piece& piece) = default;
    Piece(const std::string& color, const Position& position)
        : color(color), position(position) {}
    
    const std::string& getColor() const;
    Position getPosition() const;
    void setPosition(const Position& newPosition);
    
    virtual ~Piece() = default;
    virtual Type getType() const { return Type::Regular; }

private:
    std::string color;
    Position position;
};

#endif // PIECE_H