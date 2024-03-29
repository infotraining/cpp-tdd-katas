#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "clipboard.hpp"
#include "console.hpp"
#include "document.hpp"
#include <memory>
#include <stack>

class Command
{
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

namespace Commands
{
    constexpr auto const EXIT = "exit";
};


class PrintCmd : public Command
{
    Document& doc_;
    Console& console_;
public:
    constexpr static auto const ID = "print";

    PrintCmd(Document& doc, Console& console) : doc_(doc), console_(console) {}

    void execute() override
    {
        console_.print("[" + doc_.text() + "]");
    }
};

class AddText : public Command
{
    Document& doc_;
    Console& console_;
public:
    constexpr static auto const ID = "addtext";

    AddText(Document& doc, Console& console) : doc_(doc), console_(console) {}

    void execute() override
    {
        console_.print("> Enter text:");
        std::string text = console_.get_line();
        doc_.add_text(text);
    }
};

#endif // COMMAND_HPP
