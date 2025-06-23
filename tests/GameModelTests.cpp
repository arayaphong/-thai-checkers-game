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
    
    std::set<std::pair<int, int>> expected = {{4, 2}, {4, 4}};  // Player1 moves down (increasing row)
    EXPECT_EQ(destinations, expected);
}

TEST_F(GameModelTests, CaptureMove) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[4][4] = new Piece("Player1", {4, 4});  // Changed positions for valid capture
    grid[3][3] = new Piece("Player2", {3, 3});
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({4, 4});
    ASSERT_EQ(moves.size(), 1);
    EXPECT_EQ(moves[0].path.size(), 1);
    EXPECT_EQ(moves[0].path[0].x, 2);
    EXPECT_EQ(moves[0].path[0].y, 2);
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
    grid[6][2] = new Piece("Player1", {6, 2});  // Changed position for valid multi-capture
    grid[5][3] = new Piece("Player2", {5, 3});
    grid[3][5] = new Piece("Player2", {3, 5});
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({6, 2});
    ASSERT_EQ(moves.size(), 1);
    EXPECT_EQ(moves[0].path.size(), 2);
    EXPECT_EQ(moves[0].captureCount(), 2);
}

TEST_F(GameModelTests, GameOver) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[7][7] = new Piece("Player1", {7, 7});  // Player1 at bottom corner (blocked)
    grid[0][0] = new Piece("Player2", {0, 0});  // Player2 at top corner (blocked)
    model.initializeFromGrid(grid);
    
    // Player1 at bottom corner cannot move forward
    EXPECT_TRUE(model.isGameOver());
    EXPECT_EQ(model.getWinner(), "Player2");
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

TEST_F(GameModelTests, DamePromotion) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[6][6] = new Piece("Player1", {6, 6});  // Near promotion row for Player1
    model.initializeFromGrid(grid);
    
    // Move to promotion row
    auto moves = model.getValidMoves({6, 6});
    Move* promotionMove = nullptr;
    for (auto& move : moves) {
        if (move.path[0].x == 7) {  // Bottom row for Player1 promotion
            promotionMove = &move;
            break;
        }
    }
    ASSERT_NE(promotionMove, nullptr);
    
    model.executeMove(*promotionMove);
    
    // Check piece is promoted
    auto board = model.getBoard();
    // Check the actual landing position
    Piece* promotedPiece = nullptr;
    Position promotedPos;
    for (int y = 0; y < 8; ++y) {
        if (board[7][y] && board[7][y]->getColor() == "Player1") {
            promotedPiece = board[7][y];
            promotedPos = {7, y};
            break;
        }
    }
    ASSERT_NE(promotedPiece, nullptr);
    EXPECT_TRUE(promotedPiece->isDame());
}

TEST_F(GameModelTests, DameSimpleMove) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    auto dame = new Piece("Player1", {3, 3}, Piece::Type::Dame);
    grid[3][3] = dame;
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({3, 3});
    // Dame can move to multiple squares in 4 directions
    EXPECT_GT(moves.size(), 4);
    
    // Check some expected moves
    bool found55 = false, found11 = false;
    for (const auto& move : moves) {
        if (move.path[0].x == 5 && move.path[0].y == 5) found55 = true;
        if (move.path[0].x == 1 && move.path[0].y == 1) found11 = true;
    }
    EXPECT_TRUE(found55);
    EXPECT_TRUE(found11);
}

TEST_F(GameModelTests, DameMultiCapture) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    auto dame = new Piece("Player1", {7, 7}, Piece::Type::Dame);  // Start from corner
    grid[7][7] = dame;
    grid[5][5] = new Piece("Player2", {5, 5});
    grid[2][2] = new Piece("Player2", {2, 2});
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({7, 7});
    
    // Should find multi-capture paths
    bool foundDoubleCapture = false;
    for (const auto& move : moves) {
        if (move.captureCount() == 2) {
            foundDoubleCapture = true;
            break;
        }
    }
    EXPECT_TRUE(foundDoubleCapture);
}

TEST_F(GameModelTests, DameFlexibleLanding) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    auto dame = new Piece("Player1", {7, 7}, Piece::Type::Dame);  // Start from corner
    grid[7][7] = dame;
    grid[5][5] = new Piece("Player2", {5, 5});
    model.initializeFromGrid(grid);
    
    auto moves = model.getValidMoves({7, 7});
    
    // Dame should only land on (4,4) - the first empty square after (5,5)
    int captureMovesCount = 0;
    bool foundCorrectLanding = false;
    for (const auto& move : moves) {
        if (move.isCapture() && move.captureCount() == 1) {
            captureMovesCount++;
            if (move.path[0].x == 4 && move.path[0].y == 4) {
                foundCorrectLanding = true;
            }
        }
    }
    EXPECT_EQ(captureMovesCount, 1); // Only one capture move should exist
    EXPECT_TRUE(foundCorrectLanding); // And it must land on (4,4)
}
