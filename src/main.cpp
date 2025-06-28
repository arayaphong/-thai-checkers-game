#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include "GameModel.h"

// Helper function to print the board
void printBoard(const std::array<std::array<Piece*, 8>, 8>& board) {
    std::cout << "=== GAME BOARD ===" << std::endl;
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
                    symbol = board[i][j]->isDame() ? "▲" : "●";
                } else { // Black
                    symbol = board[i][j]->isDame() ? "△" : "○";
                }
            }
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\nLegend: ●=White Pion, ▲=White Dame, ○=Black Pion, △=Black Dame\n\n";
}

int main(int argc, char* argv[]) {
    GameModel game;
    game.initializeStandardGame("White", "Black");

    while (!game.isGameOver()) {
        std::cout << "Current player: " << game.getCurrentPlayer() << std::endl;
        std::cout << "Game Stage: " << game.getMoveHistory().size() << std::endl;
        printBoard(game.getBoard());

        auto allMoves = game.getAllValidMoves();
        if (allMoves.empty()) {
            std::cout << "No valid moves for " << game.getCurrentPlayer() << std::endl;
            break;
        }

        // Select the first valid move
        const Move& move = allMoves.begin()->second.front();
        
        std::cout << "Executing move from: "
                  << (char)('A' + move.from.x) << move.from.y + 1 << " -> "
                  << (char)('A' + move.path.back().x) << move.path.back().y + 1 << std::endl;

        game.executeMove(move);
        std::cout << "-------------------------" << std::endl;
    }

    std::cout << "Game over!" << std::endl;
    std::cout << "Game Stage: " << game.getMoveHistory().size() << std::endl;
    if (game.getWinner() != "") {
        std::cout << "Winner: " << game.getWinner() << std::endl;
    } else {
        std::cout << "It's a draw!" << std::endl;
    }
    printBoard(game.getBoard());

    return 0;
}
