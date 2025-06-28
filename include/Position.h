#ifndef POSITION_H
#define POSITION_H

struct Position {
    int x = 0;
    int y = 0;
    
    constexpr Position() noexcept = default;
    constexpr Position(int x_, int y_) noexcept : x(x_), y(y_) {}
    
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

#endif // POSITION_H
