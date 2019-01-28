#include "bowling_game.hpp"
#include "gtest/gtest.h"

TEST(BowlingGameTests, NewGameScore)
{
    BowlingGame game;

    ASSERT_EQ(game.score(), 0);
}

TEST(BowlingGameTests, AllRollsInAGutter)
{
    BowlingGame game;

    for(int i = 0; i < 20; ++i)
        game.roll(0);
    
    ASSERT_EQ(game.score(), 0);
}