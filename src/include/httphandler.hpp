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

    public:
        HttpHandler();

        void add_bytes(const char* bytes, const size_t bytes_len);
        
        bool is_header_complete();
        
        size_t get_content_length();
        size_t get_body_size();
        
        char* byte_stream();
        
        void clear();
};
