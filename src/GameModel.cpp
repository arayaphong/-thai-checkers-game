#include "GameModel.h"
#include <algorithm>
#include <set>

namespace {
    constexpr int BOARD_SIZE = 8;
    
    const std::vector<std::pair<int, int>> DIAGONAL_DIRECTIONS = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
}

GameModel::GameModel() : currentPlayer(""), player1Name(""), player2Name("") {
    grid.resize(BOARD_SIZE, std::vector<Piece*>(BOARD_SIZE, nullptr));
}

GameModel::~GameModel() {
    clearGrid();
}

void GameModel::clearGrid() {
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
}

bool GameModel::isValidPosition(const Position& pos) const {
    return pos.x >= 0 && pos.x < BOARD_SIZE && pos.y >= 0 && pos.y < BOARD_SIZE;
}

bool GameModel::isPlayer1(const std::string& player) const {
    return player == player1Name;
}

bool GameModel::isPlayer2(const std::string& player) const {
    return player == player2Name;
}

std::string GameModel::getOpponent(const std::string& player) const {
    return isPlayer1(player) ? player2Name : player1Name;
}

void GameModel::initializeStandardGame(const std::string& player1, const std::string& player2) {
    player1Name = player1;
    player2Name = player2;
    clearGrid();
    
    // Place player1 pieces (top of board)
    for (int row = 0; row < 2; ++row) {
        for (int col = (row % 2); col < BOARD_SIZE; col += 2) {
            grid[row][col] = new Piece(player1, {row, col});
        }
    }
    
    // Place player2 pieces (bottom of board)
    for (int row = 6; row < BOARD_SIZE; ++row) {
        for (int col = (row % 2); col < BOARD_SIZE; col += 2) {
            grid[row][col] = new Piece(player2, {row, col});
        }
    }
    
    currentPlayer = player1;
}

void GameModel::setCurrentPlayer(const std::string& player) {
    currentPlayer = player;
}

void GameModel::initializeFromGrid(const std::vector<std::vector<Piece*>>& initialGrid) {
    clearGrid();
    
    // Collect unique player names from the grid
    std::set<std::string> uniquePlayers;
    
    // Copy the grid and collect players
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (initialGrid[i][j]) {
                grid[i][j] = new Piece(*initialGrid[i][j]);
                uniquePlayers.insert(grid[i][j]->getColor());
            }
        }
    }
    
    // Convert to sorted vector for consistent ordering
    std::vector<std::string> playerList(uniquePlayers.begin(), uniquePlayers.end());
    std::sort(playerList.begin(), playerList.end());
    
    // Assign player names - prioritize common test names
    if (playerList.size() >= 1) {
        // If we have Player1 in the list, make it player1Name
        auto player1Iter = std::find(playerList.begin(), playerList.end(), "Player1");
        if (player1Iter != playerList.end()) {
            player1Name = "Player1";
            playerList.erase(player1Iter);
        } else {
            player1Name = playerList[0];
            playerList.erase(playerList.begin());
        }
    }
    
    if (playerList.size() >= 1) {
        // If we have Player2 in the list, make it player2Name
        auto player2Iter = std::find(playerList.begin(), playerList.end(), "Player2");
        if (player2Iter != playerList.end()) {
            player2Name = "Player2";
        } else if (!playerList.empty()) {
            player2Name = playerList[0];
        }
    }
    
    // Set current player to player1Name, or first player found
    currentPlayer = !player1Name.empty() ? player1Name : "DefaultPlayer";
}

bool GameModel::canAnyPieceCapture() const {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (grid[i][j] && grid[i][j]->getColor() == currentPlayer) {
                std::vector<Move> captures = grid[i][j]->isDame() 
                    ? generateDameCaptureMoves({i, j})
                    : generateCaptureMoves({i, j});
                if (!captures.empty()) return true;
            }
        }
    }
    return false;
}

std::vector<Move> GameModel::getValidMoves(const Position& piecePos) const {
    if (!isValidPosition(piecePos)) return {};
    
    Piece* piece = grid[piecePos.x][piecePos.y];
    if (!piece || piece->getColor() != currentPlayer) return {};
    
    // Mandatory capture rule: if any piece can capture, only return captures
    if (canAnyPieceCapture()) {
        return piece->isDame() 
            ? generateDameCaptureMoves(piecePos)
            : generateCaptureMoves(piecePos);
    }
    
    // No captures available, return simple moves
    return piece->isDame() 
        ? generateDameSimpleMoves(piecePos)
        : generateSimpleMoves(piecePos);
}

std::vector<Move> GameModel::generateSimpleMoves(const Position& from) const {
    std::vector<Move> moves;
    Piece* piece = grid[from.x][from.y];
    if (!piece) return moves;
    
    int direction = isPlayer1(piece->getColor()) ? 1 : -1;
    
    // Check diagonal moves
    for (int dy : {-1, 1}) {
        Position dest{from.x + direction, from.y + dy};
        if (isValidPosition(dest) && !grid[dest.x][dest.y]) {
            Move move{from, {dest}, {}, piece->getColor()};
            moves.push_back(move);
        }
    }
    
    return moves;
}

