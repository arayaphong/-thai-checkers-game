#include <gtest/gtest.h>
#include "GameModel.h"
#include "Board.h"
#include "Piece.h"
#include <vector>
#include <cmath>

class GameScenarioTests : public ::testing::Test {
protected:
    GameModel model;
    
    void SetUp() override {
        // Each test will set up its own grid
    }
    
    void TearDown() override {
        // GameModel handles cleanup of pieces
    }
    
    // Helper function to create a grid and initialize the model
    void initializeGrid(const std::vector<std::vector<Piece*>>& grid) {
        model.initializeFromGrid(grid);
    }
    
    // Helper function to clean up grid memory
    void cleanupGrid(std::vector<std::vector<Piece*>>& grid) {
        for (auto& row : grid) {
            for (auto& piece : row) {
                delete piece;
                piece = nullptr;
            }
        }
    }
};

// Test 1: Regular Piece Move - mimics demonstrateRegularPieceMove()
TEST_F(GameScenarioTests, RegularPieceMoveTest) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[3][3] = new Piece("Player1", {3, 3});
    
    initializeGrid(grid);
    
    auto allMoves = model.getAllValidMoves();
    
    // Should have exactly one moveable piece
    EXPECT_EQ(allMoves.size(), 1);
    
    // Check that the piece at (3,3) has moves
    auto piecePos = Position{3, 3};
    ASSERT_TRUE(allMoves.find(piecePos) != allMoves.end());
    
    auto moves = allMoves[piecePos];
    
    // Regular piece should have 2 diagonal forward moves
    EXPECT_EQ(moves.size(), 2);
    
    // Check specific move destinations (4,2) and (4,4)
    bool hasMove42 = false, hasMove44 = false;
    for (const auto& move : moves) {
        if (!move.path.empty()) {
            Position target = move.path.back();
            if (target.x == 4 && target.y == 2) hasMove42 = true;
            if (target.x == 4 && target.y == 4) hasMove44 = true;
        }
    }
    
    EXPECT_TRUE(hasMove42);
    EXPECT_TRUE(hasMove44);
    
    // Execute first move and verify position change
    if (!moves.empty()) {
        model.executeMove(moves[0]);
        auto board = model.getBoard();
        
        // Original position should be empty
        EXPECT_EQ(board[3][3], nullptr);
        
        // New position should have the piece
        Position newPos = moves[0].path.back();
        EXPECT_NE(board[newPos.x][newPos.y], nullptr);
        EXPECT_EQ(board[newPos.x][newPos.y]->getColor(), "Player1");
    }
}

// Test 2: Regular Piece Capture - mimics demonstrateRegularPieceCapture()
TEST_F(GameScenarioTests, RegularPieceCaptureTest) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[3][3] = new Piece("Player2", {3, 3});
    
    initializeGrid(grid);
    
    auto allMoves = model.getAllValidMoves();
    
    // Should have exactly one moveable piece
    EXPECT_EQ(allMoves.size(), 1);
    
    auto piecePos = Position{2, 2};
    ASSERT_TRUE(allMoves.find(piecePos) != allMoves.end());
    
    auto moves = allMoves[piecePos];
    
    // Should have exactly one capture move
    EXPECT_EQ(moves.size(), 1);
    
    // Verify it's a capture move
    EXPECT_TRUE(moves[0].isCapture());
    EXPECT_EQ(moves[0].captureCount(), 1);
    
    // Verify capture destination is (4,4)
    ASSERT_FALSE(moves[0].path.empty());
    Position target = moves[0].path.back();
    EXPECT_EQ(target.x, 4);
    EXPECT_EQ(target.y, 4);
    
    // Execute capture and verify results
    model.executeMove(moves[0]);
    auto board = model.getBoard();
    
    // Original position should be empty
    EXPECT_EQ(board[2][2], nullptr);
    
    // Captured piece position should be empty
    EXPECT_EQ(board[3][3], nullptr);
    
    // Final position should have the capturing piece
    EXPECT_NE(board[4][4], nullptr);
    EXPECT_EQ(board[4][4]->getColor(), "Player1");
}

