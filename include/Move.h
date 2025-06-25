#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include "Position.h"
#include <string>

struct Move {
    Position from;
    std::vector<Position> path;
    std::vector<Position> captured;  // Changed from 'captures' to 'captured'
    std::string player;
    
    bool isCapture() const { return !captured.empty(); }
    int captureCount() const { return captured.size(); }
};

#endif // MOVE_H
