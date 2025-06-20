#include <iostream>
#include "THCheckers.h"

int main() {
    THCheckers game;
    game.startGame();
    game.displayBoard();

    while (!game.checkWinCondition()) {
        game.makeMove();
        game.displayBoard();
    }

    std::cout << "Game Over!" << std::endl;
    return 0;
}