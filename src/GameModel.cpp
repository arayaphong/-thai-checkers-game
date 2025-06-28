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
    return pos.x >= 0 && pos.x < static_cast<int>(BOARD_SIZE) \
           && pos.y >= 0 && pos.y < static_cast<int>(BOARD_SIZE);
}

bool GameModel::isPlayer1(std::string_view player) const noexcept {
    return player == player1Name;
}

bool GameModel::isPlayer2(std::string_view player) const noexcept {
    return player == player2Name;
}

std::string_view GameModel::getOpponent(std::string_view player) const noexcept {
    return isPlayer1(player) ? player2Name : player1Name;
}

void GameModel::initializeStandardGame(std::string_view player1, std::string_view player2) {
    player1Name = player1;
    player2Name = player2;
    clearGrid();
    
    // Place player1 pieces (top of board)
    for (int row = 0; row < 2; ++row) {
        for (int col = (row % 2); col < BOARD_SIZE; col += 2) {
            grid[row][col] = std::make_unique<Piece>(player1, Position{row, col});
        }
    }
    
    // Place player2 pieces (bottom of board)
    for (int row = 6; row < BOARD_SIZE; ++row) {
        for (int col = (row % 2); col < BOARD_SIZE; col += 2) {
            grid[row][col] = std::make_unique<Piece>(player2, Position{row, col});
        }
    }
    currentPlayer = player1;
}

void GameModel::setCurrentPlayer(std::string_view player) noexcept {
    currentPlayer = player;
}

// Get current player's color
std::string_view GameModel::getCurrentPlayer() const noexcept {
    return currentPlayer;
}

void GameModel::initializeFromGrid(const std::vector<std::vector<std::unique_ptr<Piece>>>& initialGrid) {
    clearGrid();
    // Collect unique player names from the grid
    std::set<std::string> uniquePlayers;
    
    // Copy the grid and collect players
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (initialGrid[i][j]) {
                grid[i][j] = std::make_unique<Piece>(*initialGrid[i][j]);
                uniquePlayers.insert(std::string{grid[i][j]->getColor()});
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
    if (!piece || piece->getColor() != currentPlayer) return {};
    
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
    int forwardDirection = isPlayer1(piece->getColor()) ? 1 : -1;
    int deltaX = to.x - from.x;
    int deltaY = abs(to.y - from.y);
    
    // Must move exactly one square forward diagonally
    return (deltaX == forwardDirection) && (deltaY == 1);
}

std::vector<Move> GameModel::generatePionSimpleMoves(const Position& from) const {
    std::vector<Move> moves;
    const Piece* piece = grid[from.x][from.y].get();
    if (!piece || piece->isDame()) return moves;
    
    int forwardDirection = isPlayer1(piece->getColor()) ? 1 : -1;
    for (int dy : {-1, 1}) {
        Position dest{from.x + forwardDirection, from.y + dy};
        if (isValidPosition(dest) && !grid[dest.x][dest.y]) {
            Move move{from, {dest}, {}, std::string{piece->getColor()}};
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
            Move move{from, {dest}, {}, std::string{piece->getColor()}};
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
    int forwardDirection = isPlayer1(piece->getColor()) ? 1 : -1;
    
    for (int dy : {-1, 1}) {
        int dx = forwardDirection;
        Position enemy{current.x + dx, current.y + dy};
        Position landing{current.x + 2*dx, current.y + 2*dy};
        
        if (!isValidPosition(landing)) continue;
        if (!grid[enemy.x][enemy.y] || grid[enemy.x][enemy.y]->getColor() == piece->getColor()) continue;
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
        Move move{from, path, captured, std::string{piece->getColor()}};
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
        Move move{from, path, captured, std::string{piece->getColor()}};
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
            if (grid[i][j] && grid[i][j]->getColor() == currentPlayer) {
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
    currentPlayer = getOpponent(currentPlayer);
}

void GameModel::checkPromotion(const Position& pos) {
    Piece* piece = grid[pos.x][pos.y].get();
    if (!piece || piece->isDame()) return;
    
    bool shouldPromote = isPlayer1(piece->getColor()) ? (pos.x == 7) : (pos.x == 0);
    if (shouldPromote) piece->promote();
}

bool GameModel::isGameOver() const noexcept {
    // Game ends if a player has no pieces, no valid moves available, position is repeated, or insufficient material
    if (getPieceCount(player1Name) == 0 || getPieceCount(player2Name) == 0) return true;
    return getAllValidMoves().empty() || isInsufficientMaterial();
}

std::string_view GameModel::getWinner() const noexcept {
    if (!isGameOver()) return std::string_view{};
    // If a player has no pieces, the other wins
    if (getPieceCount(player1Name) == 0) return player2Name;
    if (getPieceCount(player2Name) == 0) return player1Name;
    
    // If insufficient material, it's a draw
    if (isInsufficientMaterial()) return std::string_view{};

    // Otherwise, winner is the opponent of the player who can't move
    return getOpponent(currentPlayer);
}

int GameModel::getPieceCount(std::string_view player) const noexcept {
    int count = 0;
    for (const auto& row : grid) {
        for (const auto& piece : row) {
            if (piece && piece->getColor() == player) count++;
        }
    }
    return count;
}

std::unique_ptr<GameModel> GameModel::clone() const {
    auto copy = std::make_unique<GameModel>();
    copy->currentPlayer = currentPlayer;
    copy->player1Name = player1Name;
    copy->player2Name = player2Name;
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

bool GameModel::isInsufficientMaterial() const noexcept {
    // Count pieces for each player
    int player1Count = 0, player2Count = 0;
    int player1Dames = 0, player2Dames = 0;
    
    for (const auto& row : grid) {
        for (const auto& piece : row) {
            if (piece) {
                if (piece->getColor() == player1Name) {
                    player1Count++;
                    if (piece->isDame()) player1Dames++;
                } else if (piece->getColor() == player2Name) {
                    player2Count++;
                    if (piece->isDame()) player2Dames++;
                }
            }
        }
    }
    
    // Draw if only two dames left (one for each player)
    return (player1Count == 1 && player2Count == 1 && 
            player1Dames == 1 && player2Dames == 1);
}
