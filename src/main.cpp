#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include "Board.h"
#include "GameModel.h"
#include "Piece.h"

void runAnalyticsScenario(const std::string& scenarioName, 
                         const std::vector<std::vector<Piece*>>& grid,
                         bool showAllMoves = true) {
    std::cout << "\n=== " << scenarioName << " ===" << std::endl;
    
    // Create game model
    GameModel model;
    model.initializeFromGrid(grid);
    
    // Create board for display
    Board board;
    board.setModel(&model);
    board.display();
    
    // Get all valid moves
    auto allMoves = model.getAllValidMoves();
    
    std::cout << "\nAnalytics Report:" << std::endl;
    std::cout << "Current Player: " << model.getCurrentPlayer() << std::endl;
    std::cout << "Total Moveable Pieces: " << allMoves.size() << std::endl;
    
    if (showAllMoves) {
        std::cout << "\nAvailable Moves:" << std::endl;
        for (const auto& [pos, moves] : allMoves) {
            std::cout << "  Piece at (" << pos.x << "," << pos.y << "):" << std::endl;
            for (size_t i = 0; i < moves.size(); ++i) {
                const auto& move = moves[i];
                std::cout << "    Option " << i + 1 << ": ";
                for (const auto& step : move.path) {
                    std::cout << "(" << step.x << "," << step.y << ") ";
                }
                if (move.isCapture()) {
                    std::cout << "[captures " << move.captureCount() << " pieces]";
                }
                std::cout << std::endl;
            }
        }
    }
    
    // Execute first available move for demonstration
    if (!allMoves.empty()) {
        auto firstMove = allMoves.begin()->second[0];
        std::cout << "\nExecuting move: (" << firstMove.from.x << "," << firstMove.from.y << ") -> ";
        for (const auto& step : firstMove.path) {
            std::cout << "(" << step.x << "," << step.y << ") ";
        }
        std::cout << std::endl;
        
        model.executeMove(firstMove);
        std::cout << "\nBoard after move:" << std::endl;
        board.display();
        
        // Show game statistics
        std::cout << "\nGame Statistics:" << std::endl;
        std::cout << "  Player1 pieces: " << model.getPieceCount("Player1") << std::endl;
        std::cout << "  Player2 pieces: " << model.getPieceCount("Player2") << std::endl;
    }
}

void demonstratePionPieceMove() {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[3][3] = new Piece("Player1", {3, 3});
    runAnalyticsScenario("Pion Piece Move Demonstration", grid);
}

void demonstratePionPieceCapture() {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[3][3] = new Piece("Player2", {3, 3});
    runAnalyticsScenario("Pion Piece Capture Demonstration", grid);
}

