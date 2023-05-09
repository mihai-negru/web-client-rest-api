#include "./include/errors.hpp"

pcom::Errors::Errors(char *msg) : err_msg(msg) {}

char* pcom::Errors::what() {
    return err_msg;
}
