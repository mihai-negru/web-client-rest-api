#pragma once

#include <sstream>
#include <string.h>
#include "./utils.hpp"
#include "./errors.hpp"

class pcom::HttpHandler {
    private:
        std::string header;
        std::string body;
        bool header_complete;
        bool body_complete;
        size_t content_length;
        std::string host_ip;
        std::string host_url;
        std::string queries;
        std::string content_type;
        std::string cookies;
        std::string token;
        int status_code;

    public:
        HttpHandler();

        HttpHandler& set_host_ip(std::string host_ip);
        HttpHandler& set_host_url(std::string host_url);
        HttpHandler& set_queries(std::string queries);
        HttpHandler& set_cookies(std::string cookies);
        HttpHandler& set_authorization(std::string token);
        HttpHandler& set_content_type(std::string content_type);
        HttpHandler& set_body(std::string body);

        HttpHandler& generate_get_request();
        HttpHandler& generate_post_request();

        int get_status_code();
        std::string extract_cookies();

        void add_bytes(const char* bytes, const size_t bytes_len);
        
        bool is_header_complete();
        
        size_t get_content_length();
        size_t get_body_size();
        
        char* byte_stream();
        std::string get_body();
        
        void clear();
};
