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
    std::array<std::array<std::unique_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE> grid{};
    std::string currentPlayer;
    std::string player1Name;
    std::string player2Name;
    std::vector<Move> moveHistory;
    
public:
    GameModel() = default;
    // Destructor
    ~GameModel() = default;
    void initializeStandardGame(std::string_view player1, std::string_view player2);
    void initializeFromGrid(const std::vector<std::vector<std::unique_ptr<Piece>>>& initialGrid);
    // Overload to initialize from raw pointer grid
    void initializeFromGrid(const std::vector<std::vector<Piece*>>& rawGrid);
    void setCurrentPlayer(std::string_view player) noexcept;
    // Getter for current player's color
    [[nodiscard]] std::string_view getCurrentPlayer() const noexcept;
    // Get a copy of the board as raw pointers for readonly access
    [[nodiscard]] std::array<std::array<Piece*, BOARD_SIZE>, BOARD_SIZE> getBoard() const noexcept;
    [[nodiscard]] std::vector<Move> getValidMoves(const Position& piecePos) const;
    [[nodiscard]] std::map<Position, std::vector<Move>> getAllValidMoves() const;
    void executeMove(const Move& move);
    [[nodiscard]] bool isGameOver() const noexcept;
    [[nodiscard]] std::string_view getWinner() const noexcept;
    [[nodiscard]] int getPieceCount(std::string_view player) const noexcept;
    [[nodiscard]] std::unique_ptr<GameModel> clone() const;
    [[nodiscard]] const std::vector<Move>& getMoveHistory() const noexcept;

private:
    void clearGrid();
    static constexpr bool isValidPosition(const Position& pos) noexcept;
    bool canAnyPieceCapture() const;
    bool isPlayer1(std::string_view player) const noexcept;
    bool isPlayer2(std::string_view player) const noexcept;
    std::string_view getOpponent(std::string_view player) const noexcept;
    
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
