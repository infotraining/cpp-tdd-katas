#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "command.hpp"
#include "console.hpp"

#include <map>

std::string to_lower(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](auto c) { return std::tolower(c); });
    return text;
}

class Application
{
    Console& console_;
    std::map<std::string, std::shared_ptr<Command>> commands_;

public:
    Application(Console& console)
        : console_(console)
    { }

    int run()
    {
        using namespace std::literals;

        while (true)
        {
            console_.print("> Enter a command:");

            std::string line = to_lower(console_.get_line());

            if (line == Commands::EXIT)
                return 0;

            if (auto cmd_entry = commands_.find(line); cmd_entry != commands_.end())
            {
                const auto& [name, command] = *cmd_entry;
                command->execute();
            }
            else
            {
                console_.print("Unknown command: "s + line);
            }
        }
    }

    void add_command(std::string const& name, std::shared_ptr<Command> cmd)
    {
        commands_.emplace(name, cmd);
    }
};

#endif // APPLICATION_HPP
