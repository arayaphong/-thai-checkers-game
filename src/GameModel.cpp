#include "GameModel.h"
#include <algorithm>
#include <array>
#include <stack>
#include <sstream>

namespace {
inline constexpr std::array<std::pair<int, int>, 4> DIAGONAL_DIRECTIONS{{{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}};
}

void GameModel::clearGrid() {
    for (auto& row : grid) {
        for (auto& piece : row) {
            piece.reset();
        }
    }
}

constexpr bool GameModel::isValidPosition(const Position& pos) noexcept {
    return pos.x >= 0 && pos.x < static_cast<int>(BOARD_SIZE)            && pos.y >= 0 && pos.y < static_cast<int>(BOARD_SIZE);
}

bool GameModel::getOpponent() const noexcept {
    return !isBlacksTurn;
}

void GameModel::initializeStandardGame() {
    clearGrid();
    
    // Place black pieces (top of board)
    for (int row = 0; row < 2; ++row) {
        for (int col = (row % 2); col < BOARD_SIZE; col += 2) {
            grid[row][col] = std::make_unique<Piece>(true, Position{row, col});
        }
    }
    
    // Place white pieces (bottom of board)
    for (int row = 6; row < BOARD_SIZE; ++row) {
        for (int col = (row % 2); col < BOARD_SIZE; col += 2) {
            grid[row][col] = std::make_unique<Piece>(false, Position{row, col});
        }
    }
    isBlacksTurn = true;
}

void GameModel::setCurrentPlayer(bool isBlack) noexcept {
    isBlacksTurn = isBlack;
}

// Get current player's color
bool GameModel::isBlacksTurnFunc() const noexcept {
    return isBlacksTurn;
}

void GameModel::initializeFromGrid(const std::vector<std::vector<std::unique_ptr<Piece>>>& initialGrid) {
    clearGrid();
    // Copy the grid
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (initialGrid[i][j]) {
                grid[i][j] = std::make_unique<Piece>(*initialGrid[i][j]);
            }
        }
    }
    isBlacksTurn = true; // Assume black starts
}

bool GameModel::canAnyPieceCapture() const {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (grid[i][j] && grid[i][j]->isBlackPiece() == isBlacksTurn) {
                std::vector<Move> captures = grid[i][j]->isDame()
                    ? generateDameCaptureMoves(Position{i, j})
                    : generatePionCaptureMoves(Position{i, j});
                if (!captures.empty()) return true;
            }
        }
    }
    return false;
}

std::vector<Move> GameModel::getValidMoves(const Position& piecePos) const {
    if (!isValidPosition(piecePos)) return {};
    Piece* piece = grid[piecePos.x][piecePos.y].get();
    if (!piece || piece->isBlackPiece() != isBlacksTurn) return {};
    
    // Mandatory capture rule: if any piece can capture, only return captures
    if (canAnyPieceCapture()) {
        return piece->isDame()
            ? generateDameCaptureMoves(piecePos)
            : generatePionCaptureMoves(piecePos);
    }
    
    // No captures available, return simple moves
    return piece->isDame()
        ? generateDameSimpleMoves(piecePos)
        : generatePionSimpleMoves(piecePos);
}

bool GameModel::isValidPionMove(const Position& from, const Position& to, const Piece* piece) const {
    if (!piece || piece->isDame()) return false;
    
    // Pions can only move one square diagonally forward
    int forwardDirection = piece->isBlackPiece() ? 1 : -1;
    int deltaX = to.x - from.x;
    int deltaY = abs(to.y - from.y);
    
    // Must move exactly one square forward diagonally
    return (deltaX == forwardDirection) && (deltaY == 1);
}

std::vector<Move> GameModel::generatePionSimpleMoves(const Position& from) const {
    std::vector<Move> moves;
    const Piece* piece = grid[from.x][from.y].get();
    if (!piece || piece->isDame()) return moves;
    
    int forwardDirection = piece->isBlackPiece() ? 1 : -1;
    for (int dy : {-1, 1}) {
        Position dest{from.x + forwardDirection, from.y + dy};
        if (isValidPosition(dest) && !grid[dest.x][dest.y]) {
            Move move{from, {dest}, {}, piece->isBlackPiece()};
            moves.push_back(move);
        }
    }
    return moves;
}

