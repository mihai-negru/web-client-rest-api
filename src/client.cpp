#include "./include/client.hpp"

#define GREEN_COLOR "\033[0;32m"
#define RESET_COLOR "\033[0m"

#define loop for(;;)

pcom::Client::Client(
    const char *host_ip,
    uint16_t host_port,
    std::unordered_map<pcom::InputCommand::Command, std::string>& urls
):  
tcpsock(host_ip, host_port),
httphandler(),
cmdhandler(),
urls(urls) {

    httphandler.set_host_ip(host_ip);
    is_logged_in = false;
    has_library_access = false;
}

pcom::Client::~Client() {}

void pcom::Client::handle_register_request() {
    httphandler.clear();

    httphandler
        .set_host_url(urls[pcom::InputCommand::Command::REGISTER])
        .set_content_type("application/json")
        .set_body(cmdhandler.get_command_body())
        .generate_post_request();

    tcpsock.open_connection();
    tcpsock.send_stream(httphandler);
    tcpsock.recv_stream(httphandler);
    
    if (httphandler.get_status_code() != 201) {
        throw pcom::Errors("The username is already registered.");
    }

    std::cout << GREEN_COLOR << "[OK] Client registered.\n\n" << RESET_COLOR;
}

void pcom::Client::handle_login_request() {
    if (is_logged_in) {
        throw pcom::Errors("Client is already logged in.");
    }

    httphandler.clear();
    
    httphandler
        .set_host_url(urls[pcom::InputCommand::Command::LOGIN])
        .set_content_type("application/json")
        .set_body(cmdhandler.get_command_body())
        .generate_post_request();

    tcpsock.open_connection();
    tcpsock.send_stream(httphandler);
    tcpsock.recv_stream(httphandler);

    if (httphandler.get_status_code() != 200) {
        throw pcom::Errors("Wrong user credentials.");
    }

    session_cookie = httphandler.extract_cookies();
    is_logged_in = true;

    std::cout << GREEN_COLOR << "[OK] Client logged in.\n\n" << RESET_COLOR;
}

void pcom::Client::handle_enter_library_request() {
    if (!is_logged_in) {
        throw pcom::Errors("Client is not logged in in order to enter library.");
    }

    httphandler.clear();

    httphandler
        .set_host_url(urls[pcom::InputCommand::Command::ENTER_LIBRARY])
        .set_content_type("application/json")
        .set_cookies(session_cookie)
        .generate_get_request();

    tcpsock.open_connection();
    tcpsock.send_stream(httphandler);
    tcpsock.recv_stream(httphandler);

    if (httphandler.get_status_code() != 200) {
        throw pcom::Errors("Session cookie is depricated on entering the library.");
    }

    library_token = json::parse(httphandler.get_body())["token"];
    has_library_access = true;

    std::cout << GREEN_COLOR << "[OK] Client got library access.\n\n" << RESET_COLOR;
}

void pcom::Client::handle_get_books_request() {
    if (!has_library_access) {
        throw pcom::Errors("Client does not have access to the library.");
    }

    httphandler.clear();

    httphandler
        .set_host_url(urls[pcom::InputCommand::Command::GET_BOOKS])
        .set_content_type("application/json")
        .set_authorization(library_token)
        .generate_get_request();

    tcpsock.open_connection();
    tcpsock.send_stream(httphandler);
    tcpsock.recv_stream(httphandler);

    if (httphandler.get_status_code() != 200) {
        throw pcom::Errors("Library token is depricated on getting books.");
    }

    json books = json::parse(httphandler.get_body());

    size_t books_count = books.size();

    if (books_count == 0) {
        throw pcom::Errors("No books found in the library");
    }

    size_t books_to_print = 0;

    bool is_invalid_number = true;
    std::string dummy;

    while (is_invalid_number) {
        std::cout << GREEN_COLOR << "How many books to print out of <" << books_count << ">? --> " << RESET_COLOR;
        std::getline(std::cin, dummy);

        if ((!dummy.empty()) &&(dummy.find_first_not_of("0123456789") == std::string::npos)) {
            std::stringstream stream(dummy);

            stream >> books_to_print;

            if (books_to_print <= books_count) {
                is_invalid_number = false;
            }
        }
    }

    if (books_to_print == 0) {
        std::cout << GREEN_COLOR << "\n[OK] No book printed.\n\n" << RESET_COLOR;
    } else {
        std::cout << '\n';
        for (size_t i = 0; i < books_to_print; ++i) {
            auto& book = books[i];
            std::cout << "Book No. " << (i + 1) << '\n';
            std::cout << "  Title: " << book["title"] << '\n';
            std::cout << "  Id: " << book["id"] << "\n\n";
        }
    }
}