void demonstratePiecePromotion() {
    std::cout << "\n=== Piece Promotion to Dame ===" << std::endl;
    std::cout << "Scenario: Pion piece reaches opponent's back row and gets promoted to Dame" << std::endl;
    
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    
    // Set up a pion piece close to promotion (Player1 moving toward row 7)
    grid[6][5] = new Piece("Player1", {6, 5}); // One move away from promotion
    
    // Add some context pieces but ensure promotion path is available
    grid[3][3] = new Piece("Player2", {3, 3}); // Opponent piece for context
    
    // Create game model and board
    GameModel model;
    model.initializeFromGrid(grid);
    Board board;
    board.setModel(&model);
    
    std::cout << "\nInitial board setup:" << std::endl;
    std::cout << "Player1 pion piece at (6,5) - one move away from promotion" << std::endl;
    board.display();
    
    std::cout << "\nPromotion Rules:" << std::endl;
    std::cout << "1. Pion piece promotes to Dame when reaching opponent's back row" << std::endl;
    std::cout << "2. For Player1: promotion occurs when reaching row 7 (bottom)" << std::endl;
    std::cout << "3. For Player2: promotion occurs when reaching row 0 (top)" << std::endl;
    std::cout << "4. After promotion, piece gains Dame movement and capturing abilities" << std::endl;
    std::cout << "5. Promoted Dame can immediately use all Dame powers" << std::endl;
    
    // Get all valid moves
    auto allMoves = model.getAllValidMoves();
    
    std::cout << "\nAnalysis of available moves:" << std::endl;
    for (const auto& [pos, moves] : allMoves) {
        if (pos.x == 6 && pos.y == 5) {
            std::cout << "Pion piece at (6,5) can:" << std::endl;
            for (size_t i = 0; i < moves.size(); ++i) {
                const auto& move = moves[i];
                std::cout << "  Option " << i + 1 << ": ";
                for (const auto& step : move.path) {
                    std::cout << "(" << step.x << "," << step.y << ") ";
                }
                
                // Check if this move leads to promotion
                if (!move.path.empty()) {
                    Position target = move.path.back();
                    if (target.x == 7) { // Reaching Player1's promotion row
                        std::cout << "[PROMOTION TO DAME!]";
                    }
                }
                
                if (move.isCapture()) {
                    std::cout << " [captures " << move.captureCount() << " pieces]";
                }
                std::cout << std::endl;
            }
        }
    }
    
    // Execute a promotion move if available
    bool promotionExecuted = false;
    for (const auto& [pos, moves] : allMoves) {
        if (pos.x == 6 && pos.y == 5) {
            for (const auto& move : moves) {
                if (!move.path.empty() && move.path.back().x == 7) {
                    std::cout << "\nExecuting promotion move:" << std::endl;
                    std::cout << "Pion piece moves from (6,5) to (" 
                              << move.path.back().x << "," << move.path.back().y << ")" << std::endl;
                    
                    model.executeMove(move);
                    promotionExecuted = true;
                    break;
                }
            }
            if (promotionExecuted) break;
        }
    }
    
    if (promotionExecuted) {
        std::cout << "\nBoard after promotion:" << std::endl;
        std::cout << "Notice: The piece is now a Dame (♛) with enhanced abilities!" << std::endl;
        board.display();
        
        // Show the Dame's new capabilities
        auto newMoves = model.getAllValidMoves();
        std::cout << "\nNew Dame capabilities analysis:" << std::endl;
        
        for (const auto& [pos, moves] : newMoves) {
            // Find the newly promoted Dame
            if (pos.x == 7) {
                std::cout << "Newly promoted Dame at (" << pos.x << "," << pos.y << ") can now:" << std::endl;
                std::cout << "  Total move options: " << moves.size() << std::endl;
                
                // Categorize moves
                int pionMoves = 0, captures = 0;
                int maxDistance = 0;
                
                for (const auto& move : moves) {
                    if (move.isCapture()) {
                        captures++;
                    } else {
                        pionMoves++;
                        // Calculate distance for regular moves
                        if (!move.path.empty()) {
                            int distance = abs(move.path.back().x - pos.x);
                            maxDistance = std::max(maxDistance, distance);
                        }
                    }
                }
                
                std::cout << "  Pion moves: " << pionMoves 
                          << " (max distance: " << maxDistance << " squares)" << std::endl;
                std::cout << "  Capture options: " << captures << std::endl;
                std::cout << "  Can move backward: YES (Dame ability)" << std::endl;
                std::cout << "  Can move unlimited diagonal distance: YES (Dame ability)" << std::endl;
                
                // Show some specific moves
                std::cout << "\n  Sample Dame moves:" << std::endl;
                int count = 0;
                for (const auto& move : moves) {
                    if (count >= 5) break; // Show first 5 moves
                    std::cout << "    Move to: ";
                    for (const auto& step : move.path) {
                        std::cout << "(" << step.x << "," << step.y << ") ";
                    }
                    if (move.isCapture()) {
                        std::cout << "[captures " << move.captureCount() << " pieces]";
                    }
                    std::cout << std::endl;
                    count++;
                }
                break;
            }
        }
    } else {
        std::cout << "\nNo promotion move available in this setup." << std::endl;
    }
    
    // Clean up memory
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
}

void demonstrateDamePieceMove() {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[3][3] = new Piece("Player1", {3, 3}, Piece::Type::Dame);
    runAnalyticsScenario("Dame Piece Move Demonstration", grid);
}

