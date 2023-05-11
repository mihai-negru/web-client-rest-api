#pragma once

#include <sstream>
#include "./nlohmann/json.hpp"
#include "./utils.hpp"
#include "./errors.hpp"

using json = nlohmann::json;

class pcom::InputCommand {
    public:
        enum Command {
            REGISTER        = 0,
            LOGIN           = 1,
            ENTER_LIBRARY   = 2,
            GET_BOOKS       = 3,
            GET_BOOK        = 4,
            ADD_BOOK        = 5,
            DELETE_BOOK     = 6,
            LOGOUT          = 7,
            EXIT            = 8,
            UNKNOWN         = 9
        };

        InputCommand();

        void listen_for_command();

        std::string get_command_body();
        json get_command_raw();
        Command get_command_type();

    private:
        json body;
        Command type;

        bool is_number(const std::string& str);
        bool is_word(const std::string& str);

        void parse_command(std::string command);

        void handle_register_cmd();
        void handle_login_cmd();
        void handle_enter_library_cmd();
        void handle_get_books_cmd();
        void handle_get_book_cmd();
        void handle_add_book_cmd();
        void handle_delete_book_cmd();
        void handle_logout_cmd();
        void handle_exit_cmd();
};
