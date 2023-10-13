#include "source.hpp"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <ranges>
#include <string>

using namespace std;

struct Coordinates
{
    int x;
    int y;

    bool operator==(const Coordinates& other) const = default;

    friend std::ostream& operator<<(std::ostream& out, const Coordinates& coord)
    {
        return out << "Coordinates{" << coord.x << ", " << coord.y << "}";
    }
};

struct Position
{
    static constexpr std::array<const char, 4> orientations = {'N', 'E', 'S', 'W'};

    Coordinates coordinates_;
    char orientation_;

    constexpr size_t orientation_index() const
    {
        auto index = std::ranges::find(orientations, orientation_) - orientations.begin();
        assert(index != orientations.size());

        return index;
    }

public:
    constexpr Position(int x, int y, char orientation)
        : coordinates_{x, y}
        , orientation_{orientation}
    {
    }

    constexpr Position(Coordinates coordinates, char orientation)
        : coordinates_{coordinates}
        , orientation_{orientation}
    {
    }

    constexpr Position next_clockwise() const
    {
        auto index = orientation_index();
        auto next_index = (index + 1) % orientations.size();

        return Position{coordinates_, orientations[next_index]};
    }

    constexpr Position next_counter_clockwise() const
    {
        auto index = orientation_index();
        auto next_index = (index - 1) % orientations.size();

        return Position{coordinates_, orientations[next_index]};
    }

    Position move_forward() const
    {
        static constexpr std::array<Coordinates, 4> deltas = {{
            {0, 1},  // North
            {1, 0},  // East
            {0, -1}, // South
            {-1, 0}  // West
        }};

        auto index = orientation_index();

        auto [dx, dy] = deltas[index];

        return Position(coordinates_.x + dx,
            coordinates_.y + dy,
            orientation_);
    }

    Position move_backward() const
    {
        static constexpr std::array<Coordinates, 4> deltas = {{
            {0, -1}, // North
            {-1, 0}, // East
            {0, 1},  // South
            {1, 0}   // West
        }};

        auto index = orientation_index();

        auto [dx, dy] = deltas[index];

        return Position(coordinates_.x + dx, coordinates_.y + dy, orientation_);
    }

    Coordinates coordinates() const { return coordinates_; }

    char orientation() const { return orientation_; }

    bool operator==(const Position& other) const = default;

    friend std::ostream& operator<<(std::ostream& out, const Position& pos)
    {
        return out << "Position(" << pos.coordinates() << ", " << pos.orientation() << ")";
    }
};

struct UnknownCommand : public std::invalid_argument
{
    std::string unknown_command;
    std::string last_commands;

    UnknownCommand(const std::string& unknown_command, const std::string& last_commands )
        : std::invalid_argument("Unknown command: " + unknown_command + " in " + last_commands)
        , unknown_command{unknown_command}
        , last_commands{last_commands}
    {
    }

    bool operator==(const UnknownCommand& other) const
    {
        return unknown_command == other.unknown_command && last_commands == other.last_commands;
    } 
};

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

    void move_forward()
    {
        position_ = position_.move_forward();
    }

    void move_backward()
    {
        position_ = position_.move_backward();
    }

    Position go(const std::string& commands)
    {
        for (auto command : commands)
        {
            switch (std::toupper(command))
            {
            case 'F':
                move_forward();
                break;
            case 'B':
                move_backward();
                break;
            case 'L':
                turn_left();
                break;
            case 'R':
                turn_right();
                break;
            default:
                throw UnknownCommand{std::string{command}, commands};                
            }
        }

        return get_position();
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
        StartEndPos{{0, 0, 'E'}, {0, 0, 'N'}});

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
        StartEndPos{{0, 0, 'W'}, {0, 0, 'N'}});

    auto [start, end] = position;

    Rover rover{start};

    rover.turn_right();

    CHECK(rover.get_position() == end);
}

TEST_CASE("move forward")
{
    auto position = GENERATE(
        StartEndPos{{0, 0, 'N'}, {0, 1, 'N'}},
        StartEndPos{{0, 0, 'E'}, {1, 0, 'E'}},
        StartEndPos{{0, 0, 'S'}, {0, -1, 'S'}},
        StartEndPos{{0, 0, 'W'}, {-1, 0, 'W'}});

    auto [start, end] = position;

    Rover rover{start};

    rover.move_forward();

    CHECK(rover.get_position() == end);
}

TEST_CASE("move backward")
{
    auto position = GENERATE(
        StartEndPos{{0, 0, 'N'}, {0, -1, 'N'}},
        StartEndPos{{0, 0, 'E'}, {-1, 0, 'E'}},
        StartEndPos{{0, 0, 'S'}, {0, 1, 'S'}},
        StartEndPos{{0, 0, 'W'}, {1, 0, 'W'}});

    auto [start, end] = position;

    Rover rover{start};

    rover.move_backward();

    CHECK(rover.get_position() == end);
}

TEST_CASE("rover executes set of commands")
{
    SECTION("returning final position")
    {
        auto commands_end = GENERATE(
            std::pair{"FFRFF", Position{2, 2, 'E'}},
            std::pair{"FFRFFBFLR", Position{2, 2, 'E'}},
            std::pair{"FFRFFLFFRFFLFF", Position{4, 6, 'N'}},
            std::pair{"FFRFFLFFRFFLFFRFFLFF", Position{6, 8, 'N'}},
            std::pair{"FFRFFLFFRFBBFLFBBFRFFLFF", Position{4, 6, 'N'}});

        auto [commands, end] = commands_end;

        Rover rover{0, 0, 'N'};

        Position result = rover.go(commands);

        CHECK(result == end);
    }

    SECTION("commands are case insensitive")
    {
        auto commands_end = GENERATE(
            std::pair{"ffRff", Position{2, 2, 'E'}},
            std::pair{"ffrffBflr", Position{2, 2, 'E'}},
            std::pair{"ffrfflffrfflff", Position{4, 6, 'N'}},
            std::pair{"ffrFFlffrfflffrfflff", Position{6, 8, 'N'}},
            std::pair{"ffrfflffrfbbflfbbfrfflff", Position{4, 6, 'N'}});

        auto [commands, end] = commands_end;

        Rover rover{0, 0, 'N'};

        Position result = rover.go(commands);

        CHECK(result == end);
    }

    SECTION("at unknown commands")
    {
        Rover rover{0, 0, 'N'};
        
        SECTION("exception is thrown")
        {

            auto cmd = "FFRFFLFFxLLLL";

            try
            {
                rover.go(cmd);
            }
            catch(const UnknownCommand& e)
            {
                REQUIRE(e == UnknownCommand{"x", "FFRFFLFFxLLLL"});
            }
            catch(...)
            {
                FAIL("Unknown exception");
            }
        }

        SECTION("rover stops after last known command")
        {
            auto cmd = "FFRFFLFFxLLLL";

            REQUIRE_THROWS(rover.go(cmd));

            CHECK(rover.get_position() == Position{2, 4, 'N'});
        }
    }
}