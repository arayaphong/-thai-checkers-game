#include <gtest/gtest.h>
#include "GameModel.h"
#include <set>

// Game Logic Tests
// Tests game rules, move validation, captures, and game state management
class GameLogicTests : public ::testing::Test {
protected:
    GameModel model;
};

// Basic Move and Capture Tests
TEST_F(GameLogicTests, InitializeStandardGame) {
    model.initializeStandardGame("Player1", "Player2");
    
    auto board = model.getBoard();
    
    // Check Player1 pieces (top rows based on board display)
    int player1Count = 0;
    for (int i = 0; i < 3; ++i) {  // Player1 is at top (rows 0-2)
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] && board[i][j]->getColor() == "Player1") {
                player1Count++;
            }
        }
    }
    EXPECT_EQ(player1Count, 8);  // Standard checkers has 8 pieces per player
    
    // Check Player2 pieces (bottom rows)
    int player2Count = 0;
    for (int i = 6; i < 8; ++i) {  // Player2 is at bottom (rows 6-7)
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] && board[i][j]->getColor() == "Player2") {
                player2Count++;
            }
        }
    }
    EXPECT_EQ(player2Count, 8);  // Standard checkers has 8 pieces per player
    
    EXPECT_EQ(model.getCurrentPlayer(), "Player1");
}

TEST_F(GameLogicTests, SimpleMove) {
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
    
    std::set<std::pair<int, int>> expected = {{4, 2}, {4, 4}};  // Player1 moves down (increasing row)
    EXPECT_EQ(destinations, expected);
}

TEST_F(GameLogicTests, CaptureMove) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});  // Player1 at top, can move down
    grid[3][3] = new Piece("Player2", {3, 3});  // Player2 diagonally below Player1
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({2, 2});
    ASSERT_EQ(moves.size(), 1);
    EXPECT_EQ(moves[0].path.size(), 1);
    EXPECT_EQ(moves[0].path[0].x, 4);  // Land at row 4 (moving downward)
    EXPECT_EQ(moves[0].path[0].y, 4);  // Land at column 4
    EXPECT_TRUE(moves[0].isCapture());
    EXPECT_EQ(moves[0].captureCount(), 1);
}

TEST_F(GameLogicTests, ExecuteMove) {
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

TEST_F(GameLogicTests, MultipleCaptures) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});  // Player1 starting position
    grid[3][3] = new Piece("Player2", {3, 3});  // First capture target
    grid[5][5] = new Piece("Player2", {5, 5});  // Second capture target
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({2, 2});
    ASSERT_EQ(moves.size(), 1);
    EXPECT_EQ(moves[0].path.size(), 2);  // Two jumps in the capture sequence
    EXPECT_EQ(moves[0].captureCount(), 2);
}

// Game State Tests
TEST_F(GameLogicTests, GameOver) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[7][7] = new Piece("Player1", {7, 7});  // Player1 at bottom corner (blocked)
    grid[0][0] = new Piece("Player2", {0, 0});  // Player2 at top corner (blocked)
    model.initializeFromGrid(grid);
    
    // Player1 at bottom corner cannot move forward
    EXPECT_TRUE(model.isGameOver());
    EXPECT_EQ(model.getWinner(), "Player2");
}

TEST_F(GameLogicTests, CloneModel) {
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

// Dame (King) Piece Tests
TEST_F(GameLogicTests, DamePromotion) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[6][6] = new Piece("Player1", {6, 6});  // Near promotion row for Player1
    model.initializeFromGrid(grid);
    
    // Move to promotion row
    auto moves = model.getValidMoves({6, 6});
    Move* promotionMove = nullptr;
    for (auto& move : moves) {
        if (!move.path.empty() && move.path[0].x == 7) {  // Bottom row for Player1 promotion
            promotionMove = &move;
            break;
        }
    }
    ASSERT_NE(promotionMove, nullptr);
    
    model.executeMove(*promotionMove);
    
    auto board = model.getBoard();
    Position newPos = promotionMove->path[0];
    EXPECT_NE(board[newPos.x][newPos.y], nullptr);
    EXPECT_TRUE(board[newPos.x][newPos.y]->isDame());
}

TEST_F(GameLogicTests, DameSimpleMove) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[4][4] = new Piece("Player1", {4, 4});
    grid[4][4]->promote();  // Make it a Dame
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({4, 4});
    
    // Dame should have moves in all 4 diagonal directions with multiple distances
    EXPECT_GT(moves.size(), 4);
    
    // Check for moves in different directions
    bool hasNorthEast = false, hasNorthWest = false, hasSouthEast = false, hasSouthWest = false;
    for (const auto& move : moves) {
        if (!move.path.empty()) {
            Position target = move.path.back();
            int dx = target.x - 4;
            int dy = target.y - 4;
            
            if (dx < 0 && dy > 0) hasNorthEast = true;
            if (dx < 0 && dy < 0) hasNorthWest = true;
            if (dx > 0 && dy > 0) hasSouthEast = true;
            if (dx > 0 && dy < 0) hasSouthWest = true;
        }
    }
    
    EXPECT_TRUE(hasNorthEast && hasNorthWest && hasSouthEast && hasSouthWest);
}

TEST_F(GameLogicTests, DameMultiCapture) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[1][1] = new Piece("Player1", {1, 1});
    grid[1][1]->promote();  // Make it a Dame
    grid[2][2] = new Piece("Player2", {2, 2});      // Enemy piece
    grid[4][4] = new Piece("Player2", {4, 4});      // Another enemy piece
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({1, 1});
    
    // Should find multi-capture moves
    bool foundMultiCapture = false;
    for (const auto& move : moves) {
        if (move.captureCount() >= 2) {
            foundMultiCapture = true;
            break;
        }
    }
    EXPECT_TRUE(foundMultiCapture);
}

TEST_F(GameLogicTests, DameFlexibleLanding) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[2][2]->promote();  // Make it a Dame
    grid[3][3] = new Piece("Player2", {3, 3});      // Enemy piece to capture
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({2, 2});
    
    // Dame should have capture moves
    int captureMovesCount = 0;
    for (const auto& move : moves) {
        if (move.isCapture()) {
            captureMovesCount++;
        }
    }
    EXPECT_GE(captureMovesCount, 1);  // At least one capture move
}
