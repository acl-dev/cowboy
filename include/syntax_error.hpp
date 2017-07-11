#pragma once
#include <stdexcept>

struct syntax_error :std::logic_error
{
    syntax_error(const std::string &error = "syntax_error")
        :std::logic_error(error)
    {

    }
};