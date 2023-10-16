#ifndef ROVER_HPP
#define ROVER_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>
#include <limits>

namespace TDD
{
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

        UnknownCommand(const std::string& unknown_command, const std::string& last_commands)
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

    struct Grid
    {
        size_t max_x;
        size_t max_y;

        Grid(size_t max_x = std::numeric_limits<size_t>::max(), 
             size_t max_y = std::numeric_limits<size_t>::max())
            : max_x{max_x}
            , max_y{max_y}
        {
        }

        Position wrap(Position position) const
        {
            auto [x, y] = position.coordinates();

            int wrapped_x = x >= 0 ? x % max_x : max_x - (-x % max_x);
            int wrapped_y = y >= 0 ? y % max_y : max_y - (-y % max_y);

            return Position{wrapped_x, wrapped_y, position.orientation()};
        }
    };

    class Rover
    {
        Position position_;
        Grid grid_;

    public:
        Rover(int x, int y, char orientation, Grid grid = {})
            : position_{x, y, orientation}, grid_{grid}
        {
        }

        Rover(Position position, Grid grid = {})
            : position_{position}, grid_{grid}
        {
        }

        Position position() const
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

            position_ = grid_.wrap(position_);

            return position();
        }
    };
} // namespace TDD

#endif