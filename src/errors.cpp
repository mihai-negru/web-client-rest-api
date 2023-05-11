#include "./include/errors.hpp"

pcom::Errors::Errors(const char *msg) {
    err_msg = "\033[0;31m[ERROR]: ";
    err_msg += msg;
    err_msg += "\033[0m\n"; 
}

const std::string pcom::Errors::what() {
    return err_msg;
}