std::vector<Move> GameModel::generateDameSimpleMoves(const Position& from) const {
    std::vector<Move> moves;
    Piece* piece = grid[from.x][from.y];
    if (!piece || !piece->isDame()) return moves;
    
    for (const auto& [dx, dy] : DIAGONAL_DIRECTIONS) {
        for (int dist = 1; dist < BOARD_SIZE; ++dist) {
            Position dest{from.x + dist * dx, from.y + dist * dy};
            
            if (!isValidPosition(dest) || grid[dest.x][dest.y]) break;
            
            Move move{from, {dest}, {}, piece->getColor()};
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
    
    for (const auto& [dx, dy] : DIAGONAL_DIRECTIONS) {
        Position enemy{current.x + dx, current.y + dy};
        Position landing{current.x + 2*dx, current.y + 2*dy};
        
        if (!isValidPosition(landing)) continue;
        if (!grid[enemy.x][enemy.y] || grid[enemy.x][enemy.y]->getColor() == piece->getColor()) continue;
        if (grid[landing.x][landing.y]) continue;
        if (std::find(captured.begin(), captured.end(), enemy) != captured.end()) continue;
        
        foundCapture = true;
        
        path.push_back(landing);
        captured.push_back(enemy);
        
        generateCaptureSequences(from, landing, path, captured, allMoves);
        
        path.pop_back();
        captured.pop_back();
    }
    
    if (!foundCapture && !captured.empty()) {
        Move move{from, path, captured, piece->getColor()};
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
                                           std::vector<Move>& allMoves,
                                           int initialDx, int initialDy) const {
    Piece* piece = grid[from.x][from.y];
    if (!piece || !piece->isDame()) return;
    
    bool foundCapture = false;
    
    for (const auto& [dx, dy] : DIAGONAL_DIRECTIONS) {
        for (int dist = 1; dist < BOARD_SIZE; ++dist) {
            Position checkPos{current.x + dist * dx, current.y + dist * dy};
            
            if (!isValidPosition(checkPos)) break;
            
            // Found enemy piece
            if (grid[checkPos.x][checkPos.y] && grid[checkPos.x][checkPos.y]->getColor() != piece->getColor()) {
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
        Move move{from, path, captured, piece->getColor()};
        allMoves.push_back(move);
    }
}

std::map<Position, std::vector<Move>> GameModel::getAllValidMoves() const {
    std::map<Position, std::vector<Move>> allMoves;
    
    // Check if any piece can capture
    bool hasCaptures = false;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (grid[i][j] && grid[i][j]->getColor() == currentPlayer) {
                Position pos{i, j};
                std::vector<Move> captures = grid[i][j]->isDame() 
                    ? generateDameCaptureMoves(pos)
                    : generateCaptureMoves(pos);
                
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
            if (grid[i][j] && grid[i][j]->getColor() == currentPlayer) {
                Position pos{i, j};
                std::vector<Move> moves = grid[i][j]->isDame() 
                    ? generateDameSimpleMoves(pos)
                    : generateSimpleMoves(pos);
                
                if (!moves.empty()) {
                    allMoves[pos] = moves;
                }
            }
        }
    }
    
    return allMoves;
}

void GameModel::executeMove(const Move& move) {
    Piece* piece = grid[move.from.x][move.from.y];
    if (!piece) return;
    
    // Move piece to final position
    Position finalPos = move.path.back();
    grid[finalPos.x][finalPos.y] = piece;
    grid[move.from.x][move.from.y] = nullptr;
    piece->setPosition(finalPos);
    
    checkPromotion(finalPos);
    
    // Remove captured pieces
    for (const auto& capturedPos : move.captured) {
        delete grid[capturedPos.x][capturedPos.y];
        grid[capturedPos.x][capturedPos.y] = nullptr;
    }
    
    moveHistory.push_back(move);
    
    // Switch turns
    currentPlayer = getOpponent(currentPlayer);
}

void GameModel::checkPromotion(const Position& pos) {
    Piece* piece = grid[pos.x][pos.y];
    if (!piece || piece->isDame()) return;
    
    bool shouldPromote = isPlayer1(piece->getColor()) ? (pos.x == 7) : (pos.x == 0);
    if (shouldPromote) piece->promote();
}

bool GameModel::isGameOver() const {
    return getAllValidMoves().empty();
}

std::string GameModel::getWinner() const {
    if (!isGameOver()) return "";
    return getOpponent(currentPlayer);
}

int GameModel::getPieceCount(const std::string& player) const {
    int count = 0;
    for (const auto& row : grid) {
        for (const auto& piece : row) {
            if (piece && piece->getColor() == player) count++;
        }
    }
    return count;
}

GameModel* GameModel::clone() const {
    GameModel* copy = new GameModel();
    copy->currentPlayer = currentPlayer;
    copy->player1Name = player1Name;
    copy->player2Name = player2Name;
    copy->moveHistory = moveHistory;
    
    // Deep copy the grid
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (grid[i][j]) {
                copy->grid[i][j] = new Piece(*grid[i][j]);
            }
        }
    }
    
    return copy;
}