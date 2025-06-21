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

TEST_F(BoardTests, GetPiecesCanMove) {
    // Test getting pieces that can move for Player1
    std::vector<Position> player1Pieces = board.getPiecesCanMove("Player1");
    
    // In the initial position, Player1 should have pieces that can move
    // Player1 pieces are on rows 0 and 1
    EXPECT_FALSE(player1Pieces.empty());
    
    // Test getting pieces that can move for Player2
    std::vector<Position> player2Pieces = board.getPiecesCanMove("Player2");
    
    // Player2 pieces are on rows 6 and 7
    EXPECT_FALSE(player2Pieces.empty());
    
    // Test with non-existent player
    std::vector<Position> nonExistentPlayerPieces = board.getPiecesCanMove("NonExistentPlayer");
    EXPECT_TRUE(nonExistentPlayerPieces.empty());
    
    // Verify that returned positions contain valid pieces
    for (const Position& pos : player1Pieces) {
        EXPECT_GE(pos.x, 0);
        EXPECT_LT(pos.x, 8);
        EXPECT_GE(pos.y, 0);
        EXPECT_LT(pos.y, 8);
        // Verify that these positions have actual moves available
        std::vector<std::vector<Position>> moves = board.getPossibleMoves(pos);
        EXPECT_FALSE(moves.empty());
    }
}

TEST_F(BoardTests, GetPiecesCanMoveAfterMove) {
    // Test that movable pieces count changes after making a move
    std::vector<Position> player1InitialPieces = board.getPiecesCanMove("Player1");
    size_t initialCount = player1InitialPieces.size();
    
    // Make a move
    ASSERT_TRUE(board.movePiece(1, 1, 2, 2));
    
    // Check pieces can move after the move
    std::vector<Position> player1AfterMove = board.getPiecesCanMove("Player1");
    size_t afterMoveCount = player1AfterMove.size();
    
    // The count should change (likely increase due to new movement possibilities)
    EXPECT_NE(initialCount, afterMoveCount);
    
    // All returned positions should still be valid
    for (const Position& pos : player1AfterMove) {
        EXPECT_GE(pos.x, 0);
        EXPECT_LT(pos.x, 8);
        EXPECT_GE(pos.y, 0);
        EXPECT_LT(pos.y, 8);
    }
}

TEST_F(BoardTests, GetPiecesCanMoveCaptureScenario) {
    // Set up a capture scenario
    Board captureBoard;
    captureBoard.initialize("Player1", "Player2");
    
    // Move pieces to create capture opportunity
    ASSERT_TRUE(captureBoard.movePiece(1, 1, 2, 2));
    ASSERT_TRUE(captureBoard.movePiece(6, 2, 5, 1));
    ASSERT_TRUE(captureBoard.movePiece(2, 2, 3, 3));
    ASSERT_TRUE(captureBoard.movePiece(5, 1, 4, 2));
    
    // Now there should be capture opportunities
    std::vector<Position> player1Pieces = captureBoard.getPiecesCanMove("Player1");
    
    // Should have pieces that can move (including potential captures)
    EXPECT_FALSE(player1Pieces.empty());
    
    // Verify that at least one piece has moves (could be captures)
    bool hasMovablePiece = false;
    for (const Position& pos : player1Pieces) {
        std::vector<std::vector<Position>> moves = captureBoard.getPossibleMoves(pos);
        if (!moves.empty()) {
            hasMovablePiece = true;
            break;
        }
    }
    EXPECT_TRUE(hasMovablePiece);
}

TEST_F(BoardTests, GetPiecesCanMoveEmptyBoard) {
    // Test with an empty board
    Board emptyBoard;
    // Don't initialize - board should be empty
    
    std::vector<Position> player1Pieces = emptyBoard.getPiecesCanMove("Player1");
    std::vector<Position> player2Pieces = emptyBoard.getPiecesCanMove("Player2");
    
    // Empty board should have no movable pieces
    EXPECT_TRUE(player1Pieces.empty());
    EXPECT_TRUE(player2Pieces.empty());
}

TEST_F(BoardTests, GetPiecesCanMoveCustomPlayerNames) {
    // Test with custom player names
    Board customBoard;
    customBoard.initialize("Alice", "Bob");
    
    std::vector<Position> alicePieces = customBoard.getPiecesCanMove("Alice");
    std::vector<Position> bobPieces = customBoard.getPiecesCanMove("Bob");
    std::vector<Position> invalidPieces = customBoard.getPiecesCanMove("Charlie");
    
    // Alice and Bob should have movable pieces
    EXPECT_FALSE(alicePieces.empty());
    EXPECT_FALSE(bobPieces.empty());
    
    // Invalid player should return empty
    EXPECT_TRUE(invalidPieces.empty());
    
    // Verify Alice's pieces are valid
    for (const Position& pos : alicePieces) {
        EXPECT_GE(pos.x, 0);
        EXPECT_LT(pos.x, 8);
        EXPECT_GE(pos.y, 0);
        EXPECT_LT(pos.y, 8);
        std::vector<std::vector<Position>> moves = customBoard.getPossibleMoves(pos);
        EXPECT_FALSE(moves.empty());
    }
}

