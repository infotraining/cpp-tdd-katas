#include "catch.hpp"
#include "bowling.hpp"
#include <numeric>

using namespace std;

void roll_many(Game& game, size_t count, size_t pins)
{
    for (size_t i = 0; i < count; ++i)
    {
        game.roll(pins);
    }
}

void roll_spare(Game& game)
{
    game.roll(1);
    game.roll(9);
}

void roll_strike(Game& game)
{
    game.roll(10);
}

TEST_CASE("when game starts score is zero", "[score]")
{
    Game game;

    REQUIRE(game.score() == 0);
}

TEST_CASE("when all rolls in gutter score is zero", "[score]")
{
    Game game;
    roll_many(game, 20, 0);

    REQUIRE(game.score() == 0);
}

TEST_CASE("when all rolls no mark score is sum of pins", "[score]")
{
    Game game;
    roll_many(game, 20, 2);

    REQUIRE(game.score() == 40);
}

TEST_CASE("when spare next throw is counted twice", "[score]")
{
    Game game;

    roll_spare(game);
    roll_many(game, 18, 1);

    REQUIRE(game.score() == 29);
}

TEST_CASE("when strike next two throws are counted twice", "[score]")
{
    Game game;

    roll_many(game, 2, 1);
    roll_strike(game);
    roll_many(game, 16, 1);

    REQUIRE(game.score() == 30);
}

TEST_CASE("when spare in a last frame player has an extra roll", "[score]")
{
    Game game;

    roll_many(game, 18, 1);
    roll_spare(game);
    game.roll(6);

    REQUIRE(game.score() == 34);
}

TEST_CASE("when strike in a last frame player has two extra rolls", "[score]")
{
    Game game;

    roll_many(game, 18, 1);
    roll_strike(game);
    game.roll(6);
    game.roll(7);

    REQUIRE(game.score() == 41);
}

TEST_CASE("perfect game - score 300")
{
    Game game;

    for(int i = 0; i < 12; ++i)
        roll_strike(game);

    REQUIRE(game.score() == 300);
}