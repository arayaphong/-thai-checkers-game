#include "Board.h"
#include "KingPiece.h"
#include <iostream>
#include <vector>

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
    // Clear board
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
    // Set up initial pieces
    for (int i = 0; i < 2; ++i) {
        for (int j = (i % 2); j < 8; j += 2) {
            grid[i][j] = new Piece(player1Name, {i, j});
        }
    }
    for (int i = 6; i < 8; ++i) {
        for (int j = (i % 2); j < 8; j += 2) {
            grid[i][j] = new Piece(player2Name, {i, j});
        }
    }
}

// Overload initialize with default player names
void Board::initialize() {
    initialize("Player1", "Player2");
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
    return (startX >= 0 && startX < 8 && startY >= 0 && startY < 8 &&
            endX >= 0 && endX < 8 && endY >= 0 && endY < 8 &&
            grid[startX][startY] != nullptr);
}

bool Board::movePiece(int startX, int startY, int endX, int endY) {
    if (isValidMove(startX, startY, endX, endY)) {
        delete grid[endX][endY];
        grid[endX][endY] = grid[startX][startY];
        grid[startX][startY] = nullptr;

        // Check for promotion to King
        Piece* moved = grid[endX][endY];
        if (moved->getType() == Piece::Type::Regular) {
            const std::string& color = moved->getColor();
            // First player promotes at row 7, second at row 0
            if ((color == player1Name && endX == 7) || (color == player2Name && endX == 0)) {
                delete moved;
                grid[endX][endY] = new KingPiece(color, {endX, endY});
            }
        }

        return true;
    }
    return false;
}

// Get possible moves (normal and capture) for a piece at the given position
std::vector<Position> Board::getPossibleMoves(Position pos) const {
    std::vector<Position> moves;
    int x = pos.x;
    int y = pos.y;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return moves;
    Piece* p = grid[x][y];
    if (!p) return moves;

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

    for (auto [dx, dy] : dirs) {
        int nx = x + dx;
        int ny = y + dy;
        // Simple move
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            if (!grid[nx][ny]) {
                moves.push_back({nx, ny});
            } else if (grid[nx][ny]->getColor() != p->getColor()) {
                // Capture move
                int jx = x + 2*dx;
                int jy = y + 2*dy;
                if (jx >= 0 && jx < 8 && jy >= 0 && jy < 8 && !grid[jx][jy]) {
                    moves.push_back({jx, jy});
                }
            }
        }
    }
    return moves;
}