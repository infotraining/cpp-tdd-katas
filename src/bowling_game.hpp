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

    size_t frame_score(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index + 1];
    }

public:
    unsigned int score() const
    {
        unsigned int result{};

        for (size_t i = 0; i < 20; i += 2)
        {
            result += frame_score(i);

            if (is_spare(i))
                result += spare_bonus(i);
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
