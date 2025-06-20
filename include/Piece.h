#ifndef PIECE_H
#define PIECE_H

#include <string>

struct Position {
    int x;
    int y;
};

class Piece {
public:
    enum class Type { Regular, King };    // added type enum
    Piece();
    Piece(const std::string& color, Position position);
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