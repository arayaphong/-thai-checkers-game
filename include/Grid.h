#pragma once

#include <bit>
#include <array>
#include <memory>
#include <vector>
#include <optional>
#include "GridState.h"
#include "PieceWrapper.h"

class Grid : public std::enable_shared_from_this<Grid> {
    State state;
    std::size_t pieceCount = 0;
    static constexpr std::size_t MaxPieces = 16;
    std::array<std::shared_ptr<PieceWrapper>, MaxPieces> pieces{};

public:
    // Added default constructor initializing state
    Grid() : state(0) {}

    void clear() {
        pieceCount = 0;
        pieces.fill(nullptr);
        state = State(0);
    }

    void updateToGrid(const PieceWrapper& piece) {
        const auto &pos = piece.getPosition();
        const int index = pos.x + pos.y * 8;

        const PieceType type = piece.isBlackPiece()
                       ? (piece.isDame() ? PieceType::BLACK_DAME : PieceType::BLACK_PION)
                       : (piece.isDame() ? PieceType::WHITE_DAME : PieceType::WHITE_PION);

        state.setPiece(index, type);
    }

    const auto createPiece(const bool isBlack, const Position& pos) {
        if (pieceCount >= MaxPieces) {
            throw std::runtime_error("Maximum piece count reached");
        }
        
        auto newPiece = std::make_shared<PieceWrapper>(shared_from_this(), isBlack, pos);
        if (pieceCount < MaxPieces) {
            pieces[pieceCount++] = newPiece;
        }
        updateToGrid(*newPiece);
        return newPiece;
    }

    bool isValidPosition(const int x, const int y) const {
        const int index = x + y * 8;
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    bool isOccupied(const int x, const int y) const {
        const int index = x + y * 8;
        return state.isOccupied(index);
    }

    const PieceWrapper& getPiece(const int x, const int y) const {
        const int index = x + y * 8;
        return *pieces[index];
    }

    void setPiece(const int x, const int y, const PieceWrapper& piece) {
        const int index = x + y * 8;
        pieces[index] = std::make_shared<PieceWrapper>(piece);
        updateToGrid(piece);
    }

    void removePiece(const int x, const int y) {
        const int index = x + y * 8;
        pieces[index].reset();
        state.removePiece(index);
    }
};
