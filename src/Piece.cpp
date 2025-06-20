#include "Piece.h"

Piece::Piece() : color(""), position({0, 0}) {}

Piece::Piece(const std::string& c, Position pos) : color(c), position(pos) {}

const std::string& Piece::getColor() const {
    return color;
}

Position Piece::getPosition() const {
    return position;
}

void Piece::setPosition(Position newPosition) {
    position = newPosition;
}