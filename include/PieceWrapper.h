#pragma once
#include "Piece.h"
#include <memory>

class Grid; // Forward declaration

class PieceWrapper {
    Piece piece;
    std::shared_ptr<Grid> grid;
public:
    // Default constructor
    PieceWrapper() = default;
    // Main constructor
    PieceWrapper(std::shared_ptr<Grid> g, bool isBlack, Position pos)
        : grid(std::move(g)), piece(isBlack, pos) {}
    // Copy constructor
    PieceWrapper(const PieceWrapper& other)
        : piece(other.piece), grid(other.grid) {}
    // Assignment operator
    PieceWrapper& operator=(const PieceWrapper& other) {
        if (this != &other) {
            piece = other.piece;
            grid = other.grid;
        }
        return *this;
    }

    bool isBlackPiece() const noexcept { return piece.isBlackPiece(); }
    bool isDame() const noexcept { return piece.isDame(); }
    const Position& getPosition() const noexcept { return piece.getPosition(); }
    void setPosition(Position pos) noexcept;
    void promote() noexcept;
    const PieceWrapper& getPiece() const noexcept { return *this; }
    PieceWrapper& getPiece() noexcept { return *this; }
};
