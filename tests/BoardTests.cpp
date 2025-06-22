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
    EXPECT_FALSE(board.getMoveablePieces("Player1").empty());
    EXPECT_FALSE(board.getMoveablePieces("Player2").empty());
    EXPECT_TRUE(board.getMoveablePieces("NonExistentPlayer").empty());
}

TEST_F(BoardTests, DisplayBoard) {
    // Test the display functionality
    EXPECT_NO_THROW(board.display());
}

TEST_F(BoardTests, GetMoveablePieces) {
    // Test getting pieces that can move for Player1
    const std::vector<Piece> player1Pieces = board.getMoveablePieces("Player1");
    EXPECT_FALSE(player1Pieces.empty());

    // Assuming Player1 pieces are on rows 0 and 1, only pieces on row 1 can move initially.
    for (const Piece& piece : player1Pieces) {
        Position pos = piece.getPosition();
        EXPECT_EQ(pos.x, 1);
    }

    // Test getting pieces that can move for Player2
    std::vector<Piece> player2Pieces = board.getMoveablePieces("Player2");
    EXPECT_FALSE(player2Pieces.empty());

    // Assuming Player2 pieces are on rows 6 and 7, only pieces on row 6 can move initially.
    for (const Piece& piece : player2Pieces) {
        Position pos = piece.getPosition();
        EXPECT_EQ(pos.x, 6);
    }
}
/*
TEST_F(BoardTests, GetMoveablePiecesAfterMove) {
    std::vector<Piece> player1InitialPieces = board.getMoveablePieces("Player1");
    size_t initialCount = player1InitialPieces.size();

    auto pieces = board.getMoveablePieces(board.getCurrentPlayer());
    auto moves = board.getTargetPositions(pieces[0]);
    
    // Make a move
    ASSERT_TRUE(board.movePiece(1, 1, 2, 2));
    
    // After P1 moves, let's assume getMoveablePieces is for a given player regardless of turn.
    // The piece at (0,0) and (0,2) might now be able to move.
    std::vector<Piece> player1AfterMove = board.getMoveablePieces("Player1");
    size_t afterMoveCount = player1AfterMove.size();
    
    // The number of pieces that can move should have changed.
    EXPECT_NE(initialCount, afterMoveCount);
}

TEST_F(BoardTests, GetMoveablePiecesCaptureScenario) {
    // Set up a capture scenario
    ASSERT_TRUE(board.movePiece(1, 1, 2, 2));
    ASSERT_TRUE(board.movePiece(6, 2, 5, 1));
    ASSERT_TRUE(board.movePiece(2, 2, 3, 3));
    ASSERT_TRUE(board.movePiece(5, 1, 4, 2));
    
    // Now, Player1 should have a capture opportunity.
    // The piece at (3,3) can capture the piece at (4,2).
    // We can check if (3,3) is in the list of pieces that can move.
    std::vector<Piece> player1Pieces = board.getMoveablePieces("Player1");
    bool found_capture_piece = false;
    for (const auto& piece : player1Pieces) {
        Position pos = piece.getPosition();
        if (pos.x == 3 && pos.y == 3) {
            found_capture_piece = true;
            break;
        }
    }
    EXPECT_TRUE(found_capture_piece);
}
*/