void pcom::Client::handle_get_book_request() {
    if (!has_library_access) {
        throw pcom::Errors("Client does not have access to the library.");
    }

    std::string book_id = std::to_string((int)cmdhandler.get_command_raw()["id"]);

    httphandler.clear();

    httphandler
        .set_host_url(urls[pcom::InputCommand::Command::GET_BOOK] + book_id)
        .set_content_type("application/json")
        .set_authorization(library_token)
        .generate_get_request();

    tcpsock.open_connection();
    tcpsock.send_stream(httphandler);
    tcpsock.recv_stream(httphandler);

    if (httphandler.get_status_code() == 404) {
        throw pcom::Errors("No book matches the id from the library.");
    } else if (httphandler.get_status_code() != 200) {
        throw pcom::Errors("Library token is depricated on getting book.");
    }

    json book = json::parse(httphandler.get_body());

    std::cout << "Book info:\n";
    std::cout << "  Title: " << book["title"] << '\n';
    std::cout << "  Author: " << book["author"] << '\n';
    std::cout << "  Publisher: " << book["publisher"] << '\n';
    std::cout << "  Genre: " << book["genre"] << '\n';
    std::cout << "  Page Count: " << book["page_count"] << "\n\n";
}

void pcom::Client::handle_add_book_request() {
    if (!has_library_access) {
        throw pcom::Errors("Client does not have access to the library.");
    }

    httphandler.clear();

    httphandler
        .set_host_url(urls[pcom::InputCommand::Command::ADD_BOOK])
        .set_content_type("application/json")
        .set_authorization(library_token)
        .set_body(cmdhandler.get_command_body())
        .generate_post_request();

    tcpsock.open_connection();
    tcpsock.send_stream(httphandler);
    tcpsock.recv_stream(httphandler);

    if (httphandler.get_status_code() == 400) {
        throw pcom::Errors("Book info is not complete or do not follow the formatting.");
    } else if (httphandler.get_status_code() != 200) {
        throw pcom::Errors("Library token is depricated on adding book.");
    }

    std::cout << GREEN_COLOR << "[OK] Book was added in the library.\n\n" << RESET_COLOR;
}

void pcom::Client::handle_delete_book_request() {
    if (!has_library_access) {
        throw pcom::Errors("Client does not have access to the library.");
    }

    std::string book_id = std::to_string((int)cmdhandler.get_command_raw()["id"]);

    httphandler.clear();

    httphandler
        .set_host_url(urls[pcom::InputCommand::Command::DELETE_BOOK] + book_id)
        .set_content_type("application/json")
        .set_authorization(library_token)
        .generate_delete_request();

    tcpsock.open_connection();
    tcpsock.send_stream(httphandler);
    tcpsock.recv_stream(httphandler);

    if (httphandler.get_status_code() == 404) {
        throw pcom::Errors("No book matches the id from the library.");
    } else if (httphandler.get_status_code() != 200) {
        throw pcom::Errors("Library token is depricated on getting book.");
    }

    std::cout << GREEN_COLOR << "[OK] Book was deleted from the library.\n\n" << RESET_COLOR;
}

void pcom::Client::handle_logout_request() {
    if (!is_logged_in) {
        throw pcom::Errors("Client is not logged in in order to logout.");
    }

    httphandler.clear();

    httphandler
        .set_host_url(urls[pcom::InputCommand::Command::LOGOUT])
        .set_content_type("application/json")
        .set_cookies(session_cookie)
        .generate_get_request();

    tcpsock.open_connection();
    tcpsock.send_stream(httphandler);
    tcpsock.recv_stream(httphandler);

    if (httphandler.get_status_code() != 200) {
        throw pcom::Errors("Session cookie is depricated on log out.");
    }

    session_cookie.clear();
    library_token.clear();
    is_logged_in = false;
    has_library_access = false;

    std::cout << GREEN_COLOR << "[OK] Client logged out.\n\n" << RESET_COLOR;
}

void pcom::Client::start_session() {
    loop {
        try {
            cmdhandler.listen_for_command();

            pcom::InputCommand::Command cmd_type = cmdhandler.get_command_type();

            if (cmd_type == pcom::InputCommand::Command::EXIT) {
                break;
            }

            switch (cmd_type) {
                case pcom::InputCommand::Command::REGISTER:
                    handle_register_request();
                    break;
                case pcom::InputCommand::Command::LOGIN:
                    handle_login_request();
                    break;
                case pcom::InputCommand::Command::ENTER_LIBRARY:
                    handle_enter_library_request();
                    break;
                case pcom::InputCommand::Command::GET_BOOKS:
                    handle_get_books_request();
                    break;
                case pcom::InputCommand::Command::GET_BOOK:
                    handle_get_book_request();
                    break;
                case pcom::InputCommand::Command::ADD_BOOK:
                    handle_add_book_request();
                    break;
                case pcom::InputCommand::Command::DELETE_BOOK:
                    handle_delete_book_request();
                    break;
                case pcom::InputCommand::Command::LOGOUT:
                    handle_logout_request();
                    break;
                default:
                    throw pcom::Errors("Parsing commands failed with unknown reasons.");
            }
        } catch (pcom::Errors& err) {
            std::cout << err.what() << '\n';
        }
    }
}