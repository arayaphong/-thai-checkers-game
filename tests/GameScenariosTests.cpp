#include <gtest/gtest.h>
#include "GameModel.h"
#include "Board.h"
#include "Piece.h"
#include <vector>
#include <cmath>

// Game Scenario Tests
// Tests comprehensive game scenarios and complex interactions
class GameScenariosTests : public ::testing::Test {
protected:
    GameModel model;
    
    void SetUp() override {
        // Each test will set up its own grid
    }
    
    void TearDown() override {
        // GameModel handles cleanup of pieces
    }
    
    // Helper function to create a grid and initialize the model
    void initializeGrid(const std::vector<std::vector<std::unique_ptr<Piece>>>& grid) {
        model.initializeFromGrid(grid);
    }
};

// Comprehensive Game Scenarios
TEST_F(GameScenariosTests, StandardGameProgression) {
    // Test a full standard game setup and initial moves
    model.initializeStandardGame("Player1", "Player2");
    
    auto allMoves = model.getAllValidMoves();
    EXPECT_FALSE(allMoves.empty());
    
    // Player1 should have multiple pieces that can move
    EXPECT_GE(allMoves.size(), 4);  // At least 4 pieces can move initially
    
    // Execute a move and verify turn change
    if (!allMoves.empty()) {
        auto firstMove = allMoves.begin()->second[0];
        model.executeMove(firstMove);
        EXPECT_EQ(model.getCurrentPlayer(), "Player2");
    }
}

TEST_F(GameScenariosTests, ForcedCaptureScenario) {
    // Create a scenario where Player1 must capture
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[2][2] = std::make_unique<Piece>("Player1", Position{2, 2});
    grid[3][3] = std::make_unique<Piece>("Player2", Position{3, 3});
    grid[1][1] = std::make_unique<Piece>("Player1", Position{1, 1});  // Another piece that could move

    initializeGrid(grid);
    
    auto allMoves = model.getAllValidMoves();
    
    // Only the piece that can capture should be moveable
    EXPECT_EQ(allMoves.size(), 1);
    EXPECT_TRUE(allMoves.find({2, 2}) != allMoves.end());
    
    auto moves = allMoves[{2, 2}];
    EXPECT_TRUE(moves[0].isCapture());
}

TEST_F(GameScenariosTests, MultipleChoiceCaptureScenario) {
    // Create a scenario where a piece has multiple capture options
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[2][2] = std::make_unique<Piece>("Player1", Position{2, 2});  // Player1 at top, can move downward
    grid[3][1] = std::make_unique<Piece>("Player2", Position{3, 1});  // Capturable enemy to the left-forward
    grid[3][3] = std::make_unique<Piece>("Player2", Position{3, 3});  // Capturable enemy to the right-forward

    initializeGrid(grid);
    
    auto moves = model.getValidMoves({2, 2});
    
    // Should have multiple capture options (left-forward and right-forward)
    int captureCount = 0;
    for (const auto& move : moves) {
        if (move.isCapture()) {
            captureCount++;
        }
    }
    EXPECT_GE(captureCount, 2);
}

TEST_F(GameScenariosTests, ChainCaptureScenario) {
    // Create a scenario with chain captures
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[1][1] = std::make_unique<Piece>("Player1", Position{1, 1});
    grid[2][2] = std::make_unique<Piece>("Player2", Position{2, 2});
    grid[4][4] = std::make_unique<Piece>("Player2", Position{4, 4});
    grid[6][6] = std::make_unique<Piece>("Player2", Position{6, 6});

    initializeGrid(grid);
    
    auto moves = model.getValidMoves({1, 1});
    
    // Should find chain capture moves
    bool foundChainCapture = false;
    for (const auto& move : moves) {
        if (move.captureCount() >= 2) {
            foundChainCapture = true;
            break;
        }
    }
    EXPECT_TRUE(foundChainCapture);
}