std::vector<Move> GameModel::generateDameSimpleMoves(const Position& from) const {
    std::vector<Move> moves;
    Piece* piece = grid[from.x][from.y].get();
    if (!piece || !piece->isDame()) return moves;
    
    for (const auto& [dx, dy] : DIAGONAL_DIRECTIONS) {
        for (int dist = 1; dist < BOARD_SIZE; ++dist) {
            Position dest{from.x + dist * dx, from.y + dist * dy};
            if (!isValidPosition(dest) || grid[dest.x][dest.y]) break;
            Move move{from, {dest}, {}, piece->isBlackPiece()};
            moves.push_back(move);
        }
    }
    return moves;
}

std::vector<Move> GameModel::generatePionCaptureMoves(const Position& from) const {
    std::vector<Move> moves;
    std::vector<Position> path, captured;
    generatePionCaptureSequences(from, from, path, captured, moves);
    return moves;
}

void GameModel::generatePionCaptureSequences(const Position& from, const Position& current,
                                            std::vector<Position>& path,
                                            std::vector<Position>& captured,
                                            std::vector<Move>& allMoves) const {
    const Piece* piece = grid[from.x][from.y].get();
    if (!piece) return;
    
    bool foundCapture = false;
    int forwardDirection = piece->isBlackPiece() ? 1 : -1;
    
    for (int dy : {-1, 1}) {
        int dx = forwardDirection;
        Position enemy{current.x + dx, current.y + dy};
        Position landing{current.x + 2*dx, current.y + 2*dy};
        
        if (!isValidPosition(landing)) continue;
        if (!grid[enemy.x][enemy.y] || grid[enemy.x][enemy.y]->isBlackPiece() == piece->isBlackPiece()) continue;
        if (grid[landing.x][landing.y]) continue;
        if (std::find(captured.begin(), captured.end(), enemy) != captured.end()) continue;
        
        foundCapture = true;
        path.push_back(landing);
        captured.push_back(enemy);
        generatePionCaptureSequences(from, landing, path, captured, allMoves);
        path.pop_back();
        captured.pop_back();
    }
    
    if (!foundCapture && !captured.empty()) {
        Move move{from, path, captured, piece->isBlackPiece()};
        allMoves.push_back(move);
    }
}

std::vector<Move> GameModel::generateDameCaptureMoves(const Position& from) const {
    std::vector<Move> allMoves;
    std::vector<Position> path;
    std::vector<Position> captured;
    generateDameCaptureSequences(from, from, path, captured, allMoves, 0, 0);
    return allMoves;
}

void GameModel::generateDameCaptureSequences(const Position& from, const Position& current,
                                            std::vector<Position>& path,
                                            std::vector<Position>& captured,
                                            std::vector<Move>& allMoves, int dx, int dy) const {
    const Piece* piece = grid[from.x][from.y].get();
    if (!piece || !piece->isDame()) return;
    
    bool foundCapture = false;
    for (const auto& [dx, dy] : DIAGONAL_DIRECTIONS) {
        for (int dist = 1; dist < BOARD_SIZE; ++dist) {
            Position checkPos{current.x + dist * dx, current.y + dist * dy};
            if (!isValidPosition(checkPos)) break;
            
            // Found enemy piece
            if (grid[checkPos.x][checkPos.y] && grid[checkPos.x][checkPos.y]->isBlackPiece() != piece->isBlackPiece()) {
                if (std::find(captured.begin(), captured.end(), checkPos) != captured.end()) break;
                
                Position landing{current.x + (dist + 1) * dx, current.y + (dist + 1) * dy};
                if (!isValidPosition(landing)) break;
                if (!grid[landing.x][landing.y]) {
                    foundCapture = true;
                    path.push_back(landing);
                    captured.push_back(checkPos);
                    generateDameCaptureSequences(from, landing, path, captured, allMoves, 0, 0);
                    path.pop_back();
                    captured.pop_back();
                }
                break;
            }
            
            if (grid[checkPos.x][checkPos.y]) break;
        }
    }
    
    if (!foundCapture && !captured.empty()) {
        Move move{from, path, captured, piece->isBlackPiece()};
        allMoves.push_back(move);
    }
}

std::map<Position, std::vector<Move>> GameModel::getAllValidMoves() const {
    return getValidMovesForPlayer(isBlacksTurn);
}

