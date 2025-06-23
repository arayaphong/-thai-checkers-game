#include <iostream>
#include <iomanip>
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
        std::cout << "  Position evaluation (Player1): " << std::fixed << std::setprecision(2) 
                  << model.evaluatePosition("Player1") << std::endl;
    }
}

int main() {
    std::cout << "Thai Checkers Analytics System" << std::endl;
    std::cout << "==============================" << std::endl;
    
    // Scenario 1: Simple Move Analysis
    {
        std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
        grid[3][3] = new Piece("Player1", {3, 3});
        runAnalyticsScenario("Simple Move Analysis", grid);
    }
    
    // Scenario 2: Capture Move Analysis
    {
        std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
        grid[2][2] = new Piece("Player1", {2, 2});
        grid[3][3] = new Piece("Player2", {3, 3});
        runAnalyticsScenario("Capture Move Analysis", grid);
    }
    
    // Scenario 3: Multiple Capture Chain Analysis
    {
        std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
        grid[2][2] = new Piece("Player1", {2, 2});
        grid[3][3] = new Piece("Player2", {3, 3});
        grid[5][5] = new Piece("Player2", {5, 5});
        runAnalyticsScenario("Multiple Capture Chain Analysis", grid);
    }
    
    // Scenario 4: Multi-Choice Capture Analysis
    {
        std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
        grid[2][2] = new Piece("Player1", {2, 2});
        grid[3][1] = new Piece("Player2", {3, 1});
        grid[3][3] = new Piece("Player2", {3, 3});
        runAnalyticsScenario("Multi-Choice Capture Analysis", grid);
    }
    
    // Scenario 5: Complex Board State Analysis
    {
        std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
        grid[0][0] = new Piece("Player1", {0, 0});
        grid[1][1] = new Piece("Player2", {1, 1});
        grid[3][3] = new Piece("Player2", {3, 3});
        grid[3][1] = new Piece("Player2", {3, 1});
        runAnalyticsScenario("Complex Board State Analysis", grid);
    }
    
    // Scenario 6: Game Simulation
    {
        std::cout << "\n=== Full Game Simulation ===" << std::endl;
        GameModel game;
        game.initializeStandardGame("Player1", "Player2");
        
        Board board;
        board.setModel(&game);
        
        int moveCount = 0;
        const int maxMoves = 10;
        
        while (!game.isGameOver() && moveCount < maxMoves) {
            std::cout << "\nMove " << (moveCount + 1) << " - " << game.getCurrentPlayer() << "'s turn" << std::endl;
            
            auto moves = game.getAllPossibleMoves();
            if (moves.empty()) break;
            
            // Simple strategy: pick the first available move
            auto selectedMove = moves[0];
            
            // Prefer captures if available
            for (const auto& move : moves) {
                if (move.isCapture() && move.captureCount() > selectedMove.captureCount()) {
                    selectedMove = move;
                }
            }
            
            game.executeMove(selectedMove);
            moveCount++;
            
            std::cout << "Moved from (" << selectedMove.from.x << "," << selectedMove.from.y << ") to (";
            std::cout << selectedMove.path.back().x << "," << selectedMove.path.back().y << ")";
            if (selectedMove.isCapture()) {
                std::cout << " [captured " << selectedMove.captureCount() << "]";
            }
            std::cout << std::endl;
        }
        
        std::cout << "\nFinal Board State:" << std::endl;
        board.display();
        
        std::cout << "\nGame Summary:" << std::endl;
        std::cout << "  Total moves: " << moveCount << std::endl;
        std::cout << "  Player1 pieces: " << game.getPieceCount("Player1") << std::endl;
        std::cout << "  Player2 pieces: " << game.getPieceCount("Player2") << std::endl;
        
        if (game.isGameOver()) {
            std::cout << "  Winner: " << game.getWinner() << std::endl;
        }
    }
    
    return 0;
}
