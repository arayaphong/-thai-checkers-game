#include "Grid.h"
#include <iostream>

void printBoardGrid(const Grid& grid) {
    // Column indices header
    std::cout << "   ";
    for (char col = 'A'; col <= 'H'; ++col) {
        std::cout << col << " ";
    }
    std::cout << "\n";

    // Iterate rows top-down
    for (int i = 0; i < 8; ++i) {
        std::cout << " " << i + 1 << " ";
        for (int j = 0; j < 8; ++j) {
            std::string symbol = " ";
            if ((i + j) % 2 == 1) symbol = ".";
            if (grid.isOccupied(i, j)) {
                // Defensive: check if piece pointer is not null
                const auto* piecePtr = &grid.getPiece(i, j);
                if (piecePtr) {
                    if (!piecePtr->isBlackPiece()) {
                        symbol = piecePtr->isDame() ? "■" : "●";
                    } else {
                        symbol = piecePtr->isDame() ? "□" : "○";
                    }
                } else {
                    symbol = "?"; // Should not happen, but safe fallback
                }
            }
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    auto grid = std::make_shared<Grid>();
    grid->clear();
    
    auto piece = grid->createPiece(false, Position{0, 0});
    printBoardGrid(*grid);

    return 0;
}
