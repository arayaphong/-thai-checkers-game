#include <gtest/gtest.h>
#include "Board.h"
#include <memory>
#include <set>

// Board Core Functionality Tests
// Tests basic board operations: initialization, display, piece management
class BoardCoreTests : public ::testing::Test {
protected:
    Board board;

    void SetUp() override {
        board.initialize();
    }
};

TEST_F(BoardCoreTests, InitializeStandardBoard) {
    // Test that the board is initialized with correct initial state
    board.setTurn(true);
    EXPECT_FALSE(board.getMoveablePieces().empty());
    board.setTurn(false);
    EXPECT_FALSE(board.getMoveablePieces().empty());
}

TEST_F(BoardCoreTests, InitializeCustomGrid) {
    // Test custom grid initialization
    std::vector<std::vector<Piece*>> customGrid(8, std::vector<Piece*>(8, nullptr));
    customGrid[3][3] = new Piece(true, {3, 3});
    board.initialize(customGrid);
    board.setTurn(true);

    auto pieces = board.getMoveablePieces();
    ASSERT_EQ(pieces.size(), 1);
    EXPECT_EQ(pieces[0]->getPosition().x, 3);
    EXPECT_EQ(pieces[0]->getPosition().y, 3);
}

TEST_F(BoardCoreTests, GetMoveablePiecesForStandardSetup) {
    // Test getting pieces that can move for Player1
    board.setTurn(true);
    const auto player1Pieces = board.getMoveablePieces();
    EXPECT_FALSE(player1Pieces.empty());

    // Player1 pieces on row 1 can move initially
    for (const auto& piece : player1Pieces) {
        Position pos = piece->getPosition();
        EXPECT_EQ(pos.x, 1);
    }

    // Test getting pieces that can move for Player2
    board.setTurn(false);
    auto player2Pieces = board.getMoveablePieces();
    EXPECT_FALSE(player2Pieces.empty());

    // Player2 pieces on row 6 can move initially
    for (const auto& piece : player2Pieces) {
        Position pos = piece->getPosition();
        EXPECT_EQ(pos.x, 6);
    }
}

TEST_F(BoardCoreTests, DisplayBoard) {
    // Test the display functionality
    EXPECT_NO_THROW(board.display());
}

TEST_F(BoardCoreTests, ValidMovesForPosition) {
    // Test getting valid moves for a specific position
    std::vector<std::vector<Piece*>> customGrid(8, std::vector<Piece*>(8, nullptr));
    customGrid[3][3] = new Piece(true, {3, 3});
    board.initialize(customGrid);
    board.setTurn(true);

    auto moves = board.getValidMovesFor({3, 3});
    ASSERT_EQ(moves.size(), 2);

    bool found1 = false, found2 = false;
    for (const auto& move : moves) {
        ASSERT_EQ(move.path.size(), 1);
        Position p = move.path[0];
        if (p.x == 4 && p.y == 4) found1 = true;
        if (p.x == 4 && p.y == 2) found2 = true;
    }
    EXPECT_TRUE(found1 && found2);
}
