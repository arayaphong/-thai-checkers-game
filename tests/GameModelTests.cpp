#include <gtest/gtest.h>
#include "GameModel.h"
#include <set>

class GameModelTests : public ::testing::Test {
protected:
    GameModel model;
};

TEST_F(GameModelTests, InitializeStandardGame) {
    model.initializeStandardGame("Player1", "Player2");
    
    auto board = model.getBoard();
    
    // Check Player1 pieces
    int player1Count = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] && board[i][j]->getColor() == "Player1") {
                player1Count++;
            }
        }
    }
    EXPECT_EQ(player1Count, 8);
    
    // Check Player2 pieces
    int player2Count = 0;
    for (int i = 6; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] && board[i][j]->getColor() == "Player2") {
                player2Count++;
            }
        }
    }
    EXPECT_EQ(player2Count, 8);
    
    EXPECT_EQ(model.getCurrentPlayer(), "Player1");
}

TEST_F(GameModelTests, SimpleMove) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[3][3] = new Piece("Player1", {3, 3});
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({3, 3});
    ASSERT_EQ(moves.size(), 2);
    
    std::set<std::pair<int, int>> destinations;
    for (const auto& move : moves) {
        ASSERT_EQ(move.path.size(), 1);
        destinations.insert({move.path[0].x, move.path[0].y});
    }
    
    std::set<std::pair<int, int>> expected = {{4, 2}, {4, 4}};
    EXPECT_EQ(destinations, expected);
}

TEST_F(GameModelTests, CaptureMove) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[3][3] = new Piece("Player2", {3, 3});
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({2, 2});
    ASSERT_EQ(moves.size(), 1);
    EXPECT_EQ(moves[0].path.size(), 1);
    EXPECT_EQ(moves[0].path[0].x, 4);
    EXPECT_EQ(moves[0].path[0].y, 4);
    EXPECT_TRUE(moves[0].isCapture());
    EXPECT_EQ(moves[0].captureCount(), 1);
}

TEST_F(GameModelTests, ExecuteMove) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[3][3] = new Piece("Player2", {3, 3});
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({2, 2});
    ASSERT_FALSE(moves.empty());
    
    model.executeMove(moves[0]);
    
    auto board = model.getBoard();
    EXPECT_EQ(board[2][2], nullptr);
    EXPECT_EQ(board[3][3], nullptr); // Captured
    EXPECT_NE(board[4][4], nullptr);
    EXPECT_EQ(board[4][4]->getColor(), "Player1");
    EXPECT_EQ(model.getCurrentPlayer(), "Player2");
}

TEST_F(GameModelTests, MultipleCaptures) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[3][3] = new Piece("Player2", {3, 3});
    grid[5][5] = new Piece("Player2", {5, 5});
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({2, 2});
    ASSERT_EQ(moves.size(), 1);
    EXPECT_EQ(moves[0].path.size(), 2);
    EXPECT_EQ(moves[0].captureCount(), 2);
}

TEST_F(GameModelTests, GameOver) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[7][7] = new Piece("Player1", {7, 7});
    grid[0][0] = new Piece("Player2", {0, 0});
    model.initializeFromGrid(grid);
    
    // Player1 at corner cannot move
    EXPECT_TRUE(model.isGameOver());
    EXPECT_EQ(model.getWinner(), "Player2");
}

TEST_F(GameModelTests, PositionEvaluation) {
    model.initializeStandardGame("Player1", "Player2");
    
    double eval1 = model.evaluatePosition("Player1");
    double eval2 = model.evaluatePosition("Player2");
    
    // In starting position, evaluation should be equal
    EXPECT_NEAR(eval1, -eval2, 0.01);
}

TEST_F(GameModelTests, CloneModel) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[3][3] = new Piece("Player1", {3, 3});
    model.initializeFromGrid(grid);
    
    GameModel* clone = model.clone();
    
    // Verify clone has same state
    auto origBoard = model.getBoard();
    auto cloneBoard = clone->getBoard();
    
    EXPECT_NE(origBoard[3][3], cloneBoard[3][3]); // Different pointers
    EXPECT_EQ(cloneBoard[3][3]->getColor(), "Player1");
    EXPECT_EQ(cloneBoard[3][3]->getPosition().x, 3);
    EXPECT_EQ(cloneBoard[3][3]->getPosition().y, 3);
    
    delete clone;
}
