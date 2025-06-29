#pragma once

#include <vector>
#include "Position.h"
#include <string>

struct Move {
    Position from;
    std::vector<Position> path;
    std::vector<Position> captured;
    bool isBlack;

    [[nodiscard]] bool isCapture() const noexcept { return !captured.empty(); }
    [[nodiscard]] int captureCount() const noexcept { return static_cast<int>(captured.size()); }
};

