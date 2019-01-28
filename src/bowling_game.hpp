#ifndef BOWLING_GAME_HPP
#define BOWLING_GAME_HPP

#include <algorithm>
#include <array>

class BowlingGame
{
    std::array<unsigned int, 20> pins_ = {{}};
    size_t roll_no = 0;

public:
    unsigned int score() const
    {
        unsigned int result{};

        for(size_t i = 0; i < 20; i+=2)
        {
            if (pins_[i] + pins_[i+1] == 10)
                result += pins_[i + 2];            

            result += pins_[i] + pins_[i+1];
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
