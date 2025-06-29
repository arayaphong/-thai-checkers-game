#pragma once
#include "Piece.h"
#include "Move.h"
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <array>
#include <string_view>

class GameModel {
    inline static constexpr size_t BOARD_SIZE = 8;
    inline static constexpr size_t MAX_HISTORY_SIZE = 20; // Keep last 20 positions for draw detection
    std::array<std::array<std::unique_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE> grid{};
    bool isBlacksTurn = true; // Black moves first
    std::vector<Move> moveHistory;
    
public:
    GameModel() = default;
    // Destructor
    ~GameModel() = default;
    void initializeStandardGame();
    void initializeFromGrid(const std::vector<std::vector<std::unique_ptr<Piece>>>& initialGrid);
    // Overload to initialize from raw pointer grid
    void initializeFromGrid(const std::vector<std::vector<Piece*>>& rawGrid);
    void setCurrentPlayer(bool isBlack) noexcept;
    // Getter for current player's color
    [[nodiscard]] bool isBlacksTurnFunc() const noexcept;
    // Get a copy of the board as raw pointers for readonly access
    [[nodiscard]] std::array<std::array<Piece*, BOARD_SIZE>, BOARD_SIZE> getBoard() const noexcept;
    [[nodiscard]] std::vector<Move> getValidMoves(const Position& piecePos) const;
    [[nodiscard]] std::map<Position, std::vector<Move>> getAllValidMoves() const;
    [[nodiscard]] std::map<Position, std::vector<Move>> getValidMovesForPlayer(bool isBlack) const;
    void executeMove(const Move& move);
    [[nodiscard]] bool isGameOver() const noexcept;
    [[nodiscard]] std::string getWinner() const noexcept;
    [[nodiscard]] int getPieceCount(bool isBlack) const noexcept;
    [[nodiscard]] std::unique_ptr<GameModel> clone() const;

private:
    void clearGrid();
    static constexpr bool isValidPosition(const Position& pos) noexcept;
    bool canAnyPieceCapture() const;
    bool getOpponent() const noexcept;
    
    // Move generation helpers
    std::vector<Move> generatePionSimpleMoves(const Position& from) const;
    std::vector<Move> generatePionCaptureMoves(const Position& from) const;
    std::vector<Move> generateDameSimpleMoves(const Position& from) const;
    std::vector<Move> generateDameCaptureMoves(const Position& from) const;
    void generatePionCaptureSequences(const Position& from, const Position& current,
                                  std::vector<Position>& path, std::vector<Position>& captured,
                                  std::vector<Move>& allMoves) const;
    void generateDameCaptureSequences(const Position& from, const Position& current,
                                  std::vector<Position>& path, std::vector<Position>& captured,
                                  std::vector<Move>& allMoves, int dx, int dy) const;
    void checkPromotion(const Position& pos);
    
    // Rule validation helpers
    bool isValidPionMove(const Position& from, const Position& to, const Piece* piece) const;
};
