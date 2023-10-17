#include "bowling.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <numeric>

using namespace std;

void roll_many(BowlingGame& game, size_t count, size_t pins)
{
    for (size_t i = 0; i < count; ++i)
    {
        game.roll(pins);
    }
}

void roll_spare(BowlingGame& game)
{
    game.roll(1);
    game.roll(9);
}

void roll_strike(BowlingGame& game)
{
    game.roll(10);
}

TEST_CASE("When game starts score is zero", "[score]")
{
    BowlingGame game;

    REQUIRE(game.score() == 0);
}

TEST_CASE("When all rolls in gutter score is zero", "[score]")
{
    BowlingGame game;
    roll_many(game, 20, 0);

    REQUIRE(game.score() == 0);
}

TEST_CASE("When all rolls no mark score is sum of pins", "[score]")
{
    BowlingGame game;
    roll_many(game, 20, 2);

    REQUIRE(game.score() == 40);
}

TEST_CASE("When spare next throw is counted twice", "[score]")
{
    BowlingGame game;

    roll_spare(game);
    roll_many(game, 18, 1);

    REQUIRE(game.score() == 29);
}

TEST_CASE("When strike next two throws are counted twice", "[score]")
{
    BowlingGame game;

    roll_many(game, 2, 1);
    roll_strike(game);
    roll_many(game, 16, 1);

    REQUIRE(game.score() == 30);
}

TEST_CASE("When spare in a last frame player has an extra roll", "[score]")
{
    BowlingGame game;

    roll_many(game, 18, 1);
    roll_spare(game);
    game.roll(6);

    REQUIRE(game.score() == 34);
}

TEST_CASE("When strike in a last frame player has two extra rolls", "[score]")
{
    BowlingGame game;

    roll_many(game, 18, 1);
    roll_strike(game);
    game.roll(6);
    game.roll(7);

    REQUIRE(game.score() == 41);
}

TEST_CASE("Perfect game - score 300")
{
    BowlingGame game;

    for (int i = 0; i < 12; ++i)
        roll_strike(game);

    REQUIRE(game.score() == 300);
}

struct BowlingGameExample
{
    std::vector<unsigned int> pins;
    unsigned int expected_score;
};

std::ostream& operator<<(std::ostream& out, const BowlingGameExample& param)
{
    out << "{ [ ";

    for (const auto& pin : param.pins)
    {
        out << pin << " ";
    }

    out << "] - " << param.expected_score << " }";
    return out;
}

namespace std
{
    template <typename T>
    std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec)
    {
        out << "[ ";

        for (const auto& item : vec)
        {
            out << item << " ";
        }

        out << "]";
        return out;
    }
} // namespace std

TEST_CASE("Parametrized tests")
{
    auto params = GENERATE(
        BowlingGameExample{{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 20},
        BowlingGameExample{{10, 3, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 44},
        BowlingGameExample{{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 10, 5, 5}, 38},
        BowlingGameExample{{1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 10}, 119},
        BowlingGameExample{{10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}, 300});

    DYNAMIC_SECTION("When rolls are " << params.pins << " score is " << params.expected_score)
    {
        BowlingGame game;

        for (const auto& r : params.pins)
            game.roll(r);

        REQUIRE(game.score() == params.expected_score);
    }
}