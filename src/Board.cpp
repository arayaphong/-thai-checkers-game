#include "Board.h"
#include "KingPiece.h"
#include <iostream>
#include <vector>
#include <functional>

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

void Board::initialize(const std::string& p1, const std::string& p2) {
    // Store player names
    player1Name = p1;
    player2Name = p2;
    currentPlayer = player1Name;
    // Clear board
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
    // Set up initial pieces with a lambda (no OpenMP)
    auto place_pieces = [&](int row_start, int row_end, const std::string& player) {
        for (int i = row_start; i < row_end; ++i) {
            for (int j = (i % 2); j < 8; j += 2) {
                grid[i][j] = new Piece(player, {i, j});
            }
        }
    };
    place_pieces(0, 2, player1Name);
    place_pieces(6, 8, player2Name);
}

void Board::display() const {
    for (const auto& row : grid) {
        for (const auto& piece : row) {
            if (piece) {
                // Display X for first player and O for second
                char symbol = (piece->getColor() == player1Name) ? 'X' : 'O';
                std::cout << symbol << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

bool Board::isValidMove(int startX, int startY, int endX, int endY) const {
    if (startX < 0 || startX >= 8 || startY < 0 || startY >= 8 ||
        endX < 0 || endX >= 8 || endY < 0 || endY >= 8) {
        return false;
    }

    Piece* piece = grid[startX][startY];
    if (!piece || piece->getColor() != currentPlayer) {
        return false;
    }

    if (grid[endX][endY] != nullptr) {
        return false; // Destination must be empty
    }

    int dx = endX - startX;
    int dy = endY - startY;

    if (piece->getType() == Piece::Type::Regular) {
        int forward_dir = (piece->getColor() == player1Name) ? 1 : -1;
        if (abs(dx) == 1 && dx == forward_dir && abs(dy) == 1) {
            return true; // Regular move
        }
        if (abs(dx) == 2 && dx == 2 * forward_dir && abs(dy) == 2) {
            Piece* captured = grid[startX + dx / 2][startY + dy / 2];
            if (captured && captured->getColor() != piece->getColor()) {
                return true; // Capture
            }
        }
    } else { // King
        if (abs(dx) == abs(dy)) {
            // Check for blocking pieces along the path for non-capture moves
            if (abs(dx) == 1) return true;

            // Check for captures
            int stepX = dx / abs(dx);
            int stepY = dy / abs(dy);
            int steps = abs(dx);
            int piecesToJump = 0;
            Piece* captured = nullptr;
            for (int i = 1; i < steps; ++i) {
                Piece* p = grid[startX + i * stepX][startY + i * stepY];
                if (p) {
                    piecesToJump++;
                    captured = p;
                }
            }
            if (piecesToJump == 1 && captured && captured->getColor() != piece->getColor()) {
                return true;
            }
        }
    }

    return false;
}

bool Board::movePiece(int startX, int startY, int endX, int endY) {
    if (isValidMove(startX, startY, endX, endY)) {
        int dx = endX - startX;
        // If it's a capture move, remove the captured piece
        if (abs(dx) == 2) {
            int capturedX = startX + dx / 2;
            int capturedY = startY + (endY - startY) / 2;
            delete grid[capturedX][capturedY];
            grid[capturedX][capturedY] = nullptr;
        }

        grid[endX][endY] = grid[startX][startY];
        grid[startX][startY] = nullptr;
        grid[endX][endY]->setPosition({endX, endY});

        // Check for promotion to King
        Piece* moved = grid[endX][endY];
        if (moved->getType() == Piece::Type::Regular) {
            const std::string& color = moved->getColor();
            if ((color == player1Name && endX == 7) || (color == player2Name && endX == 0)) {
                delete moved;
                grid[endX][endY] = new KingPiece(color, {endX, endY});
            }
        }
        
        // Switch turns
        currentPlayer = (currentPlayer == player1Name) ? player2Name : player1Name;

        return true;
    }
    return false;
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