void demonstrateDamePieceCapture() {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2}, Piece::Type::Dame);
    grid[3][3] = new Piece("Player2", {3, 3});
    grid[5][5] = new Piece("Player2", {5, 5});
    runAnalyticsScenario("Dame Piece Capture Demonstration", grid);
}

void demonstrateDameMultipleCaptures() {
    std::cout << "\n=== Dame Multiple Captures - First Empty Square Rule ===" << std::endl;
    std::cout << "Scenario: Dame must capture and land on first empty square after each captured piece" << std::endl;
    
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    
    // Set up Dame at position (1,1)
    grid[1][1] = new Piece("Player1", {1, 1}, Piece::Type::Dame);
    
    // Set up opponent pieces for multiple captures
    grid[2][2] = new Piece("Player2", {2, 2}); // First capture target
    grid[4][4] = new Piece("Player2", {4, 4}); // Second capture target
    grid[6][6] = new Piece("Player2", {6, 6}); // Third capture target
    
    // Create game model and board
    GameModel model;
    model.initializeFromGrid(grid);
    Board board;
    board.setModel(&model);
    
    std::cout << "\nInitial board setup:" << std::endl;
    board.display();
    
    std::cout << "\nDame Rules Demonstration:" << std::endl;
    std::cout << "1. Dame can move diagonally forward/backward any number of empty squares" << std::endl;
    std::cout << "2. When capturing, Dame MUST jump over adjacent opponent and land on FIRST empty square" << std::endl;
    std::cout << "3. Multiple captures are mandatory if possible" << std::endl;
    std::cout << "4. Each jump must land on the first empty square after the captured piece" << std::endl;
    
    // Get all valid moves for the Dame
    auto allMoves = model.getAllValidMoves();
    
    std::cout << "\nAnalytics Report:" << std::endl;
    std::cout << "Current Player: " << model.getCurrentPlayer() << std::endl;
    std::cout << "Dame at position (1,1) can:" << std::endl;
    
    for (const auto& [pos, moves] : allMoves) {
        if (pos.x == 1 && pos.y == 1) {
            for (size_t i = 0; i < moves.size(); ++i) {
                const auto& move = moves[i];
                std::cout << "  Option " << i + 1 << ": ";
                for (const auto& step : move.path) {
                    std::cout << "(" << step.x << "," << step.y << ") ";
                }
                if (move.isCapture()) {
                    std::cout << "[captures " << move.captureCount() << " pieces]";
                }
                std::cout << std::endl;
            }
        }
    }
    
    // Clean up memory
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
}

void demonstrateDameMandatoryCapture() {
    std::cout << "\n=== Dame Mandatory Capture Rule ===" << std::endl;
    std::cout << "Scenario: Dame must capture when possible - cannot choose to move without capturing" << std::endl;
    
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    
    // Set up Dame with both regular move and capture options
    grid[3][3] = new Piece("Player1", {3, 3}, Piece::Type::Dame);
    grid[4][4] = new Piece("Player2", {4, 4}); // Must capture this
    
    // Create game model and board
    GameModel model;
    model.initializeFromGrid(grid);
    Board board;
    board.setModel(&model);
    
    std::cout << "\nBoard setup - Dame has both move and capture options:" << std::endl;
    board.display();
    
    auto allMoves = model.getAllValidMoves();
    
    std::cout << "\nRule verification:" << std::endl;
    std::cout << "Even though Dame could move to many empty squares," << std::endl;
    std::cout << "it MUST capture the opponent piece when possible." << std::endl;
    
    std::cout << "\nAvailable moves (should only show captures):" << std::endl;
    for (const auto& [pos, moves] : allMoves) {
        if (pos.x == 3 && pos.y == 3) {
            for (size_t i = 0; i < moves.size(); ++i) {
                const auto& move = moves[i];
                std::cout << "  Move " << i + 1 << ": ";
                for (const auto& step : move.path) {
                    std::cout << "(" << step.x << "," << step.y << ") ";
                }
                if (move.isCapture()) {
                    std::cout << "[MANDATORY CAPTURE - " << move.captureCount() << " pieces]";
                } else {
                    std::cout << "[Regular move - Should NOT appear if capture available]";
                }
                std::cout << std::endl;
            }
        }
    }
    
    // Clean up memory
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
}

