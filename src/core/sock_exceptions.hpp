#ifndef SOCK_EXCEPTIONS_HPP
#define SOCK_EXCEPTIONS_HPP

#include <iostream>
#include <stdio.h>

#define TYPED_EXCEPTION(message, data) TypedException<decltype(data)>(message, data, __FILE__, __LINE__)

enum class SockExceptionCode
{
    UNKNOWN = -1,
    LIBRARY_NOT_FOUND,
    CREATION_FAILED,
    SEND_FAILED,
    RECEIVE_FAILED,
    BIND_FAILED,
    LISTEN_FAILED,
};

template <typename T>
class TypedException
{

private:
    std::string message;
    T data;
    const std::string file;
    const int line;

public:
    TypedException(std::string message, T data, const std::string file, const int line)
        : message{std::move(message)},
          data{std::move(data)},
          file{std::move(file)},
          line{line} {}

    std::string &getMessage()
    {
        return this->message;
    }

    const std::string &getMessage() const noexcept
    {
        return this->message;
    }

    T &getData()
    {
        return this->data;
    }

    const T &getData() const noexcept
    {
        return this->data;
    }

    const std::string getFileName() const noexcept
    {
        return this->file;
    }

    const int getLineNumber() const noexcept
    {
        return this->line;
    }

    friend std::ostream &operator<<(std::ostream &os, const TypedException &e)
    {
        os << "Failed with code (" << static_cast<int>(e.getData()) << "):" << e.getMessage() << std::endl
           << "Location: " << e.getFileName() << ":" << e.getLineNumber() << std::endl;

        return os;
    }
};

#endif