// Test 3: Piece Promotion - mimics demonstratePiecePromotion()
TEST_F(GameScenarioTests, PiecePromotionTest) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[6][5] = new Piece("Player1", {6, 5}); // One move away from promotion
    grid[3][3] = new Piece("Player2", {3, 3}); // Opponent piece for context
    
    initializeGrid(grid);
    
    auto allMoves = model.getAllValidMoves();
    
    // Should have exactly one moveable piece
    EXPECT_EQ(allMoves.size(), 1);
    
    auto piecePos = Position{6, 5};
    ASSERT_TRUE(allMoves.find(piecePos) != allMoves.end());
    
    auto moves = allMoves[piecePos];
    
    // Should have promotion moves available
    EXPECT_GE(moves.size(), 1);
    
    // Find a promotion move (reaches row 7)
    Move promotionMove;
    bool hasPromotionMove = false;
    for (const auto& move : moves) {
        if (!move.path.empty() && move.path.back().x == 7) {
            promotionMove = move;
            hasPromotionMove = true;
            break;
        }
    }
    
    EXPECT_TRUE(hasPromotionMove);
    
    // Execute promotion move
    model.executeMove(promotionMove);
    auto board = model.getBoard();
    
    // Check that piece is now at promotion row
    Position finalPos = promotionMove.path.back();
    EXPECT_EQ(finalPos.x, 7);
    EXPECT_NE(board[7][finalPos.y], nullptr);
    EXPECT_EQ(board[7][finalPos.y]->getColor(), "Player1");
    
    // Check that piece is now a Dame
    EXPECT_TRUE(board[7][finalPos.y]->isDame());
    
    // Verify Dame has enhanced movement capabilities
    auto dameMoves = model.getAllValidMoves();
    if (dameMoves.find(finalPos) != dameMoves.end()) {
        auto dameMovelist = dameMoves[finalPos];
        // Dame should have more move options than a regular piece
        EXPECT_GT(dameMovelist.size(), 2);
    }
}

// Test 4: Dame Piece Move - mimics demonstrateDamePieceMove()
TEST_F(GameScenarioTests, DamePieceMoveTest) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[3][3] = new Piece("Player1", {3, 3}, Piece::Type::Dame);
    
    initializeGrid(grid);
    
    auto allMoves = model.getAllValidMoves();
    
    // Should have exactly one moveable piece
    EXPECT_EQ(allMoves.size(), 1);
    
    auto piecePos = Position{3, 3};
    ASSERT_TRUE(allMoves.find(piecePos) != allMoves.end());
    
    auto moves = allMoves[piecePos];
    
    // Dame should have many more moves than regular piece (can move any distance)
    EXPECT_GT(moves.size(), 8); // Should have moves in all 4 diagonal directions
    
    // Verify Dame can move multiple squares in each direction
    bool hasLongRangeMove = false;
    for (const auto& move : moves) {
        if (!move.path.empty() && !move.isCapture()) {
            Position target = move.path.back();
            int distance = abs(target.x - 3);
            if (distance > 1) {
                hasLongRangeMove = true;
                break;
            }
        }
    }
    
    EXPECT_TRUE(hasLongRangeMove);
}

// Test 5: Dame Piece Capture - mimics demonstrateDamePieceCapture()
TEST_F(GameScenarioTests, DamePieceCaptureTest) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2}, Piece::Type::Dame);
    grid[3][3] = new Piece("Player2", {3, 3});
    grid[5][5] = new Piece("Player2", {5, 5});
    
    initializeGrid(grid);
    
    auto allMoves = model.getAllValidMoves();
    
    // Should have exactly one moveable piece
    EXPECT_EQ(allMoves.size(), 1);
    
    auto piecePos = Position{2, 2};
    ASSERT_TRUE(allMoves.find(piecePos) != allMoves.end());
    
    auto moves = allMoves[piecePos];
    
    // Should have at least one move
    EXPECT_GE(moves.size(), 1);
    
    // Find multiple capture move
    Move multiCapture;
    bool hasMultiCapture = false;
    for (const auto& move : moves) {
        if (move.isCapture() && move.captureCount() > 1) {
            multiCapture = move;
            hasMultiCapture = true;
            break;
        }
    }
    
    EXPECT_TRUE(hasMultiCapture);
    
    if (hasMultiCapture) {
        EXPECT_EQ(multiCapture.captureCount(), 2);
        
        // Execute multiple capture
        model.executeMove(multiCapture);
        auto board = model.getBoard();
        
        // Both opponent pieces should be captured
        EXPECT_EQ(board[3][3], nullptr);
        EXPECT_EQ(board[5][5], nullptr);
        
        // Dame should be at final position
        Position finalPos = multiCapture.path.back();
        EXPECT_NE(board[finalPos.x][finalPos.y], nullptr);
        EXPECT_EQ(board[finalPos.x][finalPos.y]->getColor(), "Player1");
        EXPECT_TRUE(board[finalPos.x][finalPos.y]->isDame());
    }
}

