#pragma once

#include <string>

struct Position {
    int x = 0;
    int y = 0;
    
    constexpr Position() noexcept = default;
    constexpr Position(int x_, int y_) noexcept : x(x_), y(y_) {}

    [[nodiscard]] std::string toString() const noexcept {
        return std::string(1, char('A' + y)) + std::to_string(x + 1);
    }
    
    [[nodiscard]] constexpr bool operator==(const Position& other) const noexcept {
        return x == other.x && y == other.y;
    }
    [[nodiscard]] constexpr bool operator!=(const Position& other) const noexcept {
        return !(*this == other);
    }
    
    [[nodiscard]] constexpr bool operator<(const Position& other) const noexcept {
        return x < other.x || (x == other.x && y < other.y);
    }
};

