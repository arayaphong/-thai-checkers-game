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
    model.initializeStandardGame();
    
    auto allMoves = model.getAllValidMoves();
    EXPECT_FALSE(allMoves.empty());
    
    // Player1 should have multiple pieces that can move
    EXPECT_GE(allMoves.size(), 4);  // At least 4 pieces can move initially
    
    // Execute a move and verify turn change
    if (!allMoves.empty()) {
        auto firstMove = allMoves.begin()->second[0];
        model.executeMove(firstMove);
        EXPECT_FALSE(model.isBlacksTurnFunc());
    }
}

TEST_F(GameScenariosTests, ForcedCaptureScenario) {
    // Create a scenario where Player1 must capture
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[2][2] = std::make_unique<Piece>(true, Position{2, 2});
    grid[3][3] = std::make_unique<Piece>(false, Position{3, 3});
    grid[1][1] = std::make_unique<Piece>(true, Position{1, 1});  // Another piece that could move

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
    grid[2][2] = std::make_unique<Piece>(true, Position{2, 2});  // Player1 at top, can move downward
    grid[3][1] = std::make_unique<Piece>(false, Position{3, 1});  // Capturable enemy to the left-forward
    grid[3][3] = std::make_unique<Piece>(false, Position{3, 3});  // Capturable enemy to the right-forward

    initializeGrid(grid);
}

TEST_F(GameScenariosTests, ChainCaptureScenario) {
    // Create a scenario with chain captures
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[1][1] = std::make_unique<Piece>(true, Position{1, 1});
    grid[2][2] = std::make_unique<Piece>(false, Position{2, 2});
    grid[4][4] = std::make_unique<Piece>(false, Position{4, 4});
    grid[6][6] = std::make_unique<Piece>(false, Position{6, 6});

    initializeGrid(grid);
}

TEST_F(GameScenariosTests, PromotionScenario) {
    // Test piece promotion to Dame
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[6][6] = std::make_unique<Piece>(true, Position{6, 6});  // Near promotion row

    initializeGrid(grid);
}

TEST_F(GameScenariosTests, DameAdvancedMovementScenario) {
    // Test Dame's advanced movement capabilities
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[4][4] = std::make_unique<Piece>(true, Position{4, 4});
    grid[4][4]->promote();
    grid[2][2] = std::make_unique<Piece>(false, Position{2, 2});      // Enemy piece
    grid[6][6] = std::make_unique<Piece>(false, Position{6, 6});      // Another enemy piece

    initializeGrid(grid);
}

TEST_F(GameScenariosTests, ComplexMandatoryCaptureScenario) {
    // Test mandatory capture rules with multiple pieces
    std::vector<std::vector<std::unique_ptr<Piece>>> grid(8);
    for (auto& row : grid) row.resize(8);
    grid[3][3] = std::make_unique<Piece>(true, Position{3, 3});
    grid[3][3]->promote();  // Make it a Dame
    grid[4][4] = std::make_unique<Piece>(false, Position{4, 4});      // Enemy piece
    grid[1][1] = std::make_unique<Piece>(true, Position{1, 1});      // Pion piece that could move

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
    grid[7][7] = std::make_unique<Piece>(true, Position{7, 7});
    grid[0][0] = std::make_unique<Piece>(false, Position{0, 0});
    initializeGrid(grid);
    EXPECT_TRUE(model.isGameOver());
    EXPECT_FALSE(model.isBlacksTurnFunc());
}