// Test 6: Dame Multiple Captures - mimics demonstrateDameMultipleCaptures()
TEST_F(GameScenarioTests, DameMultipleCapturesTest) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    
    // Set up Dame at position (1,1)
    grid[1][1] = new Piece("Player1", {1, 1}, Piece::Type::Dame);
    
    // Set up opponent pieces for multiple captures
    grid[2][2] = new Piece("Player2", {2, 2}); // First capture target
    grid[4][4] = new Piece("Player2", {4, 4}); // Second capture target
    grid[6][6] = new Piece("Player2", {6, 6}); // Third capture target
    
    initializeGrid(grid);
    
    auto allMoves = model.getAllValidMoves();
    
    auto piecePos = Position{1, 1};
    ASSERT_TRUE(allMoves.find(piecePos) != allMoves.end());
    
    auto moves = allMoves[piecePos];
    
    // Should have at least one multiple capture move
    EXPECT_GE(moves.size(), 1);
    
    // Find the triple capture move
    Move tripleCapture;
    bool hasTripleCapture = false;
    for (const auto& move : moves) {
        if (move.isCapture() && move.captureCount() == 3) {
            tripleCapture = move;
            hasTripleCapture = true;
            break;
        }
    }
    
    EXPECT_TRUE(hasTripleCapture);
    
    if (hasTripleCapture) {
        // Verify capture path: (1,1) -> (3,3) -> (5,5) -> (7,7)
        EXPECT_EQ(tripleCapture.path.size(), 3);
        
        // Execute the triple capture
        model.executeMove(tripleCapture);
        auto board = model.getBoard();
        
        // All opponent pieces should be captured
        EXPECT_EQ(board[2][2], nullptr);
        EXPECT_EQ(board[4][4], nullptr);
        EXPECT_EQ(board[6][6], nullptr);
        
        // Dame should be at final position (7,7)
        EXPECT_NE(board[7][7], nullptr);
        EXPECT_EQ(board[7][7]->getColor(), "Player1");
        EXPECT_TRUE(board[7][7]->isDame());
    }
}

// Test 7: Dame Mandatory Capture - mimics demonstrateDameMandatoryCapture()
TEST_F(GameScenarioTests, DameMandatoryCaptureTest) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    
    // Set up Dame with both regular move and capture options
    grid[3][3] = new Piece("Player1", {3, 3}, Piece::Type::Dame);
    grid[4][4] = new Piece("Player2", {4, 4}); // Must capture this
    
    initializeGrid(grid);
    
    auto allMoves = model.getAllValidMoves();
    
    auto piecePos = Position{3, 3};
    ASSERT_TRUE(allMoves.find(piecePos) != allMoves.end());
    
    auto moves = allMoves[piecePos];
    
    // All available moves should be captures (mandatory capture rule)
    EXPECT_GE(moves.size(), 1);
    
    for (const auto& move : moves) {
        EXPECT_TRUE(move.isCapture()) << "All moves should be captures when capture is available";
    }
    
    // Find the mandatory capture
    Move mandatoryCapture;
    bool hasCapture = false;
    for (const auto& move : moves) {
        if (move.isCapture() && move.captureCount() == 1) {
            mandatoryCapture = move;
            hasCapture = true;
            break;
        }
    }
    
    EXPECT_TRUE(hasCapture);
    
    if (hasCapture) {
        // Execute mandatory capture
        model.executeMove(mandatoryCapture);
        auto board = model.getBoard();
        
        // Captured piece should be removed
        EXPECT_EQ(board[4][4], nullptr);
        
        // Dame should be at capture destination (5,5)
        EXPECT_NE(board[5][5], nullptr);
        EXPECT_EQ(board[5][5]->getColor(), "Player1");
        EXPECT_TRUE(board[5][5]->isDame());
    }
}

