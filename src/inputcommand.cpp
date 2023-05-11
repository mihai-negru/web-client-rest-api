#include "./include/inputcommand.hpp"

#define GREEN_COLOR "\033[0;32m"
#define RESET_COLOR "\033[0m"

pcom::InputCommand::InputCommand() {
    body.clear();
    type = UNKNOWN;
}

void pcom::InputCommand::listen_for_command() {
    std::string input;
    std::getline(std::cin, input);

    if (!is_word(input)) {
        throw pcom::Errors("Invalid format for a command.");
    }

    body.clear();
    parse_command(input);
}

std::string pcom::InputCommand::get_command_body() {
    if (body.empty()) {
        throw pcom::Errors("Could not dump an empty json.");
    }

    return body.dump();
}

json pcom::InputCommand::get_command_raw() {
    if (body.empty()) {
        throw pcom::Errors("Could not return an empty json.");
    }

    return body;
}

pcom::InputCommand::Command pcom::InputCommand::get_command_type() {
    return type;
}

bool pcom::InputCommand::is_number(const std::string& str) {
    return !str.empty() && str.find_first_not_of("0123456789") == std::string::npos;
}

bool pcom::InputCommand::is_word(const std::string& str) {
    return !str.empty() && str.find_first_of("0123456789\n ") == std::string::npos;
}

void pcom::InputCommand::parse_command(std::string command) {
    if (command.empty()) {
        throw pcom::Errors("Could not parse empty command.");
    }

    if (command.compare("register") == 0) {
        handle_register_cmd();
    } else if (command.compare("login") == 0) {
        handle_login_cmd();
    } else if (command.compare("enter_library") == 0) {
        handle_enter_library_cmd();
    } else if (command.compare("get_books") == 0) {
        handle_get_books_cmd();
    } else if (command.compare("get_book") == 0) {
        handle_get_book_cmd();
    } else if (command.compare("add_book") == 0) {
        handle_add_book_cmd();
    } else if (command.compare("delete_book") == 0) {
        handle_delete_book_cmd();
    } else if (command.compare("logout") == 0) {
        handle_logout_cmd();
    } else if (command.compare("exit") == 0) {
        handle_exit_cmd();
    } else {
        type = UNKNOWN;

        throw pcom::Errors("Unknown input command.");
    }
}

void pcom::InputCommand::handle_register_cmd() {
    std::string input;

    std::cout << GREEN_COLOR << "username=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_word(input)) {
        throw pcom::Errors("Register with numeric or empty username field.");
    }

    body["username"] = input;

    std::cout << GREEN_COLOR << "password=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_word(input)) {
        throw pcom::Errors("Register with numeric or empty password field.");
    }

    body["password"] = input;

    type = REGISTER;

    std::cout << '\n';
}

void pcom::InputCommand::handle_login_cmd() {
    std::string input;

    std::cout << GREEN_COLOR << "username=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_word(input)) {
        throw pcom::Errors("Login with numeric or empty username field.");
    }

    body["username"] = input;

    std::cout << GREEN_COLOR << "password=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_word(input)) {
        throw pcom::Errors("Login with numeric or empty password field.");
    }

    body["password"] = input;

    type = LOGIN;

    std::cout << '\n';
}

void pcom::InputCommand::handle_enter_library_cmd() {
    type = ENTER_LIBRARY;

    std::cout << '\n';
}

void pcom::InputCommand::handle_get_books_cmd() {
    type = GET_BOOKS;

    std::cout << '\n';
}

void pcom::InputCommand::handle_get_book_cmd() {
    std::string input;

    std::cout << GREEN_COLOR << "id=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_number(input)) {
        throw pcom::Errors("Get book id is not a number.");
    }

    std::stringstream stream(input);
    int id = 0;

    stream >> id;

    body["id"] = id;

    type = GET_BOOK;

    std::cout << '\n';
}

void pcom::InputCommand::handle_add_book_cmd() {
    std::string input;

    std::cout << GREEN_COLOR << "title=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_word(input)) {
        throw pcom::Errors("Add book with numeric or empty title.");
    }

    body["title"] = input;

    std::cout << GREEN_COLOR << "author=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_word(input)) {
        throw pcom::Errors("Add book with numeric or empty author.");
    }

    body["author"] = input;

    std::cout << GREEN_COLOR << "genre=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_word(input)) {
        throw pcom::Errors("Add book with numeric or empty genre.");
    }

    body["genre"] = input;

    std::cout << GREEN_COLOR << "publisher=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_word(input)) {
        throw pcom::Errors("Add book with numeric or empty publisher.");
    }

    body["publisher"] = input;

    std::cout << GREEN_COLOR << "page_count=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_number(input)) {
        throw pcom::Errors("Add book page count is not a number.");
    }

    std::stringstream stream(input);
    int page_count = 0;

    stream >> page_count;

    body["page_count"] = page_count;

    type = ADD_BOOK;

    std::cout << '\n';
}

void pcom::InputCommand::handle_delete_book_cmd() {
    std::string input;

    std::cout << GREEN_COLOR << "id=" << RESET_COLOR;
    std::getline(std::cin, input);

    if (!is_number(input)) {
        throw pcom::Errors("Delete book id is not a number.");
    }

    std::stringstream stream(input);
    int id = 0;

    stream >> id;

    body["id"] = id;

    type = DELETE_BOOK;

    std::cout << '\n';
}

void pcom::InputCommand::handle_logout_cmd() {
    type = LOGOUT;

    std::cout << '\n';
}

void pcom::InputCommand::handle_exit_cmd() {
    type = EXIT;

    std::cout << '\n';
}
