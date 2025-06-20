#include <gtest/gtest.h>
#include "Piece.h"

class PieceTests : public ::testing::Test {
protected:
    Piece* piece;

    void SetUp() override {
        piece = new Piece("Red", {0, 0}); // Initialize a red piece at position (0, 0)
    }

    void TearDown() override {
        delete piece;
    }
};

TEST_F(PieceTests, GetPositionReturnsCorrectPosition) {
    EXPECT_EQ(piece->getPosition().x, 0);
    EXPECT_EQ(piece->getPosition().y, 0);
}

TEST_F(PieceTests, SetPositionUpdatesPosition) {
    piece->setPosition({1, 1});
    EXPECT_EQ(piece->getPosition().x, 1);
    EXPECT_EQ(piece->getPosition().y, 1);
}

TEST_F(PieceTests, GetColorReturnsCorrectColor) {
    EXPECT_EQ(piece->getColor(), "Red");
}