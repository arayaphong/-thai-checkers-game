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

// Scenario: Multi-Choice Capture
TEST_F(BoardTests, MultiChoiceCapture) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[3][1] = new Piece("Player2", {3, 1});
    grid[3][3] = new Piece("Player2", {3, 3});
    board.initialize(grid);
    auto moves = board.getTargetPositions(*grid[2][2]);
    // Expect two capture options
    ASSERT_EQ(moves.size(), 2u);
    std::set<std::pair<int,int>> targets;
    for (const auto& seq : moves) {
        ASSERT_EQ(seq.size(), 1u);
        targets.emplace(seq[0].x, seq[0].y);
    }
    EXPECT_EQ(targets, (std::set<std::pair<int,int>>{{4,0}, {4,4}}));
    auto pieces = board.getMoveablePieces("Player1");
    ASSERT_EQ(pieces.size(), 1u);
    EXPECT_EQ(pieces[0].getPosition().x, 2);
    EXPECT_EQ(pieces[0].getPosition().y, 2);
}

// Scenario: Chain Capture with Multiple Choices
TEST_F(BoardTests, ChainCaptureMultiChoice) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[0][0] = new Piece("Player1", {0, 0});
    grid[1][1] = new Piece("Player2", {1, 1});
    grid[3][3] = new Piece("Player2", {3, 3});
    grid[3][1] = new Piece("Player2", {3, 1});
    board.initialize(grid);
    auto moves = board.getTargetPositions(*grid[0][0]);
    // Expect two distinct chains of two captures
    ASSERT_EQ(moves.size(), 2u);
    std::set<std::vector<std::pair<int,int>>> chains;
    for (const auto& seq : moves) {
        ASSERT_EQ(seq.size(), 2u);
        chains.insert({{seq[0].x, seq[0].y}, {seq[1].x, seq[1].y}});
    }
    std::set<std::vector<std::pair<int,int>>> expected = {
        {{2,2}, {4,4}},
        {{2,2}, {4,0}}
    };
    EXPECT_EQ(chains, expected);
    auto pieces = board.getMoveablePieces("Player1");
    ASSERT_EQ(pieces.size(), 1u);
    EXPECT_EQ(pieces[0].getPosition().x, 0);
    EXPECT_EQ(pieces[0].getPosition().y, 0);
}

// Scenario: Multi-Capture Chain
TEST_F(BoardTests, MultiCaptureChain) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[0][0] = new Piece("Player1", {0, 0});
    grid[1][1] = new Piece("Player2", {1, 1});
    grid[3][3] = new Piece("Player2", {3, 3});
    board.initialize(grid);
    auto moves = board.getTargetPositions(*grid[0][0]);
    ASSERT_EQ(moves.size(), 1u);
    const auto& seq = moves.front();
    ASSERT_EQ(seq.size(), 2u);
    EXPECT_EQ(seq[0].x, 2);
    EXPECT_EQ(seq[0].y, 2);
    EXPECT_EQ(seq[1].x, 4);
    EXPECT_EQ(seq[1].y, 4);
    auto pieces = board.getMoveablePieces("Player1");
    ASSERT_EQ(pieces.size(), 1u);
    EXPECT_EQ(pieces[0].getPosition().x, 0);
    EXPECT_EQ(pieces[0].getPosition().y, 0);
}

// Scenario: Blocked Piece
TEST_F(BoardTests, BlockedPieceScenario) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[4][4] = new Piece("Player1", {4, 4});
    grid[5][3] = new Piece("Player1", {5, 3});
    grid[5][5] = new Piece("Player1", {5, 5});
    board.initialize(grid);
    // central piece has no moves
    auto noMoves = board.getTargetPositions(*grid[4][4]);
    EXPECT_TRUE(noMoves.empty());
    // only the surrounding two pieces can move
    auto pieces = board.getMoveablePieces("Player1");
    ASSERT_EQ(pieces.size(), 2u);
    std::set<std::pair<int,int>> coords;
    for (auto& p : pieces) coords.emplace(p.getPosition().x, p.getPosition().y);
    EXPECT_EQ(coords, (std::set<std::pair<int,int>>{{5,3},{5,5}}));
}

// Scenario: Multiple Moveable Pieces
TEST_F(BoardTests, MultipleMoveablePieces) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    // place simple-move pieces at (2,1),(2,3),(2,5),(2,7),(4,1)
    grid[2][1] = new Piece("Player1", {2, 1});
    grid[2][3] = new Piece("Player1", {2, 3});
    grid[2][5] = new Piece("Player1", {2, 5});
    grid[2][7] = new Piece("Player1", {2, 7});
    grid[4][1] = new Piece("Player1", {4, 1});
    board.initialize(grid);
    auto pieces = board.getMoveablePieces("Player1");
    ASSERT_EQ(pieces.size(), 5u);
    std::set<std::pair<int,int>> coords;
    for (auto& p : pieces) coords.emplace(p.getPosition().x, p.getPosition().y);
    EXPECT_EQ(coords, (std::set<std::pair<int,int>>{{2,1},{2,3},{2,5},{2,7},{4,1}}));
}

// Scenario: Custom Mid-Game Scenario
TEST_F(BoardTests, CustomMidGameScenario) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    // Row 0
    grid[0][0] = new Piece("Player1", {0, 0});
    grid[0][2] = new Piece("Player1", {0, 2});
    grid[0][4] = new Piece("Player1", {0, 4});
    grid[0][6] = new Piece("Player1", {0, 6});
    // Row 1
    grid[1][3] = new Piece("Player1", {1, 3});
    grid[1][5] = new Piece("Player1", {1, 5});
    grid[1][7] = new Piece("Player1", {1, 7});
    // Row 2 extra X
    grid[2][0] = new Piece("Player1", {2, 0});
    // Row 6
    grid[6][0] = new Piece("Player2", {6, 0});
    grid[6][2] = new Piece("Player2", {6, 2});
    grid[6][4] = new Piece("Player2", {6, 4});
    grid[6][6] = new Piece("Player2", {6, 6});
    // Row 7
    grid[7][1] = new Piece("Player2", {7, 1});
    grid[7][3] = new Piece("Player2", {7, 3});
    grid[7][5] = new Piece("Player2", {7, 5});
    grid[7][7] = new Piece("Player2", {7, 7});

    board.initialize(grid);
    auto pieces = board.getMoveablePieces("Player1");
    // Expect six moveable X pieces
    ASSERT_EQ(pieces.size(), 6u);
    std::set<std::pair<int,int>> coords;
    for (const auto& p : pieces) coords.emplace(p.getPosition().x, p.getPosition().y);
    EXPECT_EQ(coords, (std::set<std::pair<int,int>>{{0,0},{0,2},{1,3},{1,5},{1,7},{2,0}}));

    // Test capture move for piece at (2,0)
    auto moves = board.getTargetPositions(*grid[2][0]);
    ASSERT_EQ(moves.size(), 1u);
    ASSERT_EQ(moves[0].size(), 1u);
    EXPECT_EQ(moves[0][0].x, 3);
    EXPECT_EQ(moves[0][0].y, 1);
}