std::map<Position, std::vector<Move>> GameModel::getValidMovesForPlayer(bool isBlack) const {
    std::map<Position, std::vector<Move>> allMoves;
    
    // Check if any piece can capture
    bool hasCaptures = false;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (grid[i][j] && grid[i][j]->isBlackPiece() == isBlack) {
                Position pos{i, j};
                std::vector<Move> captures = grid[i][j]->isDame()
                    ? generateDameCaptureMoves(pos)
                    : generatePionCaptureMoves(pos);
                
                if (!captures.empty()) {
                    hasCaptures = true;
                    allMoves[pos] = captures;
                }
            }
        }
    }
    
    if (hasCaptures) return allMoves;
    
    // No captures, return simple moves
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (grid[i][j] && grid[i][j]->isBlackPiece() == isBlack) {
                Position pos{i, j};
                std::vector<Move> moves = grid[i][j]->isDame()
                    ? generateDameSimpleMoves(pos)
                    : generatePionSimpleMoves(pos);
                
                if (!moves.empty()) {
                    allMoves[pos] = moves;
                }
            }
        }
    }
    return allMoves;
}

void GameModel::executeMove(const Move& move) {
    Piece* piece = grid[move.from.x][move.from.y].get();
    if (!piece) return;
    
    // Move piece to final position
    Position finalPos = move.path.back();
    grid[finalPos.x][finalPos.y] = std::move(grid[move.from.x][move.from.y]);
    piece = grid[finalPos.x][finalPos.y].get();
    piece->setPosition(finalPos);
    checkPromotion(finalPos);
    
    // Remove captured pieces
    for (const auto& capturedPos : move.captured) {
        grid[capturedPos.x][capturedPos.y].reset();
    }
    
    moveHistory.push_back(move);
    
    // Switch turns
    isBlacksTurn = !isBlacksTurn;
}

void GameModel::checkPromotion(const Position& pos) {
    Piece* piece = grid[pos.x][pos.y].get();
    if (!piece || piece->isDame()) return;
    
    bool shouldPromote = piece->isBlackPiece() ? (pos.x == 7) : (pos.x == 0);
    if (shouldPromote) piece->promote();
}

bool GameModel::isGameOver() const noexcept {
    if (getPieceCount(true) == 0 || getPieceCount(false) == 0) return true;
    
    // Check if current player has moves
    if (!getValidMovesForPlayer(isBlacksTurn).empty()) return false;
    
    // Current player has no moves, switch turn to opponent
    const_cast<GameModel*>(this)->isBlacksTurn = !isBlacksTurn;
    
    // Check if opponent has moves
    if (getValidMovesForPlayer(isBlacksTurn).empty()) {
        // Opponent also has no moves, game is over
        return true;
    }
    
    // Opponent has moves, game continues with opponent's turn
    return false;
}

std::string GameModel::getWinner() const noexcept {
    if (!isGameOver()) return "";
    // If a player has no pieces, the other wins
    if (getPieceCount(true) == 0) return "White";
    if (getPieceCount(false) == 0) return "Black";

    // Otherwise, winner is the opponent of the player who can't move
    return isBlacksTurn ? "White" : "Black";
}

int GameModel::getPieceCount(bool isBlack) const noexcept {
    int count = 0;
    for (const auto& row : grid) {
        for (const auto& piece : row) {
            if (piece && piece->isBlackPiece() == isBlack) count++;
        }
    }
    return count;
}

std::unique_ptr<GameModel> GameModel::clone() const {
    auto copy = std::make_unique<GameModel>();
    copy->isBlacksTurn = isBlacksTurn;
    copy->moveHistory = moveHistory;
    
    // Deep copy the grid
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (grid[i][j]) {
                copy->grid[i][j] = std::make_unique<Piece>(*grid[i][j]);
            }
        }
    }
    return copy;
}

// Overload to initialize from raw pointer grid
void GameModel::initializeFromGrid(const std::vector<std::vector<Piece*>>& rawGrid) {
    std::vector<std::vector<std::unique_ptr<Piece>>> initialGrid;
    initialGrid.resize(rawGrid.size());
    for (size_t i = 0; i < rawGrid.size(); ++i) {
        initialGrid[i].resize(rawGrid[i].size());
        for (size_t j = 0; j < rawGrid[i].size(); ++j) {
            if (rawGrid[i][j]) {
                initialGrid[i][j] = std::make_unique<Piece>(*rawGrid[i][j]);
            }
        }
    }
    initializeFromGrid(initialGrid);
}

// Provide access to the board grid as raw pointers
std::array<std::array<Piece*, GameModel::BOARD_SIZE>, GameModel::BOARD_SIZE> GameModel::getBoard() const noexcept {
    std::array<std::array<Piece*, GameModel::BOARD_SIZE>, GameModel::BOARD_SIZE> rawGrid{};
    for (size_t i = 0; i < GameModel::BOARD_SIZE; ++i) {
        for (size_t j = 0; j < GameModel::BOARD_SIZE; ++j) {
            rawGrid[i][j] = grid[i][j].get();
        }
    }
    return rawGrid;
}
