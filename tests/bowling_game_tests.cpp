#include "bowling_game.hpp"
#include "gtest/gtest.h"

TEST(BowlingGameTests, NewGameScore)
{
    BowlingGame game;

    ASSERT_EQ(game.score(), 0);
}