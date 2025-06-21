#include <gtest/gtest.h>
#include "Board.h"

class BoardTests : public ::testing::Test {
protected:
    Board board;

    void SetUp() override {
        board.initialize("Player1", "Player2");
    }
};

TEST_F(BoardTests, InitializeBoard) {
    // Test that the board is initialized and players have pieces that can move.
    EXPECT_FALSE(board.getPiecesCanMove("Player1").empty());
    EXPECT_FALSE(board.getPiecesCanMove("Player2").empty());
    EXPECT_TRUE(board.getPiecesCanMove("NonExistentPlayer").empty());
}

TEST_F(BoardTests, IsValidMove) {
    // Assuming Player 1 pieces are on rows 0 and 1. A piece at (1,1) can move to (2,2).
    EXPECT_TRUE(board.isValidMove(1, 1, 2, 2));
    // Moving backwards is not allowed for regular pieces.
    EXPECT_FALSE(board.isValidMove(1, 1, 0, 0));
    // Moving to an occupied square is not allowed.
    EXPECT_FALSE(board.isValidMove(0, 0, 1, 1));
    // A jump is not a valid move on an empty board path.
    EXPECT_FALSE(board.isValidMove(1, 1, 3, 3));
}

TEST_F(BoardTests, MovePiece) {
    // Test a valid opening move for Player 1.
    EXPECT_TRUE(board.movePiece(1, 1, 2, 2));
    
    // After moving, try to move from the original position again (should fail).
    EXPECT_FALSE(board.movePiece(1, 1, 3, 3));

    // Test invalid move (moving from what is now an empty space)
    EXPECT_FALSE(board.movePiece(1, 1, 0, 0));

    // Test moving opponent's piece. After the first move, it is Player2's turn.
    EXPECT_TRUE(board.movePiece(6, 0, 5, 1));
}

TEST_F(BoardTests, DisplayBoard) {
    // Test the display functionality
    EXPECT_NO_THROW(board.display());
}

TEST_F(BoardTests, GetPiecesCanMove) {
    // Test getting pieces that can move for Player1
    std::vector<Position> player1Pieces = board.getPiecesCanMove("Player1");
    EXPECT_FALSE(player1Pieces.empty());

    // Assuming Player1 pieces are on rows 0 and 1, only pieces on row 1 can move initially.
    for (const Position& pos : player1Pieces) {
        EXPECT_EQ(pos.x, 1);
    }

    // Test getting pieces that can move for Player2
    std::vector<Position> player2Pieces = board.getPiecesCanMove("Player2");
    EXPECT_FALSE(player2Pieces.empty());

    // Assuming Player2 pieces are on rows 6 and 7, only pieces on row 6 can move initially.
    for (const Position& pos : player2Pieces) {
        EXPECT_EQ(pos.x, 6);
    }
}

TEST_F(BoardTests, GetPiecesCanMoveAfterMove) {
    std::vector<Position> player1InitialPieces = board.getPiecesCanMove("Player1");
    size_t initialCount = player1InitialPieces.size();
    
    // Make a move
    ASSERT_TRUE(board.movePiece(1, 1, 2, 2));
    
    // After P1 moves, let's assume getPiecesCanMove is for a given player regardless of turn.
    // The piece at (0,0) and (0,2) might now be able to move.
    std::vector<Position> player1AfterMove = board.getPiecesCanMove("Player1");
    size_t afterMoveCount = player1AfterMove.size();
    
    // The number of pieces that can move should have changed.
    EXPECT_NE(initialCount, afterMoveCount);
}

TEST_F(BoardTests, GetPiecesCanMoveCaptureScenario) {
    // Set up a capture scenario
    ASSERT_TRUE(board.movePiece(1, 1, 2, 2));
    ASSERT_TRUE(board.movePiece(6, 2, 5, 1));
    ASSERT_TRUE(board.movePiece(2, 2, 3, 3));
    ASSERT_TRUE(board.movePiece(5, 1, 4, 2));
    
    // Now, Player1 should have a capture opportunity.
    // The piece at (3,3) can capture the piece at (4,2).
    // We can check if (3,3) is in the list of pieces that can move.
    std::vector<Position> player1Pieces = board.getPiecesCanMove("Player1");
    bool found_capture_piece = false;
    for (const auto& pos : player1Pieces) {
        if (pos.x == 3 && pos.y == 3) {
            found_capture_piece = true;
            break;
        }
    }
    EXPECT_TRUE(found_capture_piece);
}