// Test 8: Dame Unlimited Movement - mimics demonstrateDameUnlimitedMovement()
TEST_F(GameScenarioTests, DameUnlimitedMovementTest) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    
    // Place Dame at center of board
    grid[3][3] = new Piece("Player1", {3, 3}, Piece::Type::Dame);
    
    // Place some pieces to show movement boundaries
    grid[6][0] = new Piece("Player2", {6, 0}); // Far opponent piece
    grid[1][7] = new Piece("Player1", {1, 7}); // Own piece to show blocking
    
    initializeGrid(grid);
    
    auto allMoves = model.getAllValidMoves();
    
    auto piecePos = Position{3, 3};
    ASSERT_TRUE(allMoves.find(piecePos) != allMoves.end());
    
    auto moves = allMoves[piecePos];
    
    // Dame should have many movement options
    EXPECT_GT(moves.size(), 8);
    
    // Check that Dame can move in all four diagonal directions
    bool hasUpRight = false, hasUpLeft = false, hasDownRight = false, hasDownLeft = false;
    int maxDistance = 0;
    
    for (const auto& move : moves) {
        if (!move.isCapture() && !move.path.empty()) {
            Position target = move.path.back();
            int dx = target.x - 3;
            int dy = target.y - 3;
            int distance = abs(dx);
            
            maxDistance = std::max(maxDistance, distance);
            
            if (dx > 0 && dy > 0) hasDownRight = true;
            if (dx > 0 && dy < 0) hasDownLeft = true;
            if (dx < 0 && dy > 0) hasUpRight = true;
            if (dx < 0 && dy < 0) hasUpLeft = true;
        }
    }
    
    // Dame should be able to move in all four diagonal directions
    EXPECT_TRUE(hasUpRight);
    EXPECT_TRUE(hasUpLeft);
    EXPECT_TRUE(hasDownRight);
    EXPECT_TRUE(hasDownLeft);
    
    // Dame should be able to move more than one square
    EXPECT_GT(maxDistance, 1);
    
    // Test that Dame can move to corner if path is clear
    bool canReachCorner = false;
    for (const auto& move : moves) {
        if (!move.isCapture() && !move.path.empty()) {
            Position target = move.path.back();
            if ((target.x == 0 && target.y == 0) || 
                (target.x == 7 && target.y == 7) ||
                (target.x == 0 && target.y == 6) || // Blocked by own piece at (1,7)
                (target.x == 6 && target.y == 0)) {
                canReachCorner = true;
                break;
            }
        }
    }
    
    EXPECT_TRUE(canReachCorner);
}

// Test 9: Game Statistics and Evaluation
TEST_F(GameScenarioTests, GameStatisticsTest) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    
    // Set up a scenario with mixed pieces
    grid[1][1] = new Piece("Player1", {1, 1});
    grid[2][2] = new Piece("Player1", {2, 2}, Piece::Type::Dame);
    grid[5][5] = new Piece("Player2", {5, 5});
    grid[6][6] = new Piece("Player2", {6, 6}, Piece::Type::Dame);
    
    initializeGrid(grid);
    
    // Test piece counting
    EXPECT_EQ(model.getPieceCount("Player1"), 2);
    EXPECT_EQ(model.getPieceCount("Player2"), 2);
}

// Test 10: Current Player Management
TEST_F(GameScenarioTests, CurrentPlayerTest) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[3][3] = new Piece("Player1", {3, 3});
    grid[5][5] = new Piece("Player2", {5, 5});
    
    initializeGrid(grid);
    
    // Check initial current player
    std::string initialPlayer = model.getCurrentPlayer();
    EXPECT_TRUE(initialPlayer == "Player1" || initialPlayer == "Player2");
    
    // Get a valid move for current player
    auto allMoves = model.getAllValidMoves();
    EXPECT_GT(allMoves.size(), 0);
    
    // Execute a move
    auto firstMove = allMoves.begin()->second[0];
    model.executeMove(firstMove);
    
    // Current player should have changed
    std::string newPlayer = model.getCurrentPlayer();
    EXPECT_NE(initialPlayer, newPlayer);
}
