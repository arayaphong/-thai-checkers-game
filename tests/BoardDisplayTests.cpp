#include <gtest/gtest.h>
#include "Board.h"
#include <sstream>
#include <vector>
#include <string>

class BoardDisplayTests : public ::testing::Test {
protected:
    Board board;
};

TEST_F(BoardDisplayTests, DisplayDefaultBoard) {
    // Initialize default board
    board.initialize("Player1", "Player2");
    // Capture stdout
    ::testing::internal::CaptureStdout();
    board.display();
    std::string output = ::testing::internal::GetCapturedStdout();
    
    // Split output into lines
    std::istringstream iss(output);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }
    
    // Should have 1 header + 8 rows
    ASSERT_EQ(lines.size(), 9);
    // Header with column indices
    EXPECT_EQ(lines[0], "  0 1 2 3 4 5 6 7 ");
    // Row 0
    EXPECT_EQ(lines[1], "0 X . X . X . X . ");
    // Row 1
    EXPECT_EQ(lines[2], "1 . X . X . X . X ");
    // Row 6
    EXPECT_EQ(lines[7], "6 O . O . O . O . ");
    // Row 7
    EXPECT_EQ(lines[8], "7 . O . O . O . O ");
}

TEST_F(BoardDisplayTests, DisplayCustomGrid) {
    // Create empty grid and place custom pieces
    std::vector<std::vector<Piece*>> customGrid(8, std::vector<Piece*>(8, nullptr));
    customGrid[0][0] = new Piece("P1", {0, 0});
    customGrid[7][7] = new Piece("P2", {7, 7});
    board.initialize(customGrid);

    ::testing::internal::CaptureStdout();
    board.display();
    std::string output = ::testing::internal::GetCapturedStdout();
    
    std::istringstream iss(output);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    // Should have 1 header + 8 rows
    ASSERT_EQ(lines.size(), 9);
    // Header
    EXPECT_EQ(lines[0], "  0 1 2 3 4 5 6 7 ");
    // P1 at (0,0)
    EXPECT_EQ(lines[1], "0 X . . . . . . . ");
    // Rows 1 through 6 are empty
    for (int i = 2; i <= 7; ++i) {
        EXPECT_EQ(lines[i], std::to_string(i-1) + " . . . . . . . . ");
    }
    // P2 at (7,7)
    EXPECT_EQ(lines[8], "7 . . . . . . . O ");
}
