#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <memory>
#include <string>
#include <map>
#include "Piece.h"
#include "Move.h"  // Include the Move header

class GameModel {
private:
    std::vector<std::vector<Piece*>> grid;
    std::string currentPlayer;
    std::vector<Move> moveHistory;
    
    // Helper methods for move generation
    std::vector<Move> generateSimpleMoves(const Position& from) const;
    std::vector<Move> generateCaptureMoves(const Position& from) const;
    void generateCaptureSequences(const Position& from, const Position& current,
                                  std::vector<Position>& path,
                                  std::vector<Position>& captured,
                                  std::vector<Move>& allMoves) const;
    
    // Dame-specific move generation
    std::vector<Move> generateDameSimpleMoves(const Position& from) const;
    std::vector<Move> generateDameCaptureMoves(const Position& from) const;
    void generateDameCaptureSequences(const Position& from, const Position& current,
                                     std::vector<Position>& path,
                                     std::vector<Position>& captured,
                                     std::vector<Move>& allMoves,
                                     int dx, int dy) const;
    
public:
    GameModel();
    ~GameModel();
    
    // Initialization
    void initializeStandardGame(const std::string& player1, const std::string& player2);
    void initializeFromGrid(const std::vector<std::vector<Piece*>>& initialGrid);
    
    // Game state queries
    std::vector<std::vector<Piece*>> getBoard() const { return grid; }
    std::string getCurrentPlayer() const { return currentPlayer; }
    std::vector<Move> getMoveHistory() const { return moveHistory; }
    
    // Move generation and validation
    std::vector<Move> getValidMoves(const Position& piecePos) const;
    std::map<Position, std::vector<Move>> getAllValidMoves() const;
    bool isValidMove(const Move& move) const;
    
    // Move execution
    void executeMove(const Move& move);
    void undoLastMove();
    
    // Game state analysis
    bool isGameOver() const;
    std::string getWinner() const;
    int getPieceCount(const std::string& player) const;
    
    // Analytics helpers
    std::vector<Move> getAllPossibleMoves() const;
    double evaluatePosition(const std::string& player) const;
    GameModel* clone() const; // For simulation

    // Set the current player for custom grids and turn management
    void setCurrentPlayer(const std::string& player);

    // Promotion check
    void checkPromotion(const Position& pos);
};

#endif // GAMEMODEL_H
