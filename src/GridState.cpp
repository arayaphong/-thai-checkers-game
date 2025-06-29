#include "GridState.h"

State::State(uint64_t raw_state) : state(raw_state) {}

bool State::isOccupied(int position) const {
    return (state & (1ULL << position)) != 0;
}

void State::setPiece(int position, PieceType type) {
    removePiece(position);

    state |= (1ULL << position);

    uint32_t occupancy_mask = static_cast<uint32_t>(state);
    uint32_t positions_before = (1ULL << position) - 1;
    int piece_index = std::popcount(occupancy_mask & positions_before);

    uint64_t type_shift = 32 + piece_index * 2;
    state |= (static_cast<uint64_t>(type) << type_shift);
}

void State::removePiece(int position) {
    if (!isOccupied(position)) return;

    uint32_t occupancy_mask = static_cast<uint32_t>(state);
    uint32_t positions_before = (1ULL << position) - 1;
    int piece_index = std::popcount(occupancy_mask & positions_before);

    state &= ~(1ULL << position);

    uint64_t type_mask = state >> 32;
    uint64_t lower_mask = (1ULL << (piece_index * 2)) - 1;
    uint64_t upper_mask = type_mask >> ((piece_index + 1) * 2);
    uint64_t new_types = (upper_mask << (piece_index * 2)) | lower_mask;

    state = (new_types << 32) | static_cast<uint32_t>(state);
}

std::optional<PieceType> State::getPiece(int position) const {
    if (!isOccupied(position)) return std::nullopt;

    uint32_t occupancy_mask = static_cast<uint32_t>(state);
    uint32_t positions_before = (1ULL << position) - 1;
    int piece_index = std::popcount(occupancy_mask & positions_before);
    return static_cast<PieceType>((state >> (32 + piece_index * 2)) & 3);
}

int State::getPieceCount() const {
    return std::popcount(static_cast<uint32_t>(state));
}

uint64_t State::getRawState() const {
    return state;
}

uint32_t State::getOccupancyMask() const {
    return static_cast<uint32_t>(state);
}

uint32_t State::getTypeEncoding() const {
    return static_cast<uint32_t>(state >> 32);
}

std::vector<std::pair<int, PieceType>> State::getAllPieces() const {
    std::vector<std::pair<int, PieceType>> pieces;

    for (int pos = 0; pos < MAX_POSITIONS; pos++) {
        if (auto piece = getPiece(pos); piece.has_value()) {
            pieces.push_back({pos, piece.value()});
        }
    }
    return pieces;
}

bool State::isValid() const {
    return getPieceCount() <= MAX_PIECES;
}
