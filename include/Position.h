#ifndef POSITION_H
#define POSITION_H

struct Position {
    int x, y;
    
    Position() : x(0), y(0) {}
    Position(int x, int y) : x(x), y(y) {}
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator<(const Position& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

#endif // POSITION_H
