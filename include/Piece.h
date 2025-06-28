#pragma once
#include <string>

struct Position { 
    int x, y; 

    // Enable use in associative containers and comparisons
    bool operator<(const Position& other) const { return x != other.x ? x < other.x : y < other.y; }
    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
};

class Piece {
    std::string color;
    Position position;
    bool isKing = false;

public:
    Piece() = default;
    Piece(std::string color, Position pos);
    // Type of piece (Pion or Dame)
    enum class Type { Pion, Dame };
    // Construct with explicit type
    Piece(std::string color, Position pos, Type type);
    Piece(const Piece& other);  // Copy constructor
    
    const std::string& getColor() const;
    Position getPosition() const;
    void setPosition(Position newPosition);
    bool isDame() const;
    void promote();
};
