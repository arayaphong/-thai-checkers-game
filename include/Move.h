#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include "Position.h"

struct Move {
    Position from;
    std::vector<Position> path;
    std::vector<Position> captures;
    std::string player;
    
    bool isCapture() const { return !captures.empty(); }
    int captureCount() const { return captures.size(); }
};

#endif // MOVE_H
