#pragma once
#include "Piece.h"
#include <vector>
#include <memory>
#include <map>
#include <set>

struct Move {
    Position from;
    std::vector<Position> path;
    std::vector<Position> captured;
    std::string playerColor;
    // Returns true if this move captures at least one piece
    bool isCapture() const { return !captured.empty(); }
    // Returns the number of captured pieces
    int captureCount() const { return static_cast<int>(captured.size()); }
};

class GameModel {
    static constexpr int BOARD_SIZE = 8;
    std::vector<std::vector<std::unique_ptr<Piece>>> grid;
    std::string currentPlayer;
    std::string player1Name;
    std::string player2Name;
    std::vector<Move> moveHistory;
    
public:
    GameModel();
    // Destructor
    ~GameModel();
    void initializeStandardGame(const std::string& player1, const std::string& player2);
    void initializeFromGrid(const std::vector<std::vector<std::unique_ptr<Piece>>>& initialGrid);
    // Overload to initialize from raw pointer grid
    void initializeFromGrid(const std::vector<std::vector<Piece*>>& rawGrid);
    void setCurrentPlayer(const std::string& player);
    // Getter for current player's color
    std::string getCurrentPlayer() const;
    // Get a copy of the board as raw pointers for readonly access
    std::vector<std::vector<Piece*>> getBoard() const;
    std::vector<Move> getValidMoves(const Position& piecePos) const;
    std::map<Position, std::vector<Move>> getAllValidMoves() const;
    void executeMove(const Move& move);
    bool isGameOver() const;
    std::string getWinner() const;
    int getPieceCount(const std::string& player) const;
    // Clone model (caller takes ownership and should delete)
    GameModel* clone() const;
    // Access move history
    const std::vector<Move>& getMoveHistory() const;

private:
    void clearGrid();
    bool isValidPosition(const Position& pos) const;
    bool canAnyPieceCapture() const;
    bool isPlayer1(const std::string& player) const;
    bool isPlayer2(const std::string& player) const;
    std::string getOpponent(const std::string& player) const;
    
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
