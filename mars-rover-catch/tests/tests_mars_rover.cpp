#include "rover.hpp"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/trompeloeil.hpp>
#include <ranges>
#include <string>

using namespace std;
using namespace TDD;

class DummyObstacleDetector : public ObstacleDetector
{
public:
    bool detect_obstacle(const Coordinates& coord) const override
    {
        return false;
    }
};

TEST_CASE("rover reports its position and orientation")
{
    Rover rover{Position{0, 0, 'N'}, std::make_unique<DummyObstacleDetector>()};

    REQUIRE(rover.position() == Position(0, 0, 'N'));
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

    Rover rover{start, std::make_unique<DummyObstacleDetector>()};

    rover.turn_left();

    REQUIRE(rover.position() == end);
}

TEST_CASE("turn right")
{
    auto position = GENERATE(
        StartEndPos{{0, 0, 'N'}, {0, 0, 'E'}},
        StartEndPos{{0, 0, 'E'}, {0, 0, 'S'}},
        StartEndPos{{0, 0, 'S'}, {0, 0, 'W'}},
        StartEndPos{{0, 0, 'W'}, {0, 0, 'N'}});

    auto [start, end] = position;

    Rover rover{start, std::make_unique<DummyObstacleDetector>()};

    rover.turn_right();

    CHECK(rover.position() == end);
}

TEST_CASE("move forward")
{
    auto position = GENERATE(
        StartEndPos{{0, 0, 'N'}, {0, 1, 'N'}},
        StartEndPos{{0, 0, 'E'}, {1, 0, 'E'}},
        StartEndPos{{0, 0, 'S'}, {0, -1, 'S'}},
        StartEndPos{{0, 0, 'W'}, {-1, 0, 'W'}});

    auto [start, end] = position;

    Rover rover{start, std::make_unique<DummyObstacleDetector>()};

    rover.move_forward();

    REQUIRE(rover.position() == end);
}

TEST_CASE("move backward")
{
    auto position = GENERATE(
        StartEndPos{{0, 0, 'N'}, {0, -1, 'N'}},
        StartEndPos{{0, 0, 'E'}, {-1, 0, 'E'}},
        StartEndPos{{0, 0, 'S'}, {0, 1, 'S'}},
        StartEndPos{{0, 0, 'W'}, {1, 0, 'W'}});

    auto [start, end] = position;

    Rover rover{start, std::make_unique<DummyObstacleDetector>()};

    rover.move_backward();

    REQUIRE(rover.position() == end);
}

TEST_CASE("rover executes set of commands")
{
    Rover rover{Position{0, 0, 'N'}, std::make_unique<DummyObstacleDetector>()};

    SECTION("returning final position")
    {
        auto commands_end = GENERATE(
            std::pair{"FFRFF", Position{2, 2, 'E'}},
            std::pair{"FFRFFBFLR", Position{2, 2, 'E'}},
            std::pair{"FFRFFLFFRFFLFF", Position{4, 6, 'N'}},
            std::pair{"FFRFFLFFRFFLFFRFFLFF", Position{6, 8, 'N'}},
            std::pair{"FFRFFLFFRFBBFLFBBFRFFLFF", Position{4, 6, 'N'}});

        auto [commands, end] = commands_end;

        Position result = rover.go(commands);

        REQUIRE(result == end);
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

        Position result = rover.go(commands);

        REQUIRE(result == end);
    }

    SECTION("at unknown commands")
    {

        SECTION("exception is thrown")
        {
            auto cmd = "FFRFFLFFxLLLL";

            try
            {
                rover.go(cmd);
            }
            catch (const UnknownCommand& e)
            {
                REQUIRE(e == UnknownCommand{"x", "FFRFFLFFxLLLL"});
            }
            catch (...)
            {
                FAIL();
            }
        }

        SECTION("rover stops after last known command")
        {
            auto cmd = "FFRFFLFFxLLLL";

            CHECK_THROWS(rover.go(cmd));

            REQUIRE(rover.position() == Position{2, 4, 'N'});
        }
    }
}

TEST_CASE("rover wraps coordinates on the map")
{
    Grid grid{10, 10};

    auto params = GENERATE(
        std::tuple{Position{0, 9, 'N'}, "F", Position{0, 0, 'N'}},
        std::tuple{Position{0, 0, 'N'}, "B", Position{0, 9, 'N'}},
        std::tuple{Position{9, 0, 'E'}, "F", Position{0, 0, 'E'}},
        std::tuple{Position{0, 0, 'E'}, "B", Position{9, 0, 'E'}});

    auto [start_pos, cmd, end_pos] = params;

    Rover rover{start_pos, std::make_unique<DummyObstacleDetector>(), grid};

    Position result = rover.go(cmd);

    REQUIRE(result == end_pos);
}

TEST_CASE("Grid - wrapping")
{
    Grid grid{10, 10};

    Position pos{-5, -13, 'N'};

    Position result = grid.wrap(pos);

    REQUIRE(result == Position{5, 7, 'N'});
}

class ObstacleDetectorMock : public ObstacleDetector
{
public:
    MAKE_CONST_MOCK1(detect_obstacle, bool(const Coordinates&), override);
};

TEST_CASE("detecting obstacles")
{
    auto detector = std::make_unique<ObstacleDetectorMock>();

    SECTION("when there is no obstacle rover moves forward")
    {
        REQUIRE_CALL(*detector, detect_obstacle(Coordinates{0, 1})).RETURN(false);

        Rover rover{Position{0, 0, 'N'}, std::move(detector)};
        Position result = rover.go("F");

        REQUIRE(result == Position{0, 1, 'N'});
    }

    SECTION("when there is an obstacle exception is thrown")
    {
        REQUIRE_CALL(*detector, detect_obstacle(Coordinates{0, 1})).RETURN(true);

        Rover rover{Position{0, 0, 'N'}, std::move(detector)};

        REQUIRE_THROWS_AS(rover.go("F"), ObstacleDetected);
    }
}