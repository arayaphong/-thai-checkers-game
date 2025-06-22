#include "Board.h"
#include "KingPiece.h"
#include <iostream>
#include <vector>
#include <functional>
#include <set> // Add include for set to detect player names

Board::Board() {
    grid.resize(8, std::vector<Piece*>(8, nullptr));
}

Board::~Board() {
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
        }
    }
}

void Board::initialize(const std::vector<std::vector<Piece*>>& initialGrid) {
    clearBoard();
    grid = initialGrid;
    // detect player names for custom grid initialization
    std::set<std::string> names;
    for (const auto& row : grid) {
        for (const auto& p : row) {
            if (p) names.insert(p->getColor());
        }
    }
    auto it = names.begin();
    if (names.size() >= 1) {
        player1Name = *it;
        ++it;
    }
    if (names.size() >= 2) {
        player2Name = *it;
    }
    currentPlayer = player1Name;
}

void Board::initialize(const std::string& p1, const std::string& p2) {
    // Store player names
    player1Name = p1;
    player2Name = p2;
    currentPlayer = player1Name;
    clearBoard();

    // Initialize the board with 8:8 pieces each
    grid.resize(8, std::vector<Piece*>(8, nullptr));

    // place pieces for player 1 (X)
    for (int row = 0; row < 2; ++row) {
        for (int col = (row % 2); col < 8; col += 2) {
            grid[row][col] = new Piece(player1Name, {row, col});
        }
    }

    // place pieces for player 2 (O)
    for (int row = 6; row < 8; ++row) {
        for (int col = (row % 2); col < 8; col += 2) {
            grid[row][col] = new Piece(player2Name, {row, col});
        }
    }
}

void Board::clearBoard()
{
    for (auto &row : grid)
    {
        for (auto &piece : row)
        {
            delete piece;
            piece = nullptr;
        }
    }
}

void Board::display() const {
    // Print column indices
    std::cout << "  ";
    for (int j = 0; j < 8; ++j) {
        std::cout << j << " ";
    }
    std::cout << std::endl;
    // Print each row with row index
    for (int i = 0; i < 8; ++i) {
        std::cout << i << " ";
        for (const auto& piece : grid[i]) {
            if (piece) {
                char symbol = (piece->getColor() == player1Name) ? 'X' : 'O';
                std::cout << symbol << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<Position>> Board::getTargetPositions(const Piece& piece) const {
    Position pos = piece.getPosition();
    std::vector<std::vector<Position>> allMoves;
    int x = pos.x;
    int y = pos.y;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return allMoves;
    Piece* p = grid[x][y];
    if (!p) return allMoves;

    // Determine movement directions
    std::vector<std::pair<int,int>> dirs;
    if (p->getType() == Piece::Type::Regular) {
        if (p->getColor() == player1Name) {
            dirs = {{1, 1}, {1, -1}};
        } else if (p->getColor() == player2Name) {
            dirs = {{-1, 1}, {-1, -1}};
        }
    } else {
        // King moves both directions
        dirs = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    }

    // Simple moves (single-step)
    for (auto [dx, dy] : dirs) {
        int nx = x + dx;
        int ny = y + dy;
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && !grid[nx][ny]) {
            allMoves.push_back({{nx, ny}});
        }
    }

    // Capture moves (including chains)
    std::vector<Position> path;
    std::vector<std::vector<Position>> capturePaths;
    auto gridCopy = grid; // copy board state for simulation
    std::function<void(int,int)> dfs = [&](int cx, int cy) {
        bool foundCapture = false;
        for (auto [dx, dy] : dirs) {
            int mx = cx + dx;
            int my = cy + dy;
            int lx = cx + 2*dx;
            int ly = cy + 2*dy;
            if (mx >= 0 && mx < 8 && my >= 0 && my < 8 &&
                lx >= 0 && lx < 8 && ly >= 0 && ly < 8 &&
                gridCopy[mx][my] && gridCopy[mx][my]->getColor() != p->getColor() &&
                !gridCopy[lx][ly]) {

                foundCapture = true;
                // simulate capture
                Piece* captured = gridCopy[mx][my];
                gridCopy[cx][cy] = nullptr;
                gridCopy[mx][my] = nullptr;
                gridCopy[lx][ly] = p;
                path.push_back({lx, ly});

                dfs(lx, ly);

                // restore state
                path.pop_back();
                gridCopy[cx][cy] = p;
                gridCopy[mx][my] = captured;
                gridCopy[lx][ly] = nullptr;
            }
        }
        if (!foundCapture && !path.empty()) {
            capturePaths.push_back(path);
        }
    };
    dfs(x, y);

    // Add capture paths after simple moves
    for (auto& seq : capturePaths) {
        allMoves.push_back(seq);
    }
    return allMoves;
}

// Get all pieces that can move for a specific player
std::vector<Piece> Board::getMoveablePieces(const std::string& playerName) const {
    std::vector<Piece> movablePieces;
    
    // Iterate through all positions on the board
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Piece* piece = grid[x][y];
            
            // Check if there's a piece at this position and it belongs to the specified player
            if (piece && piece->getColor() == playerName) {            // Check if this piece has any possible moves
            if (!getTargetPositions(*piece).empty()) {
                movablePieces.push_back(*piece);
                }
            }
        }
    }
    
    return movablePieces;
}

std::string Board::getCurrentPlayer() const {
    return currentPlayer;
}