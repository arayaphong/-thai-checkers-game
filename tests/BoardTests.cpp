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

// New tests for custom grid initialization and move scenarios
TEST_F(BoardTests, CustomGridInitializeSimpleMove) {
    // Create empty 8x8 grid
    std::vector<std::vector<Piece*>> customGrid(8, std::vector<Piece*>(8, nullptr));
    // Place a Player1 piece at (3,3)
    customGrid[3][3] = new Piece("Player1", {3, 3});
    board.initialize(customGrid);
    
    // Get possible moves for the piece
    auto moves = board.getTargetPositions(*customGrid[3][3]);
    // Should have two simple moves
    ASSERT_EQ(moves.size(), 2);
    bool found1 = false, found2 = false;
    for (const auto& seq : moves) {
        ASSERT_EQ(seq.size(), 1);
        Position p = seq[0];
        if (p.x == 4 && p.y == 4) found1 = true;
        if (p.x == 4 && p.y == 2) found2 = true;
    }
    EXPECT_TRUE(found1 && found2);
    
    // Check getMoveablePieces returns the piece at (3,3)
    auto pieces = board.getMoveablePieces("Player1");
    ASSERT_EQ(pieces.size(), 1);
    EXPECT_EQ(pieces[0].getPosition().x, 3);
    EXPECT_EQ(pieces[0].getPosition().y, 3);
}

TEST_F(BoardTests, CustomGridInitializeCaptureMove) {
    // Create empty 8x8 grid
    std::vector<std::vector<Piece*>> customGrid(8, std::vector<Piece*>(8, nullptr));
    // Place Player1 piece at (2,2) and Player2 at (3,3)
    customGrid[2][2] = new Piece("Player1", {2, 2});
    customGrid[3][3] = new Piece("Player2", {3, 3});
    board.initialize(customGrid);
    
    // Get possible moves (including captures) for the piece
    auto moves = board.getTargetPositions(*customGrid[2][2]);
    // Should include a capture path to (4,4)
    bool foundCapture = false;
    for (const auto& seq : moves) {
        if (seq.size() == 1 && seq[0].x == 4 && seq[0].y == 4) {
            foundCapture = true;
            break;
        }
    }
    EXPECT_TRUE(foundCapture) << "Expected a capture move to (4,4)";
    
    // Check getMoveablePieces returns the piece at (2,2)
    auto pieces = board.getMoveablePieces("Player1");
    ASSERT_EQ(pieces.size(), 1);
    EXPECT_EQ(pieces[0].getPosition().x, 2);
    EXPECT_EQ(pieces[0].getPosition().y, 2);
}