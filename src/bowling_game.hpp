#ifndef BOWLING_GAME_HPP
#define BOWLING_GAME_HPP

#include <algorithm>
#include <array>

class BowlingGame
{
    unsigned int score_ = 0;

public:
    unsigned int score() const
    {
        return score_;
    }

    void roll(unsigned int pins)
    {
        score_ += pins;
    }
};

#endif