TEST_F(BoardTests, GetPiecesCanMoveEdgePieces) {
    // Test edge pieces have fewer movement options
    std::vector<Position> player1Pieces = board.getPiecesCanMove("Player1");
    std::vector<Position> player2Pieces = board.getPiecesCanMove("Player2");
    
    // Find edge pieces and verify they have limited moves
    for (const Position& pos : player1Pieces) {
        std::vector<std::vector<Position>> moves = board.getPossibleMoves(pos);
        
        if (pos.y == 0 || pos.y == 7) {
            // Edge pieces should have fewer moves than center pieces
            EXPECT_LE(moves.size(), 2); // Edge pieces have at most 2 moves
        }
    }
    
    for (const Position& pos : player2Pieces) {
        std::vector<std::vector<Position>> moves = board.getPossibleMoves(pos);
        
        if (pos.y == 0 || pos.y == 7) {
            // Edge pieces should have fewer moves than center pieces
            EXPECT_LE(moves.size(), 2); // Edge pieces have at most 2 moves
        }
    }
}

TEST_F(BoardTests, GetPiecesCanMoveInitialCount) {
    // Test that initial board has expected number of movable pieces
    std::vector<Position> player1Pieces = board.getPiecesCanMove("Player1");
    std::vector<Position> player2Pieces = board.getPiecesCanMove("Player2");
    
    // In initial position, only pieces on row 1 (Player1) and row 6 (Player2) can move
    // There should be 4 pieces each (on alternating squares)
    EXPECT_EQ(player1Pieces.size(), 4);
    EXPECT_EQ(player2Pieces.size(), 4);
    
    // Verify Player1 pieces are on row 1
    for (const Position& pos : player1Pieces) {
        EXPECT_EQ(pos.x, 1);
        EXPECT_TRUE(pos.y % 2 == 1); // Should be on odd columns (1, 3, 5, 7)
    }
    
    // Verify Player2 pieces are on row 6
    for (const Position& pos : player2Pieces) {
        EXPECT_EQ(pos.x, 6);
        EXPECT_TRUE(pos.y % 2 == 0); // Should be on even columns (0, 2, 4, 6)
    }
}

TEST_F(BoardTests, GetPiecesCanMoveAfterMultipleMoves) {
    // Test behavior after multiple moves
    Board multiMoveBoard;
    multiMoveBoard.initialize("Player1", "Player2");
    
    // Make several moves
    ASSERT_TRUE(multiMoveBoard.movePiece(1, 1, 2, 2));
    ASSERT_TRUE(multiMoveBoard.movePiece(1, 3, 2, 4));
    ASSERT_TRUE(multiMoveBoard.movePiece(6, 2, 5, 3));
    ASSERT_TRUE(multiMoveBoard.movePiece(6, 4, 5, 5));
    
    std::vector<Position> player1Pieces = multiMoveBoard.getPiecesCanMove("Player1");
    std::vector<Position> player2Pieces = multiMoveBoard.getPiecesCanMove("Player2");
    
    // Should still have movable pieces
    EXPECT_FALSE(player1Pieces.empty());
    EXPECT_FALSE(player2Pieces.empty());
    
    // All pieces should be valid and have moves
    for (const Position& pos : player1Pieces) {
        EXPECT_GE(pos.x, 0);
        EXPECT_LT(pos.x, 8);
        EXPECT_GE(pos.y, 0);
        EXPECT_LT(pos.y, 8);
        std::vector<std::vector<Position>> moves = multiMoveBoard.getPossibleMoves(pos);
        EXPECT_FALSE(moves.empty());
    }
}

TEST_F(BoardTests, GetPiecesCanMoveConsistency) {
    // Test that the method is consistent - calling it multiple times returns same result
    std::vector<Position> firstCall = board.getPiecesCanMove("Player1");
    std::vector<Position> secondCall = board.getPiecesCanMove("Player1");
    
    EXPECT_EQ(firstCall.size(), secondCall.size());
    
    // Sort both vectors for comparison
    std::sort(firstCall.begin(), firstCall.end(), [](const Position& a, const Position& b) {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
    });
    std::sort(secondCall.begin(), secondCall.end(), [](const Position& a, const Position& b) {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
    });
    
    // Compare each position
    for (size_t i = 0; i < firstCall.size(); ++i) {
        EXPECT_EQ(firstCall[i].x, secondCall[i].x);
        EXPECT_EQ(firstCall[i].y, secondCall[i].y);
    }
}