#include "bowling_game.hpp"
#include "gtest/gtest.h"

class BowlingGameTests : public ::testing::Test
{
protected:
    BowlingGame game;

    void roll_many(unsigned int count, unsigned int pins)
    {
        for (auto i = 0u; i < count; ++i)
            game.roll(pins);
    }

    void roll_spare()
    {
        game.roll(1);
        game.roll(9);
    }
};

TEST_F(BowlingGameTests, NewGameScore)
{
    ASSERT_EQ(game.score(), 0);
}

TEST_F(BowlingGameTests, AllRollsInAGutter)
{
    roll_many(20, 0);

    ASSERT_EQ(game.score(), 0);
}

TEST_F(BowlingGameTests, WhenAllRollsNoMarkScoreIsSumOfPins)
{
    roll_many(20, 2);

    ASSERT_EQ(game.score(), 40);
}

TEST_F(BowlingGameTests, Spare)
{
    roll_spare();

    roll_many(18, 1);

    ASSERT_EQ(game.score(), 29);
}