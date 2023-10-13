#include "source.hpp"

#include <array>
#include <ranges>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

using namespace std;

struct Position
{
    static constexpr std::array<const char, 4> orientations = {'N', 'E', 'S', 'W'};

    int x;
    int y;
    char orientation;

    constexpr Position next_clockwise() const
    {
        auto it = std::ranges::find(orientations, orientation);
        assert(it != orientations.end());

        auto next = std::next(it);
        if (next == orientations.end())
        {
            next = orientations.begin();
        }
        
        return Position{x, y, *next};
    }

    constexpr Position next_counter_clockwise() const
    {
        auto it = std::ranges::find(orientations, orientation);
        assert(it != orientations.end());

        if (it == orientations.begin())
        {
            it = orientations.end();
        }
        auto prev = std::prev(it);
        
        return Position{x, y, *prev};
    }

    bool operator==(const Position& other) const = default;
};

std::ostream& operator<<(std::ostream& out, const Position& pos)
{
    return out << "Position(" << pos.x << ", " << pos.y << ", " << pos.orientation << ")";
}


// refactor this removing the switch statement
// and using a map
class Rover
{
    Position position_;

public:
    Rover(int x, int y, char orientation)
        : position_{x, y, orientation}
    {
    }

    Rover(Position position)
        : position_{position}
    {
    }

    Position get_position() const
    {
        return position_;
    }

    void turn_left()
    {
        position_ = position_.next_counter_clockwise();
    }

    void turn_right()
    {
        position_ = position_.next_clockwise();        
    }
};

TEST_CASE("rover reports its position and orientation")
{
    Rover rover{Position{0, 0, 'N'}};

    CHECK(rover.get_position() == Position(0, 0, 'N'));
}

using StartEndPos = std::pair<Position, Position>;

TEST_CASE("turn left")
{
    auto position = GENERATE(
        StartEndPos{{0, 0, 'N'}, {0, 0, 'W'}},
        StartEndPos{{0, 0, 'W'}, {0, 0, 'S'}},
        StartEndPos{{0, 0, 'S'}, {0, 0, 'E'}},
        StartEndPos{{0, 0, 'E'}, {0, 0, 'N'}}
    );

    auto [start, end] = position;

    Rover rover{start};

    rover.turn_left();

    CHECK(rover.get_position() == end);
}

TEST_CASE("turn right")
{
    auto position = GENERATE(
        StartEndPos{{0, 0, 'N'}, {0, 0, 'E'}},
        StartEndPos{{0, 0, 'E'}, {0, 0, 'S'}},
        StartEndPos{{0, 0, 'S'}, {0, 0, 'W'}},
        StartEndPos{{0, 0, 'W'}, {0, 0, 'N'}}
    );

    auto [start, end] = position;

    Rover rover{start};

    rover.turn_right();

    CHECK(rover.get_position() == end);
}