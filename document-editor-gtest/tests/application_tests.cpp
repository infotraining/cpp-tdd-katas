#include "command.hpp"
#include "document.hpp"
#include "mocks/mocks.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace std::literals;


namespace Take1
{
    namespace Commands
    {
        constexpr auto const EXIT = "exit";
    };

    class Application
    {
        Console& console_;

    public:
        Application(Console& console)
            : console_(console)
        { }

        int run()
        {
            console_.print("> Enter a command:");

            while (true)
            {
                std::string line = console_.get_line();

                if (line == Commands::EXIT)
                {
                    break;
                }
            }

            return 0;
        }
    };

    TEST(ApplicationTests, DisplaysPromptForCommand)
    {
        MockConsole console;
        Application app(console);

        EXPECT_CALL(console, print("> Enter a command:"));
        EXPECT_CALL(console, get_line()).WillOnce(Return(Commands::EXIT));

        app.run();
    }

    TEST(ApplicationTests, GetsLineFromInput)
    {
        MockConsole console;
        Application app(console);

        EXPECT_CALL(console, get_line()).WillOnce(Return(Commands::EXIT));

        app.run();
    }

    TEST(ApplicationTests, MainLoop_ExitEndsTheLoop)
    {
        MockConsole console;
        Application app(console);

        EXPECT_CALL(console, get_line()).WillOnce(Return(Commands::EXIT));
        EXPECT_CALL(console, get_line())
            .Times(3)
            .WillRepeatedly(Return("cmd"))
            .RetiresOnSaturation();

        int status = app.run();
        ASSERT_EQ(status, 0);
    }

} // namespace Take1

//////////////////////////////////////////////////////////

inline namespace Take2
{
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
            console_.print("> Enter a command:");

            while (true)
            {
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

    struct ApplicationTests : public Test
    {
        NiceMock<MockConsole> console;
        Application app{console};
        std::shared_ptr<MockCommand> cmd = std::make_shared<MockCommand>();
    };

    struct ApplicationTests_MainLoop : public ApplicationTests
    {
        void SetUp() override
        {
            app.add_command("cmd", cmd);
        }
    };

    TEST_F(ApplicationTests_MainLoop, DisplaysPromptForCommand)
    {
        EXPECT_CALL(console, get_line()).WillOnce(Return(Commands::EXIT));
        EXPECT_CALL(console, print("> Enter a command:"));

        app.run();
    }

    TEST_F(ApplicationTests_MainLoop, GetsLineFromInput)
    {
        EXPECT_CALL(console, get_line()).WillOnce(Return(Commands::EXIT));

        app.run();
    }

    TEST_F(ApplicationTests_MainLoop, ExitEndsTheLoop)
    {
        EXPECT_CALL(console, get_line()).WillOnce(Return(Commands::EXIT));
        EXPECT_CALL(console, get_line())
            .Times(3)
            .WillRepeatedly(Return("cmd"))
            .RetiresOnSaturation();

        int status = app.run();
        ASSERT_EQ(status, 0);
    }

    TEST_F(ApplicationTests_MainLoop, ExecutesCommands)
    {
        EXPECT_CALL(*cmd, execute());
        EXPECT_CALL(console, get_line())
            .WillOnce(Return("cmd"))
            .WillOnce(Return(Commands::EXIT));

        app.run();
    }

    TEST_F(ApplicationTests_MainLoop, UnknownCommandPrintsErrorMessage)
    {
        EXPECT_CALL(console, get_line())
            .WillOnce(Return("unknown"))
            .WillOnce(Return(Commands::EXIT));

        EXPECT_CALL(console, print("Unknown command: unknown"));
        EXPECT_CALL(console, print(_)).Times(1).RetiresOnSaturation();

        app.run();
    }

    TEST_F(ApplicationTests_MainLoop, CommandsAreCaseInsensitive)
    {
        EXPECT_CALL(*cmd, execute()).Times(3);
        EXPECT_CALL(console, get_line())
            .WillOnce(Return("CMD"))
            .WillOnce(Return("cMd"))
            .WillOnce(Return("CMD"))
            .WillOnce(Return(Commands::EXIT));

        app.run();
    }
} // namespace Take2

//////////////////////////////////////////////////////////

TEST(PrintCmd_Execute, PrintsDocumentInConsole)
{
    NiceMock<MockConsole> console;
    Document doc{"abc"};

    EXPECT_CALL(console, print("[abc]"));

    PrintCmd cmd{doc, console};
    cmd.execute();
}

//////////////////////////////////////////////////////////

TEST(AddText_Execute, GetsTextFromConsoleAndAddsToDocument)
{
    NiceMock<MockConsole> console;
    Document doc{"abc"};

    EXPECT_CALL(console, get_line()).WillOnce(Return("def"));

    AddText cmd{doc, console};
    cmd.execute();

    ASSERT_EQ(doc.text(), "abcdef");
}