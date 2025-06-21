#include "THCheckers.h"
#include "gtest/gtest.h"

class THCheckersTests : public ::testing::Test {
protected:
    THCheckers game;

    void SetUp() override {
        game.startGame();
    }

    void TearDown() override {
        // Clean up if necessary
    }
};

TEST_F(THCheckersTests, TestInitialGameSetup) {
    // Check if the game starts without crashing
    EXPECT_NO_THROW(game.startGame());
}

TEST_F(THCheckersTests, TestDisplayBoard) {
    // Test that the board can be displayed
    EXPECT_NO_THROW(game.displayBoard());
}

TEST_F(THCheckersTests, TestWinCondition) {
    // Test the win condition check
    EXPECT_TRUE(game.checkWinCondition());
}

TEST_F(THCheckersTests, TestMakeMove) {
    // Test that makeMove can be called without crashing
    EXPECT_NO_THROW(game.makeMove());
}

TEST_F(THCheckersTests, TestGetPossibleMovesInitialPosition) {
    // For initial board, Player1 piece at (1,1) should have moves to (2,2) and (2,0)
    Position pos{1, 1};
    auto moves = game.getPossibleMoves(pos);
    ASSERT_EQ(moves.size(), 2);
    // Each move sequence contains a single step for initial moves
    EXPECT_EQ(moves[0].size(), 1);
    EXPECT_EQ(moves[0][0].x, 2);
    EXPECT_EQ(moves[0][0].y, 2);
    EXPECT_EQ(moves[1].size(), 1);
    EXPECT_EQ(moves[1][0].x, 2);
    EXPECT_EQ(moves[1][0].y, 0);
}