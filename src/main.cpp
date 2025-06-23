        #include <iostream>
        #include "Board.h"
        #include "Piece.h"
        #include "States.h"

        int main() {
            // Scenario: Simple Move with States API
            {
                std::cout << "--- Simple Move via States ---" << std::endl;
                std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
                grid[3][3] = new Piece("Player1", {3, 3});
                Board board;
                board.initialize(grid);
                board.display();

                // Generate possible states
                States states = board.getStates(*grid[3][3]);
                auto choices = states.getChoices();
                std::cout << "Choices: ";
                for (auto& path : choices) {
                    std::cout << "[";
                    for (auto& pos : path) std::cout << "(" << pos.x << "," << pos.y << ")";
                    std::cout << "] ";
                }
                std::cout << std::endl;

                // Apply first choice
                if (!choices.empty()) {
                    std::cout << "Applying choice 0" << std::endl;
                    states.selectMove(choices[0]);
                    board.display();
                }
            }

            // Scenario: Capture Move with States API
            {
                std::cout << "\n--- Capture Move via States ---" << std::endl;
                std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
                grid[2][2] = new Piece("Player1", {2, 2});
                grid[3][3] = new Piece("Player2", {3, 3});
                Board board;
                board.initialize(grid);
                board.display();

                // Generate possible states
                States states = board.getStates(*grid[2][2]);
                auto choices = states.getChoices();
                std::cout << "Choices: ";
                for (auto& path : choices) {
                    std::cout << "[";
                    for (auto& pos : path) std::cout << "(" << pos.x << "," << pos.y << ")";
                    std::cout << "] ";
                }
                std::cout << std::endl;

                // Apply first choice
                if (!choices.empty()) {
                    std::cout << "Applying choice 0" << std::endl;
                    states.selectMove(choices[0]);
                    board.display();
                }
            }

            // Scenario: Multiple Capture Chain via States API
            {
                std::cout << "\n--- Multiple Capture Chain via States ---" << std::endl;
                std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
                grid[2][2] = new Piece("Player1", {2, 2});
                grid[3][3] = new Piece("Player2", {3, 3});
                grid[5][5] = new Piece("Player2", {5, 5});
                Board board;
                board.initialize(grid);
                board.display();

                // Generate possible states
                States states = board.getStates(*grid[2][2]);
                auto choices = states.getChoices();
                std::cout << "Choices: ";
                for (auto& path : choices) {
                    std::cout << "[";
                    for (auto& pos : path) std::cout << "(" << pos.x << "," << pos.y << ")";
                    std::cout << "] ";
                }
                std::cout << std::endl;

                if (!choices.empty()) {
                    std::cout << "Applying chain capture" << std::endl;
                    // Select the longest capture path
                    auto best = choices[0];
                    for (auto& path : choices)
                        if (path.size() > best.size()) best = path;
                    states.selectMove(best);
                    board.display();
                }
            }

            // Scenario: Multi-Capture Choices via States API
            {
                std::cout << "\n--- Multi-Capture Choices via States ---" << std::endl;
                std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
                grid[2][2] = new Piece("Player1", {2, 2});
                grid[3][1] = new Piece("Player2", {3, 1});
                grid[3][3] = new Piece("Player2", {3, 3});
                Board board;
                board.initialize(grid);
                board.display();

                // Generate possible capture choices
                States states = board.getStates(*grid[2][2]);
                auto choices = states.getChoices();
                std::cout << "Choices: ";
                for (auto& path : choices) {
                    std::cout << "[";
                    for (auto& pos : path) std::cout << "(" << pos.x << "," << pos.y << ")";
                    std::cout << "] ";
                }
                std::cout << std::endl;

                // Apply each choice on a fresh board
                for (size_t i = 0; i < choices.size(); ++i) {
                    std::cout << "Applying choice " << i << std::endl;
                    // Deep copy the grid to avoid modifying the original state
                    std::vector<std::vector<Piece*>> copyGrid(8, std::vector<Piece*>(8, nullptr));
                    for(int r=0; r<8; ++r) {
                        for (int c=0; c<8; ++c) {
                            if (grid[r][c]) {
                                copyGrid[r][c] = new Piece(*grid[r][c]);
                            }
                        }
                    }

                    Board tempBoard;
                    tempBoard.initialize(copyGrid);
                    // The piece pointer is invalid in the new board, so we need to get it again
                    States tempStates = tempBoard.getStates(*copyGrid[2][2]);
                    auto tempChoices = tempStates.getChoices();
                    // The order of choices might differ, so we find the matching one
                    for (const auto& tempChoice : tempChoices) {
                        if (tempChoice == choices[i]) {
                            tempStates.selectMove(tempChoice);
                            break;
                        }
                    }
                    tempBoard.display();
                }
            }

            // Scenario: Chain Capture with Multiple Choices via States API
            {
                std::cout << "\n--- Chain Capture with Multiple Choices via States ---" << std::endl;
                std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
                grid[0][0] = new Piece("Player1", {0, 0});
                grid[1][1] = new Piece("Player2", {1, 1});
                grid[3][3] = new Piece("Player2", {3, 3});
                grid[3][1] = new Piece("Player2", {3, 1});
                Board board;
                board.initialize(grid);
                board.display();

                // Generate possible states
                States states = board.getStates(*grid[0][0]);
                auto choices = states.getChoices();
                std::cout << "Choices: ";
                for (auto& path : choices) {
                    std::cout << "[";
                    for (auto& pos : path) std::cout << "(" << pos.x << "," << pos.y << ")";
                    std::cout << "] ";
                }
                std::cout << std::endl;

                // Apply each choice on a fresh board
                for (size_t i = 0; i < choices.size(); ++i) {
                    std::cout << "Applying choice " << i << std::endl;
                    // Deep copy the grid to avoid modifying the original state
                    std::vector<std::vector<Piece*>> copyGrid(8, std::vector<Piece*>(8, nullptr));
                    for(int r=0; r<8; ++r) {
                        for (int c=0; c<8; ++c) {
                            if (grid[r][c]) {
                                copyGrid[r][c] = new Piece(*grid[r][c]);
                            }
                        }
                    }

                    Board tempBoard;
                    tempBoard.initialize(copyGrid);
                    // The piece pointer is invalid in the new board, so we need to get it again
                    States tempStates = tempBoard.getStates(*copyGrid[0][0]);
                    auto tempChoices = tempStates.getChoices();
                    // The order of choices might differ, so we find the matching one
                    for (const auto& tempChoice : tempChoices) {
                        if (tempChoice == choices[i]) {
                            tempStates.selectMove(tempChoice);
                            break;
                        }
                    }
                    tempBoard.display();
                }
            }

            // Scenario: No Moves Available
            {
                std::cout << "\n--- No Moves Available ---\n" << std::endl;
                std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
                grid[4][4] = new Piece("Player1", {4, 4});
                grid[5][5] = new Piece("Player1", {5, 5});
                grid[5][3] = new Piece("Player2", {5, 3});
                grid[6][2] = new Piece("Player2", {6, 2});
                
                Board board;
                board.initialize(grid);
                board.display();

                // Generate possible states for the piece at (4,4)
                States states = board.getStates(*grid[4][4]);
                auto choices = states.getChoices();
                
                if (choices.empty()) {
                    std::cout << "No moves available for the piece at (4,4), as expected." << std::endl;
                } else {
                    std::cout << "Error: Moves were found when none were expected." << std::endl;
                }
            }

            return 0;
        }
