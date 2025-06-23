#include "GameModel.h"
#include <algorithm>
#include <set>

GameModel::GameModel() : currentPlayer("Player1") {
    grid.resize(8, std::vector<Piece*>(8, nullptr));
}

GameModel::~GameModel() {
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
        }
    }
}

void GameModel::initializeStandardGame(const std::string& player1, const std::string& player2) {
    // Clear existing pieces
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
    
    // Place player1 pieces (top of board)
    for (int row = 0; row < 2; ++row) {
        for (int col = (row % 2); col < 8; col += 2) {
            grid[row][col] = new Piece(player1, {row, col});
        }
    }
    
    // Place player2 pieces (bottom of board)
    for (int row = 6; row < 8; ++row) {
        for (int col = (row % 2); col < 8; col += 2) {
            grid[row][col] = new Piece(player2, {row, col});
        }
    }
    
    currentPlayer = player1;
}

void GameModel::setCurrentPlayer(const std::string& player) {
    currentPlayer = player;
}

void GameModel::initializeFromGrid(const std::vector<std::vector<Piece*>>& initialGrid) {
    // Clear existing pieces
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
    
    // Copy the grid
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (initialGrid[i][j]) {
                grid[i][j] = new Piece(*initialGrid[i][j]);
            }
        }
    }
    
    // Default to Player1's turn
    currentPlayer = "Player1";
}

std::vector<Move> GameModel::getValidMoves(const Position& piecePos) const {
    if (piecePos.x < 0 || piecePos.x >= 8 || piecePos.y < 0 || piecePos.y >= 8) {
        return {};
    }
    
    Piece* piece = grid[piecePos.x][piecePos.y];
    if (!piece || piece->getColor() != currentPlayer) {
        return {};
    }
    
    std::vector<Move> moves;
    
    // Check for capture moves first (mandatory in checkers)
    std::vector<Move> captureMoves = generateCaptureMoves(piecePos);
    if (!captureMoves.empty()) {
        return captureMoves;
    }
    
    // If no captures available, return simple moves
    return generateSimpleMoves(piecePos);
}

std::vector<Move> GameModel::generateSimpleMoves(const Position& from) const {
    std::vector<Move> moves;
    Piece* piece = grid[from.x][from.y];
    if (!piece) return moves;
    
    // Determine move direction based on player
    int direction = (piece->getColor() == "Player1" || piece->getColor() == "P1") ? 1 : -1;
    
    // Check diagonal moves
    std::vector<Position> destinations = {
        {from.x + direction, from.y - 1},
        {from.x + direction, from.y + 1}
    };
    
    for (const auto& dest : destinations) {
        if (dest.x >= 0 && dest.x < 8 && dest.y >= 0 && dest.y < 8 && !grid[dest.x][dest.y]) {
            Move move;
            move.from = from;
            move.path = {dest};
            move.player = piece->getColor();
            moves.push_back(move);
        }
    }
    
    return moves;
}

std::vector<Move> GameModel::generateCaptureMoves(const Position& from) const {
    std::vector<Move> allMoves;
    std::vector<Position> path;
    std::vector<Position> captured;
    
    generateCaptureSequences(from, from, path, captured, allMoves);
    
    return allMoves;
}

void GameModel::generateCaptureSequences(const Position& from, const Position& current,
                                       std::vector<Position>& path,
                                       std::vector<Position>& captured,
                                       std::vector<Move>& allMoves) const {
    Piece* piece = grid[from.x][from.y];
    if (!piece) return;
    
    bool foundCapture = false;
    
    // Check all four diagonal directions
    std::vector<std::pair<int, int>> directions = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    
    for (const auto& [dx, dy] : directions) {
        Position enemy{current.x + dx, current.y + dy};
        Position landing{current.x + 2*dx, current.y + 2*dy};
        
        // Check bounds
        if (landing.x < 0 || landing.x >= 8 || landing.y < 0 || landing.y >= 8) continue;
        
        // Check if there's an enemy piece to capture
        if (!grid[enemy.x][enemy.y] || grid[enemy.x][enemy.y]->getColor() == piece->getColor()) continue;
        
        // Check if landing position is empty
        if (grid[landing.x][landing.y]) continue;
        
        // Check if we haven't already captured this piece
        if (std::find(captured.begin(), captured.end(), enemy) != captured.end()) continue;
        
        foundCapture = true;
        
        // Add this capture to the path
        path.push_back(landing);
        captured.push_back(enemy);
        
        // Continue searching for more captures
        generateCaptureSequences(from, landing, path, captured, allMoves);
        
        // Backtrack
        path.pop_back();
        captured.pop_back();
    }
    
    // If no more captures found and we've captured at least one piece, save this move
    if (!foundCapture && !captured.empty()) {
        Move move;
        move.from = from;
        move.path = path;
        move.captured = captured;
        move.player = piece->getColor();
        allMoves.push_back(move);
    }
}

