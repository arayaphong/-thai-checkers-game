#include <gtest/gtest.h>
#include "Board.h"

class BoardTests : public ::testing::Test {
protected:
    Board board;

    void SetUp() override {
        board.initialize();
    }
};

TEST_F(BoardTests, InitializeBoard) {
    // Test that the board is initialized without crashing
    ASSERT_NO_THROW(board.initialize());
    
    // Test that isValidMove works for valid positions
    EXPECT_TRUE(board.isValidMove(0, 0, 2, 2)); // Valid position check
    EXPECT_FALSE(board.isValidMove(-1, -1, 0, 0)); // Invalid position check
}

TEST_F(BoardTests, MovePiece) {
    // Test moving a piece using actual board coordinates
    // From the board display, we can see pieces at (0,0), (0,2), etc.
    // Try to move a piece from (0,0) to (1,1) - but (1,1) is occupied, so try (2,2)
    EXPECT_TRUE(board.movePiece(0, 0, 2, 2));
    
    // Test invalid move (moving from empty space)
    EXPECT_FALSE(board.movePiece(3, 3, 4, 4));
}

TEST_F(BoardTests, DisplayBoard) {
    // Test the display functionality
    EXPECT_NO_THROW(board.display());
}