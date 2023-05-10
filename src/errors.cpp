#include "./include/errors.hpp"

pcom::Errors::Errors(const char *msg) : err_msg(msg) {}

const char* pcom::Errors::what() {
    return err_msg;
}
