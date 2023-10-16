#ifndef BOWLING_HPP
#define BOWLING_HPP

#include <array>
#include <tuple>
#include <cstddef>


class BowlingGame
{
public:
    size_t score() const
    {
        size_t result{};
        size_t roll_index = 0;

        for(size_t i = 0; i < frame_count; ++i)
        {
            size_t pins_in_frame, rolls_in_frame;
            std::tie(pins_in_frame, rolls_in_frame) = frame_score(roll_index);

            result += pins_in_frame;

            if (is_strike(roll_index))            
                result += strike_bonus(roll_index);                
            else if (is_spare(roll_index))
                result += spare_bonus(roll_index);                
            
            roll_index += rolls_in_frame;
        }

        return result;
    }

    void roll(size_t pins)
    {
        pins_[throw_no_++] = pins;
    }

private:
    constexpr static size_t all_pins_in_frame = 10;
    constexpr static size_t frame_count = 10;

    std::array<size_t, 21> pins_ = {};
    size_t throw_no_ = 0;

    bool is_strike(size_t roll_index) const
    {
        return pins_[roll_index] == all_pins_in_frame;
    }

    size_t strike_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 1] + pins_[roll_index + 2];
    }

    bool is_spare(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index + 1] == all_pins_in_frame;
    }

    size_t spare_bonus(size_t roll_index) const
    {
        return pins_[roll_index + 2];
    }

    std::pair<size_t, size_t> frame_score(size_t roll_index) const
    {
        if (is_strike(roll_index))
            return { pins_[roll_index], 1 };
        else
            return { pins_[roll_index] + pins_[roll_index + 1], 2 };
    }
};

#endif