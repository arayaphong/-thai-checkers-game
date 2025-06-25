#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include "Board.h"
#include "GameModel.h"
#include "Piece.h"

int main(int argc, char* argv[]) {
    Board board;
    board.initialize("Player1", "Player2");
    int step = 1;
    while (true) {
        std::cout << "\nStep " << step << ": Player " << board.getCurrentPlayer() << "'s turn" << std::endl;
        board.display();
        auto allMoves = board.getAllValidMoves();
        if (allMoves.empty()) {
            std::cout << "Game over! Winner: " << (board.getCurrentPlayer().empty() ? "None" : board.getCurrentPlayer()) << std::endl;
            break;
        }
        // Pick the first piece with valid moves
        auto it = allMoves.begin();
        const Position& pos = it->first;
        const std::vector<Move>& moves = it->second;
        if (moves.empty()) {
            std::cout << "No valid moves for selected piece. Skipping..." << std::endl;
            break;
        }
        const Move& move = moves[0];
        std::cout << "Executing move from (" << move.from.x << "," << move.from.y << ") to (" << move.path.back().x << "," << move.path.back().y << ")" << std::endl;
        board.executeMove(move);
        step++;
        // Optional: add a pause or limit steps for demo
        if (step > 100) break;
    }
    return 0;
}