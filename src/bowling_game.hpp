#ifndef BOWLING_GAME_HPP
#define BOWLING_GAME_HPP

#include <algorithm>
#include <array>

class BowlingGame
{
    constexpr static size_t frames_count = 10;
    constexpr static size_t all_pins_in_frame = 10;

    std::array<unsigned int, 21> pins_ = {{}};
    size_t roll_no = 0;

    bool is_spare(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index + 1] == all_pins_in_frame;
    }

    size_t spare_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 2];
    }

    bool is_strike(size_t roll_index) const
    {
        return pins_[roll_index] == all_pins_in_frame;
    }

    size_t strike_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 1] + pins_[roll_index + 2];
    }

    auto frame_score(size_t roll_index) const
    {
        size_t score_in_frame = 0;
        size_t rolls_in_frame = 2;

        if (is_strike(roll_index))
        {
            score_in_frame += all_pins_in_frame + strike_bonus(roll_index);
            rolls_in_frame = 1;
        }
        else
        {
            score_in_frame = pins_[roll_index] + pins_[roll_index + 1];

            if (is_spare(roll_index))
            {
                score_in_frame += spare_bonus(roll_index);
            }
        }

        return std::pair{score_in_frame, rolls_in_frame};
    }

public:
    unsigned int score() const
    {
        size_t result{};
        size_t roll_index = 0;

        for (size_t i = 0; i < frames_count; ++i)
        {
            auto [score_in_frame, rolls_in_frame] = frame_score(roll_index);

            result += score_in_frame;
            roll_index += rolls_in_frame;
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
