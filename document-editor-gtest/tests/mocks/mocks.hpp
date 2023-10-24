#ifndef MOCK_CONSOLE_HPP
#define MOCK_CONSOLE_HPP

#include "clipboard.hpp"
#include "console.hpp"

#include <gmock/gmock.h>

class MockClipboard : public Clipboard 
{
public:
    MOCK_METHOD(std::string, content, (), (const, override));
    MOCK_METHOD(void, set_content, (const std::string&), (override));
};

class MockConsole : public Console
{
public:
    MOCK_METHOD(std::string, get_line, (), (override));
    MOCK_METHOD(void, print, (const std::string&), (override));
};

class MockCommand : public Command
{
public:
    MOCK_METHOD(void, execute, (), (override));
};

#endif // MOCK_CONSOLE_HPP
