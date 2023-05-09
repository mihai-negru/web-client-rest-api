#pragma once

#include "./utils.hpp"

class pcom::Errors : public std::exception {
    private:
    char *err_msg;

    public:
    Errors(char *msg);

    char *what();
};
