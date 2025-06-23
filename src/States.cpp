#include "States.h"
#include "Board.h"

// Apply a selected path back to the board, moving the piece and removing captures
void States::selectMove(const std::vector<Position>& path) {
    if (!board || !piece || path.empty()) return;
    Position from = piece->getPosition();
    int x0 = from.x;
    int y0 = from.y;
    // extract the pointer to the actual Piece
    Piece* p = board->grid[x0][y0];
    if (!p) return;
    // Remove piece from original position
    board->grid[x0][y0] = nullptr;
    // Move through the path
    for (const auto& pos : path) {
        int nx = pos.x, ny = pos.y;
        // If this is a capture, remove the captured piece
        if (std::abs(nx - x0) == 2 && std::abs(ny - y0) == 2) {
            int capX = (x0 + nx) / 2;
            int capY = (y0 + ny) / 2;
            delete board->grid[capX][capY];
            board->grid[capX][capY] = nullptr;
        }
        x0 = nx; y0 = ny;
    }
    // Place piece at final position
    board->grid[x0][y0] = p;
    p->setPosition({x0, y0});
}
