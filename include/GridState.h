#pragma once

#include <bit>
#include <array>
#include <optional>
#include <vector>
#include <cstdint>

constexpr int MAX_POSITIONS = 32;
constexpr int MAX_PIECES = 16;

enum class PieceType : uint8_t {
    WHITE_PION = 0,
    WHITE_DAME = 1,
    BLACK_PION = 2,
    BLACK_DAME = 3
};

class State {
private:
    uint64_t state;

public:
    explicit State(uint64_t raw_state);

    bool isOccupied(int position) const;

    void setPiece(int position, PieceType type);

    void removePiece(int position);

    std::optional<PieceType> getPiece(int position) const;

    int getPieceCount() const;

    uint64_t getRawState() const;

    uint32_t getOccupancyMask() const;

    uint32_t getTypeEncoding() const;

    std::vector<std::pair<int, PieceType>> getAllPieces() const;

    bool isValid() const;

    auto operator<=>(const State& other) const = default;
};
