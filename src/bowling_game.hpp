#ifndef BOWLING_GAME_HPP
#define BOWLING_GAME_HPP

#include <algorithm>
#include <array>

class BowlingGame
{
    std::array<unsigned int, 20> pins_ = {{}};
    size_t roll_no = 0;

    bool is_spare(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index + 1] == 10;
    }

    size_t spare_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 2];
    }

    bool is_strike(size_t roll_index) const
    {
        return pins_[roll_index] == 10;
    }

    size_t strike_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 1] + pins_[roll_index + 2];
    }

    size_t frame_score(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index + 1];
    }

public:
    unsigned int score() const
    {
        size_t result{};
        size_t roll_index = 0;

        for (size_t i = 0; i < 10; ++i)
        {
            if (is_strike(roll_index))
            {
                result += 10 + strike_bonus(roll_index);
                ++roll_index;
            }
            else
            {
                result += frame_score(roll_index);
                if (is_spare(roll_index))
                    result += spare_bonus(roll_index);

                roll_index += 2;
            }
        }

        return result;
    }

    void roll(unsigned int pins)
    {
        pins_[roll_no] = pins;
        ++roll_no;
    }
};

#endif
