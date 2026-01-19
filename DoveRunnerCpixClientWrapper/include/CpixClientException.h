#pragma once

#include <exception>
#include <string>

namespace doverunner {
    class CpixClientException : public std::exception
    {
    private:
        std::string message;

    public:
        CpixClientException(const std::string& msg);
        ~CpixClientException() throw();
        const char* what();
    };
}

