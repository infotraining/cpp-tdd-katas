#include "source.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

using namespace std;

struct Position
{
    int x;
    int y;
    char orientation;

    bool operator==(const Position& other) const = default;
};

std::ostream& operator<<(std::ostream& out, const Position& pos)
{
    return out << "Position(" << pos.x << ", " << pos.y << ", " << pos.orientation << ")";
}

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
        // refactor this removing the switch statement
        switch (position_.orientation)
        {
        case 'N':
            position_.orientation = 'W';
            break;
        case 'W':
            position_.orientation = 'S';
            break;
        case 'S':
            position_.orientation = 'E';
            break;
        case 'E':
            position_.orientation = 'N';
            break;
        }
    }

    void turn_right()
    {
        switch (position_.orientation)
        {
        case 'N':
            position_.orientation = 'E';
            break;
        case 'E':
            position_.orientation = 'S';
            break;
        case 'S':
            position_.orientation = 'W';
            break;
        case 'W':
            position_.orientation = 'N';
            break;
        }
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