void demonstrateDameUnlimitedMovement() {
    std::cout << "\n=== Dame Unlimited Diagonal Movement ===" << std::endl;
    std::cout << "Scenario: Dame can move any number of empty squares diagonally in all four directions" << std::endl;
    
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    
    // Place Dame at center of board
    grid[3][3] = new Piece("Player1", {3, 3}, Piece::Type::Dame);
    
    // Place some pieces to show movement boundaries (but not blocking all paths)
    grid[6][0] = new Piece("Player2", {6, 0}); // Far opponent piece
    grid[1][7] = new Piece("Player1", {1, 7}); // Own piece to show blocking
    
    // Create game model and board
    GameModel model;
    model.initializeFromGrid(grid);
    Board board;
    board.setModel(&model);
    
    std::cout << "\nBoard setup - Dame at center with minimal obstacles:" << std::endl;
    board.display();
    
    std::cout << "\nDame Movement Rules:" << std::endl;
    std::cout << "1. Can move diagonally forward OR backward in all four directions" << std::endl;
    std::cout << "2. No limit to number of empty squares in a single move" << std::endl;
    std::cout << "3. Cannot jump over own pieces" << std::endl;
    std::cout << "4. Can move to squares adjacent to opponent pieces (but not capture unless jumping)" << std::endl;
    std::cout << "5. Must stop before hitting any piece (own or opponent)" << std::endl;
    
    // Get all valid moves for the Dame
    auto allMoves = model.getAllValidMoves();
    
    std::cout << "\nAnalysis of Dame movement from position (3,3):" << std::endl;
    
    int totalMoves = 0;
    for (const auto& [pos, moves] : allMoves) {
        if (pos.x == 3 && pos.y == 3) {
            totalMoves = moves.size();
            std::cout << "\nDame can move to " << totalMoves << " different positions:" << std::endl;
            
            // Group moves by direction for better analysis
            std::vector<Position> upRight, upLeft, downRight, downLeft;
            
            for (const auto& move : moves) {
                if (!move.isCapture() && !move.path.empty()) {
                    Position target = move.path.back();
                    if (target.x > 3 && target.y > 3) {
                        downRight.push_back(target);
                    } else if (target.x > 3 && target.y < 3) {
                        downLeft.push_back(target);
                    } else if (target.x < 3 && target.y > 3) {
                        upRight.push_back(target);
                    } else if (target.x < 3 && target.y < 3) {
                        upLeft.push_back(target);
                    }
                }
            }
            
            std::cout << "\n  Down-Right diagonal: ";
            if (downRight.empty()) {
                std::cout << "BLOCKED or no valid moves";
            } else {
                for (const auto& pos : downRight) {
                    std::cout << "(" << pos.x << "," << pos.y << ") ";
                }
            }
            
            std::cout << "\n  Down-Left diagonal: ";
            if (downLeft.empty()) {
                std::cout << "BLOCKED or no valid moves";
            } else {
                for (const auto& pos : downLeft) {
                    std::cout << "(" << pos.x << "," << pos.y << ") ";
                }
            }
            
            std::cout << "\n  Up-Right diagonal: ";
            if (upRight.empty()) {
                std::cout << "BLOCKED or no valid moves";
            } else {
                for (const auto& pos : upRight) {
                    std::cout << "(" << pos.x << "," << pos.y << ") ";
                }
            }
            
            std::cout << "\n  Up-Left diagonal: ";
            if (upLeft.empty()) {
                std::cout << "BLOCKED or no valid moves";
            } else {
                for (const auto& pos : upLeft) {
                    std::cout << "(" << pos.x << "," << pos.y << ") ";
                }
            }
            
            // Check for captures
            std::cout << "\n\n  Available captures: ";
            bool hasCaptures = false;
            for (const auto& move : moves) {
                if (move.isCapture() && !move.path.empty()) {
                    Position target = move.path.back();
                    std::cout << "(" << target.x << "," << target.y << ") ";
                    std::cout << "[captures " << move.captureCount() << " pieces] ";
                    hasCaptures = true;
                }
            }
            if (!hasCaptures) {
                std::cout << "None available";
            }
            std::cout << std::endl;
        }
    }
    
    // Demonstrate a long-range move
    if (!allMoves.empty()) {
        auto damePos = Position{3, 3};
        auto dameMoves = allMoves[damePos];
        
        // Find the longest non-capture move
        auto longestMove = std::max_element(dameMoves.begin(), dameMoves.end(),
            [](const auto& a, const auto& b) {
                if (a.isCapture() != b.isCapture()) return a.isCapture();
                return a.path.size() < b.path.size();
            });
        
        if (longestMove != dameMoves.end() && !longestMove->isCapture()) {
            std::cout << "\nDemonstrating long-range movement:" << std::endl;
            std::cout << "Dame moves from (3,3) to (" << longestMove->path.back().x 
                      << "," << longestMove->path.back().y << ")" << std::endl;
            
            model.executeMove(*longestMove);
            std::cout << "\nBoard after long-range move:" << std::endl;
            board.display();
        }
    }
    
    // Clean up memory
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
}

