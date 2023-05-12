#pragma once

#include "./utils.hpp"
#include "./errors.hpp"
#include "./tcpsocket.hpp"
#include "./httphandler.hpp"
#include "./inputcommand.hpp"

class pcom::Client {
    private:
        pcom::TcpSocket tcpsock;
        pcom::HttpHandler httphandler;
        pcom::InputCommand cmdhandler;
        std::unordered_map<pcom::InputCommand::Command, std::string> urls;
        bool is_logged_in;
        bool has_library_access;
        std::string session_cookie;
        std::string library_token;

        void handle_register_request();
        void handle_login_request();
        void handle_enter_library_request();
        void handle_get_books_request();
        void handle_get_book_request();
        void handle_add_book_request();
        void handle_delete_book_request();
        void handle_logout_request();
    
    public:
        Client(
            const char *host_ip,
            uint16_t host_port,
            std::unordered_map<pcom::InputCommand::Command, std::string>& urls
        );

        void start_session();

        ~Client();
};
