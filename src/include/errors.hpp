#pragma once

#include "./utils.hpp"

class pcom::Errors : public std::exception {
    private:
    std::string err_msg;

    public:
    Errors(const char *msg);

    const std::string what();
};