std::map<Position, std::vector<Move>> GameModel::getAllValidMoves() const {
    std::map<Position, std::vector<Move>> allMoves;
    
    // First check if any piece can capture
    bool hasCaptures = false;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] && grid[i][j]->getColor() == currentPlayer) {
                Position pos{i, j};
                auto captures = generateCaptureMoves(pos);
                if (!captures.empty()) {
                    hasCaptures = true;
                    allMoves[pos] = captures;
                }
            }
        }
    }
    
    // If captures exist, only return capture moves (mandatory in checkers)
    if (hasCaptures) {
        return allMoves;
    }
    
    // Otherwise, return all simple moves
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] && grid[i][j]->getColor() == currentPlayer) {
                Position pos{i, j};
                auto moves = generateSimpleMoves(pos);
                if (!moves.empty()) {
                    allMoves[pos] = moves;
                }
            }
        }
    }
    
    return allMoves;
}

void GameModel::executeMove(const Move& move) {
    // Move the piece
    Piece* piece = grid[move.from.x][move.from.y];
    if (!piece) return;
    
    // Update piece position
    Position finalPos = move.path.back();
    grid[finalPos.x][finalPos.y] = piece;
    grid[move.from.x][move.from.y] = nullptr;
    piece->setPosition(finalPos);
    
    // Remove captured pieces
    for (const auto& capturedPos : move.captured) {
        delete grid[capturedPos.x][capturedPos.y];
        grid[capturedPos.x][capturedPos.y] = nullptr;
    }
    
    // Add to history
    moveHistory.push_back(move);
    
    // Switch turns
    currentPlayer = (currentPlayer == "Player1" || currentPlayer == "P1") ? "Player2" : "Player1";
    if (currentPlayer == "P2") currentPlayer = "P1"; // Handle P1/P2 naming
}

bool GameModel::isGameOver() const {
    return getAllValidMoves().empty();
}

std::string GameModel::getWinner() const {
    if (!isGameOver()) return "";
    
    // If current player has no moves, the other player wins
    return (currentPlayer == "Player1" || currentPlayer == "P1") ? "Player2" : "Player1";
}

int GameModel::getPieceCount(const std::string& player) const {
    int count = 0;
    for (const auto& row : grid) {
        for (const auto& piece : row) {
            if (piece && piece->getColor() == player) {
                count++;
            }
        }
    }
    return count;
}

GameModel* GameModel::clone() const {
    GameModel* copy = new GameModel();
    copy->currentPlayer = currentPlayer;
    copy->moveHistory = moveHistory;
    
    // Deep copy the grid
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j]) {
                copy->grid[i][j] = new Piece(*grid[i][j]);
            }
        }
    }
    
    return copy;
}

void GameModel::undoLastMove() {
    if (moveHistory.empty()) return;
    
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();
    
    // Move piece back
    Piece* piece = grid[lastMove.path.back().x][lastMove.path.back().y];
    if (piece) {
        grid[lastMove.from.x][lastMove.from.y] = piece;
        grid[lastMove.path.back().x][lastMove.path.back().y] = nullptr;
        piece->setPosition(lastMove.from);
        
        // Restore captured pieces
        for (const auto& capturedPos : lastMove.captured) {
            // We need to know the color of captured pieces - this is a limitation
            // For now, assume opposite of the player who made the move
            std::string capturedColor = (lastMove.player == "Player1" || lastMove.player == "P1") ? "Player2" : "Player1";
            grid[capturedPos.x][capturedPos.y] = new Piece(capturedColor, capturedPos);
        }
        
        // Switch back turns
        currentPlayer = lastMove.player;
    }
}

double GameModel::evaluatePosition(const std::string& player) const {
    // Simple evaluation function for analytics
    double score = 0.0;
    
    // Count pieces
    int myPieces = getPieceCount(player);
    std::string opponent = (player == "Player1" || player == "P1") ? "Player2" : "Player1";
    int oppPieces = getPieceCount(opponent);
    
    score += (myPieces - oppPieces) * 10.0;
    
    // Add positional bonuses
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j] && grid[i][j]->getColor() == player) {
                // Prefer advanced positions
                if (player == "Player1" || player == "P1") {
                    score += i * 0.5;
                } else {
                    score += (7 - i) * 0.5;
                }
            }
        }
    }
    
    // Invert score for Player2 to ensure symmetry
    if (player == "Player2" || player == "P2") {
        return -score;
    }
    return score;
}

std::vector<Move> GameModel::getAllPossibleMoves() const {
    std::vector<Move> moves;
    auto movesMap = getAllValidMoves();
    
    for (const auto& [pos, posMoves] : movesMap) {
        moves.insert(moves.end(), posMoves.begin(), posMoves.end());
    }
    
    return moves;
}
