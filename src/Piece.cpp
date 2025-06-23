#include "Piece.h"

Piece::Piece() : color(""), position({0, 0}) {}

const std::string& Piece::getColor() const {
    return color;
}

Position Piece::getPosition() const {
    return position;
}

void Piece::setPosition(const Position& newPosition) {
    position = newPosition;
}