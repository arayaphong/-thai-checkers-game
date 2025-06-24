#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <string>
#include <map>
#include "Piece.h"
#include "Move.h"

class GameModel {
private:
    std::vector<std::vector<Piece*>> grid;
    std::string currentPlayer;
    std::vector<Move> moveHistory;
    std::string player1Name;
    std::string player2Name;
    
    // Helper methods
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

public:
    GameModel();
    ~GameModel();
    
    // Core game functionality
    void initializeStandardGame(const std::string& player1, const std::string& player2);
    void initializeFromGrid(const std::vector<std::vector<Piece*>>& initialGrid);
    void executeMove(const Move& move);
    
    // Game state access
    std::vector<std::vector<Piece*>> getBoard() const { return grid; }
    std::string getCurrentPlayer() const { return currentPlayer; }
    std::vector<Move> getMoveHistory() const { return moveHistory; }
    
    // Move queries
    std::vector<Move> getValidMoves(const Position& piecePos) const;
    std::map<Position, std::vector<Move>> getAllValidMoves() const;
    
    // Game status
    bool isGameOver() const;
    std::string getWinner() const;
    int getPieceCount(const std::string& player) const;
    
    // Advanced features
    GameModel* clone() const;
    void setCurrentPlayer(const std::string& player); // For Board compatibility
};

#endif // GAMEMODEL_H
