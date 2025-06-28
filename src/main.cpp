#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include "GameScenario.h"

// Helper function to print the board
void printBoard(const std::array<std::array<Piece*, 8>, 8>& board) {
    // Column indices header
    std::cout << "   ";
    for (char col = 'A'; col <= 'H'; ++col) {
        std::cout << col << " ";
    }
    std::cout << "\n";

    // Iterate displayRow 0..7 top-down
    for (int i = 0; i < 8; ++i) {
        // Row index label (1..8 top-down)
        std::cout << " " << i + 1 << " ";
        for (int j = 0; j < 8; ++j) {
            std::string symbol = " ";
            if ((i + j) % 2 == 1) { // Dark squares
                symbol = ".";
            }
            if (board[i][j]) {
                if (board[i][j]->getColor() == "White") {
                    symbol = board[i][j]->isDame() ? "■" : "●";
                } else { // Black
                    symbol = board[i][j]->isDame() ? "□" : "○";
                }
            }
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
}

int main(int argc, char* argv[]) {
    GameScenario scenario;
    scenario.run();
    return 0;
}