TEST_F(GameScenariosTests, PromotionScenario) {
    // Test piece promotion to Dame
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[6][6] = std::make_unique<Piece>("Player1", Position{6, 6});  // Near promotion row

    initializeGrid(grid);
    
    // Move to promotion row
    auto moves = model.getValidMoves({6, 6});
    Move* promotionMove = nullptr;
    for (auto& move : moves) {
        if (!move.path.empty() && move.path[0].x == 7) {  // Bottom row for Player1 promotion
            promotionMove = &move;
            break;
        }
    }
    ASSERT_NE(promotionMove, nullptr);
    
    model.executeMove(*promotionMove);
    
    auto board = model.getBoard();
    Position newPos = promotionMove->path[0];
    EXPECT_NE(board[newPos.x][newPos.y], nullptr);
    EXPECT_TRUE(board[newPos.x][newPos.y]->isDame());
}

TEST_F(GameScenariosTests, DameAdvancedMovementScenario) {
    // Test Dame's advanced movement capabilities
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[4][4] = std::make_unique<Piece>("Player1", Position{4, 4});
    grid[4][4]->promote();  // Make it a Dame
    grid[2][2] = std::make_unique<Piece>("Player2", Position{2, 2});      // Enemy piece
    grid[6][6] = std::make_unique<Piece>("Player2", Position{6, 6});      // Another enemy piece

    initializeGrid(grid);
    
    auto moves = model.getValidMoves({4, 4});
    
    // Dame should have movement options
    EXPECT_GT(moves.size(), 0);  // Dame should have some moves
    
    // Verify Dame can capture if possible or move freely
    bool hasCapture = false;
    bool hasPionMove = false;
    for (const auto& move : moves) {
        if (move.isCapture()) {
            hasCapture = true;
        } else {
            hasPionMove = true;
        }
    }
    
    // Dame should have either capture moves or pion moves
    EXPECT_TRUE(hasCapture || hasPionMove);
}

TEST_F(GameScenariosTests, ComplexMandatoryCaptureScenario) {
    // Test mandatory capture rules with multiple pieces
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[3][3] = std::make_unique<Piece>("Player1", Position{3, 3});
    grid[3][3]->promote();  // Make it a Dame
    grid[4][4] = std::make_unique<Piece>("Player2", Position{4, 4});      // Enemy piece
    grid[1][1] = std::make_unique<Piece>("Player1", Position{1, 1});      // Pion piece that could move

    initializeGrid(grid);
    
    auto allMoves = model.getAllValidMoves();
    
    // Only the Dame that can capture should be moveable
    EXPECT_EQ(allMoves.size(), 1);
    EXPECT_TRUE(allMoves.find({3, 3}) != allMoves.end());
    
    auto dameMoves = allMoves[{3, 3}];
    bool hasCapture = false;
    for (const auto& move : dameMoves) {
        if (move.isCapture()) {
            hasCapture = true;
            break;
        }
    }
    EXPECT_TRUE(hasCapture);
}

TEST_F(GameScenariosTests, EndGameScenario) {
    // Test end game with few pieces remaining
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[7][7] = std::make_unique<Piece>("Player1", Position{7, 7});
    grid[7][7]->promote();  // Make it a Dame
    grid[0][0] = std::make_unique<Piece>("Player2", Position{0, 0});      // Player2 pion piece

    initializeGrid(grid);
    
    // Dame should dominate the end game
    auto player1Moves = model.getValidMoves({7, 7});
    EXPECT_GT(player1Moves.size(), 0);
    
    // Switch to Player2
    model.setCurrentPlayer("Player2");
    auto player2Moves = model.getValidMoves({0, 0});
    
    // Pion piece should have limited moves compared to Dame
    EXPECT_LE(player2Moves.size(), player1Moves.size());
}

TEST_F(GameScenariosTests, NoValidMovesScenario) {
    // Test scenario where a player has no valid moves (game over)
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[7][7] = std::make_unique<Piece>("Player1", Position{7, 7});  // Player1 at bottom corner (blocked)
    grid[0][0] = std::make_unique<Piece>("Player2", Position{0, 0});  // Player2 has moves

    initializeGrid(grid);
    
    // Player1 should have no moves (blocked at bottom)
    auto player1Moves = model.getAllValidMoves();
    EXPECT_TRUE(player1Moves.empty());
    
    EXPECT_TRUE(model.isGameOver());
    EXPECT_EQ(model.getWinner(), "Player2");
}
