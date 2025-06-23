#ifndef PIECE_H
#define PIECE_H

#include <string>

struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

class Piece {
public:
    enum class Type { Regular, Dame };    // added type enum
    Piece();
    Piece(const Piece& piece) = default;  // copy constructor
    Piece(const std::string& color, Position position)
        : color(color), position(position) {}
    const std::string& getColor() const;
    Position getPosition() const;
    void setPosition(Position newPosition);
    virtual ~Piece() = default;            // make base class polymorphic
    virtual Type getType() const { return Type::Regular; }  // default type

private:
    std::string color;
    Position position;
};

#endif // PIECE_H