void demonstrateSwitchingTurnMove() {
    std::cout << "\n=== Switching Turn Move Demonstration ===" << std::endl;
    std::cout << "Scenario: Demonstrating how turns alternate between players after each move" << std::endl;
    
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    
    // Set up a simple scenario with pieces from both players
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[2][4] = new Piece("Player1", {2, 4});
    grid[5][3] = new Piece("Player2", {5, 3});
    grid[5][5] = new Piece("Player2", {5, 5});
    
    // Create game model and board
    GameModel model;
    model.initializeFromGrid(grid);
    Board board;
    board.setModel(&model);
    
    std::cout << "\nInitial board setup:" << std::endl;
    std::cout << "Player1 pieces at (2,2) and (2,4)" << std::endl;
    std::cout << "Player2 pieces at (5,3) and (5,5)" << std::endl;
    board.display();
    
    std::cout << "\nTurn Management Rules:" << std::endl;
    std::cout << "1. Game starts with Player1's turn" << std::endl;
    std::cout << "2. After each move execution, turn switches to the other player" << std::endl;
    std::cout << "3. Players cannot move opponent's pieces" << std::endl;
    std::cout << "4. Players can only see and execute moves for their own pieces" << std::endl;
    
    // Demonstrate several turns
    for (int turn = 1; turn <= 4; ++turn) {
        std::cout << "\n--- Turn " << turn << " ---" << std::endl;
        std::cout << "Current Player: " << model.getCurrentPlayer() << std::endl;
        
        // Get all valid moves for current player
        auto allMoves = model.getAllValidMoves();
        
        if (allMoves.empty()) {
            std::cout << "No valid moves available for " << model.getCurrentPlayer() << std::endl;
            break;
        }
        
        std::cout << "Available pieces that can move:" << std::endl;
        for (const auto& [pos, moves] : allMoves) {
            std::cout << "  Piece at (" << pos.x << "," << pos.y << ") - " 
                      << moves.size() << " possible moves" << std::endl;
        }
        
        // Execute the first available move
        auto firstPiecePos = allMoves.begin()->first;
        auto firstMove = allMoves.begin()->second[0];
        
        std::cout << "\nExecuting move:" << std::endl;
        std::cout << "  " << model.getCurrentPlayer() << " moves piece from (" 
                  << firstMove.from.x << "," << firstMove.from.y << ") to (";
        
        if (!firstMove.path.empty()) {
            auto target = firstMove.path.back();
            std::cout << target.x << "," << target.y << ")";
            
            if (firstMove.isCapture()) {
                std::cout << " [captures " << firstMove.captureCount() << " pieces]";
            }
        }
        std::cout << std::endl;
        
        // Store current player before move execution for comparison
        std::string playerBeforeMove{model.getCurrentPlayer()};
        
        // Execute the move
        model.executeMove(firstMove);
        
        std::cout << "\nAfter move execution:" << std::endl;
        std::cout << "  Previous player: " << playerBeforeMove << std::endl;
        std::cout << "  Current player: " << model.getCurrentPlayer() << std::endl;
        std::cout << "  Turn successfully switched: " 
                  << (playerBeforeMove != model.getCurrentPlayer() ? "YES" : "NO") << std::endl;
        
        board.display();
        
        // Show piece counts
        std::cout << "Current game state:" << std::endl;
        std::cout << "  Player1 pieces: " << model.getPieceCount("Player1") << std::endl;
        std::cout << "  Player2 pieces: " << model.getPieceCount("Player2") << std::endl;
    }
    
    std::cout << "\nMove History Summary:" << std::endl;
    auto moveHistory = model.getMoveHistory();
    for (size_t i = 0; i < moveHistory.size(); ++i) {
        const auto& move = moveHistory[i];
        std::string player = (i % 2 == 0) ? "Player1" : "Player2";
        std::cout << "  Move " << (i + 1) << " (" << player << "): (" 
                  << move.from.x << "," << move.from.y << ") -> ";
        
        if (!move.path.empty()) {
            auto target = move.path.back();
            std::cout << "(" << target.x << "," << target.y << ")";
            if (move.isCapture()) {
                std::cout << " [captured " << move.captureCount() << " pieces]";
            }
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nDemonstration completed successfully!" << std::endl;
    std::cout << "Key observations:" << std::endl;
    std::cout << "- Each player can only move their own pieces" << std::endl;
    std::cout << "- Turn automatically switches after each valid move" << std::endl;
    std::cout << "- Game maintains proper turn order throughout play" << std::endl;
    std::cout << "- Move history tracks the sequence of all executed moves" << std::endl;
    
    // Clean up memory
    for (auto& row : grid) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Thai Checkers Analytics System" << std::endl;
    std::cout << "==============================" << std::endl;

    std::string pieceType = "";
    std::string scenario = "";

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--piece-type" && i + 1 < argc) {
                pieceType = argv[++i];
            } else if (arg == "--scenario" && i + 1 < argc) {
                scenario = argv[++i];
            }
        }
    }

    if (pieceType == "pion" && scenario == "move") {
        demonstratePionPieceMove();
    } else if (pieceType == "pion" && scenario == "capture") {
        demonstratePionPieceCapture();
    } else if (pieceType == "pion" && scenario == "promotion") {
        demonstratePiecePromotion();
    } else if (pieceType == "dame" && scenario == "move") {
        demonstrateDamePieceMove();
    } else if (pieceType == "dame" && scenario == "capture") {
        demonstrateDamePieceCapture();
    } else if (pieceType == "dame" && scenario == "unlimited-movement") {
        demonstrateDameUnlimitedMovement();
    } else if (pieceType == "dame" && scenario == "multiple-capture") {
        demonstrateDameMultipleCaptures();
    } else if (pieceType == "dame" && scenario == "mandatory-capture") {
        demonstrateDameMandatoryCapture();
    } else if (scenario == "switching-turns") {
        demonstrateSwitchingTurnMove();
    } else {
        std::cout << "\nUsage: " << argv[0] << " --piece-type <pion|dame> --scenario <move|capture|promotion|unlimited-movement|multiple-capture|mandatory-capture|switching-turns>" << std::endl;
        std::cout << "Running all demonstrations by default:" << std::endl;
        demonstratePionPieceMove();
        demonstratePionPieceCapture();
        demonstratePiecePromotion();
        demonstrateDamePieceMove();
        demonstrateDamePieceCapture();
        demonstrateDameUnlimitedMovement();
        demonstrateDameMultipleCaptures();
        demonstrateDameMandatoryCapture();
        demonstrateSwitchingTurnMove();
    }

    return 0;
}
