#ifndef CLIPBOARD_HPP
#define CLIPBOARD_HPP

#include <mutex>
#include <string>

class Clipboard
{
public:
    virtual std::string content() const = 0;
    virtual void set_content(const std::string& content) = 0;
    virtual ~Clipboard() = default;
};


class SharedClipboard : public Clipboard
{
    std::string content_;
    mutable std::mutex content_mtx_;

public:
    SharedClipboard& instance()
    {
        static SharedClipboard unique_instance;

        return unique_instance;
    }

    std::string content() const override
    {
        std::lock_guard<std::mutex> lk{content_mtx_};

        return content_;
    }

    void set_content(const std::string& content) override
    {
        std::lock_guard<std::mutex> lk{content_mtx_};

        content_ = content;
    }
};

#endif // CLIPBOARD_HPP
