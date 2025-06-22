#include <gtest/gtest.h>
#include "Board.h"
#include "Piece.h"
#include <vector>
#include <set>

class StatesTest : public ::testing::Test {
protected:
    Board board;
};

TEST_F(StatesTest, SimpleMove) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[3][3] = new Piece("Player1", {3, 3});
    board.initialize(grid);
    
    States states = board.getStates(*grid[3][3]);
    auto choices = states.getChoices();
    
    ASSERT_EQ(choices.size(), 2);
    std::set<std::pair<int, int>> destinations;
    for (const auto& path : choices) {
        ASSERT_EQ(path.size(), 1);
        destinations.insert({path[0].x, path[0].y});
    }
    
    std::set<std::pair<int, int>> expected_destinations = {{4, 2}, {4, 4}};
    ASSERT_EQ(destinations, expected_destinations);
}

TEST_F(StatesTest, CaptureMove) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[3][3] = new Piece("Player2", {3, 3});
    board.initialize(grid);

    States states = board.getStates(*grid[2][2]);
    auto choices = states.getChoices();

    ASSERT_EQ(choices.size(), 1);
    ASSERT_EQ(choices[0].size(), 1);
    EXPECT_EQ(choices[0][0].x, 4);
    EXPECT_EQ(choices[0][0].y, 4);
}

TEST_F(StatesTest, MultipleCaptureChain) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[3][3] = new Piece("Player2", {3, 3});
    grid[5][5] = new Piece("Player2", {5, 5});
    board.initialize(grid);

    States states = board.getStates(*grid[2][2]);
    auto choices = states.getChoices();

    ASSERT_EQ(choices.size(), 1);
    ASSERT_EQ(choices[0].size(), 2);
    EXPECT_EQ(choices[0][0].x, 4);
    EXPECT_EQ(choices[0][0].y, 4);
    EXPECT_EQ(choices[0][1].x, 6);
    EXPECT_EQ(choices[0][1].y, 6);
}

TEST_F(StatesTest, MultiCaptureChoices) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[2][2] = new Piece("Player1", {2, 2});
    grid[3][1] = new Piece("Player2", {3, 1});
    grid[3][3] = new Piece("Player2", {3, 3});
    board.initialize(grid);

    States states = board.getStates(*grid[2][2]);
    auto choices = states.getChoices();

    ASSERT_EQ(choices.size(), 2);
    std::set<std::pair<int, int>> destinations;
    for (const auto& path : choices) {
        ASSERT_EQ(path.size(), 1);
        destinations.insert({path[0].x, path[0].y});
    }

    std::set<std::pair<int, int>> expected_destinations = {{4, 0}, {4, 4}};
    ASSERT_EQ(destinations, expected_destinations);
}

TEST_F(StatesTest, ChainCaptureWithMultipleChoices) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[0][0] = new Piece("Player1", {0, 0});
    grid[1][1] = new Piece("Player2", {1, 1});
    grid[3][3] = new Piece("Player2", {3, 3});
    grid[3][1] = new Piece("Player2", {3, 1});
    board.initialize(grid);

    States states = board.getStates(*grid[0][0]);
    auto choices = states.getChoices();

    ASSERT_EQ(choices.size(), 2);
    std::set<std::vector<std::pair<int, int>>> paths;
    for (const auto& path : choices) {
        std::vector<std::pair<int, int>> p;
        for (const auto& pos : path) {
            p.push_back({pos.x, pos.y});
        }
        paths.insert(p);
    }

    std::set<std::vector<std::pair<int, int>>> expected_paths = {
        {{2, 2}, {4, 0}},
        {{2, 2}, {4, 4}}
    };
    ASSERT_EQ(paths, expected_paths);
}

TEST_F(StatesTest, NoMovesAvailable) {
    std::vector<std::vector<Piece*>> grid(8, std::vector<Piece*>(8, nullptr));
    grid[4][4] = new Piece("Player1", {4, 4});
    grid[5][5] = new Piece("Player1", {5, 5}); // Friendly piece, blocks capture
    grid[5][3] = new Piece("Player2", {5, 3}); // Enemy piece
    grid[6][2] = new Piece("Player2", {6, 2}); // Enemy piece blocking the capture of the other enemy
    board.initialize(grid);

    States states = board.getStates(*grid[4][4]);
    auto choices = states.getChoices();

    ASSERT_TRUE(choices.empty());
}
