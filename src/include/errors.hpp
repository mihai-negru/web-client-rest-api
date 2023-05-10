#pragma once

#include "./utils.hpp"

class pcom::Errors : public std::exception {
    private:
    const char *err_msg;

    public:
    Errors(const char *msg);

    const char *what();
};
