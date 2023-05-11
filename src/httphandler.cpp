#include "./include/httphandler.hpp"

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE sizeof(HEADER_TERMINATOR) - 1
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE sizeof(CONTENT_LENGTH) - 1

pcom::HttpHandler::HttpHandler() {
    clear();
}

pcom::HttpHandler& pcom::HttpHandler::set_host_ip(std::string host_ip) {
    this->host_ip = host_ip;

    return *this;
}

pcom::HttpHandler& pcom::HttpHandler::set_host_url(std::string host_url) {
    this->host_url = host_url;

    return *this;
}

pcom::HttpHandler& pcom::HttpHandler::set_queries(std::string queries) {
    this->queries = queries;

    return *this;
}

pcom::HttpHandler& pcom::HttpHandler::set_cookies(std::string cookies) {
    this->cookies = cookies;

    return *this;
}

pcom::HttpHandler& pcom::HttpHandler::set_authorization(std::string token) {
    this->token = token;

    return *this;
}

pcom::HttpHandler& pcom::HttpHandler::set_content_type(std::string content_type) {
    this->content_type = content_type;

    return *this;
}

pcom::HttpHandler& pcom::HttpHandler::set_body(std::string body) {
    this->body = body;
    body_complete = true;

    return *this;
}

pcom::HttpHandler& pcom::HttpHandler::generate_get_request() {
    if ((host_url.empty()) || (host_ip.empty())) {
        throw pcom::Errors("Could not generate GET request because of null fields.");
    }

    if (!queries.empty()) {
        header = "GET " + host_url + "?" + queries + " HTTP/1.1\r\n";
    } else {
        header = "GET " + host_url + " HTTP/1.1\r\n";
    }

    header += "Host: " + host_ip + "\r\n";

    if (!token.empty()) {
        header += "Authorization: Bearer " + token + "\r\n";
    }

    if (!cookies.empty()) {
        header += "Cookie: " + cookies + "\r\n";
    }

    header += "\r\n";

    header_complete = true;

    return *this;
}

pcom::HttpHandler& pcom::HttpHandler::generate_post_request() {
    if ((host_url.empty()) || (host_ip.empty()) || (content_type.empty())) {
        throw pcom::Errors("Could not generate POST request because of null fields.");
    }

    header = "POST " + host_url + " HTTP/1.1\r\n";
    header += "Host: " + host_ip + "\r\n" + "Content-Type: " + content_type + "\r\n";
    
    if (!token.empty()) {
        header += "Authorization: Bearer " + token + "\r\n";
    }

    if (!cookies.empty()) {
        header += "Cookie: " + cookies + "\r\n";
    }

    content_length = body.size();
    header += "Content-Length: " + std::to_string(content_length) + "\r\n\r\n";

    header_complete = true;

    return *this;
}

int pcom::HttpHandler::get_status_code() {
    if (!header_complete) {
        throw pcom::Errors("Header is not complete for reading status.");
    }

    std::stringstream stream(header.substr(9, 3));

    int status_code = 0;
    stream >> status_code;

    return status_code;
}

std::string pcom::HttpHandler::extract_cookies() {
    if (!header_complete) {
        throw pcom::Errors("Header is not complete for reading cookies.");
    }

    std::size_t cookies_start = header.find("Set-Cookie: ");

    if (cookies_start == std::string::npos) {
        throw pcom::Errors("Could not find the cookies in the header.");
    }

    cookies_start += 12;

    std::size_t cookies_end = header.rfind(';');

    if (cookies_end == std::string::npos) {
        throw pcom::Errors("Cookies format is wrong, cannot read cookies.");
    }

    return header.substr(cookies_start, cookies_end - cookies_start + 1);
}

void pcom::HttpHandler::add_bytes(const char* bytes, const size_t bytes_len) {
    if (header_complete == true) {
        size_t append_bytes = content_length > body.size() ? content_length - body.size() : 0;

        if (bytes_len < append_bytes) {
            body.append(bytes, bytes_len);
        } else {
            body_complete = true;
            body.append(bytes, append_bytes);
        }
    } else {
        std::string dummy(bytes, bytes_len);

        std::size_t header_end = dummy.find(HEADER_TERMINATOR);

        if (header_end != std::string::npos) {
            header_complete = true;
            header_end += HEADER_TERMINATOR_SIZE;
            header.append(dummy.substr(0, header_end));

            std::size_t content_length_start = dummy.find(CONTENT_LENGTH);

            if (content_length_start != std::string::npos) {
                content_length_start += CONTENT_LENGTH_SIZE;

                std::stringstream stream(dummy.substr(content_length_start));
                stream >> content_length;

                add_bytes(bytes + header_end, bytes_len - header_end);
            } else {
                content_length = 0;
            }
        } else {
            header.append(bytes, bytes_len);
        }
    }
}
        
bool pcom::HttpHandler::is_header_complete() {
    return header_complete;
}

size_t pcom::HttpHandler::get_content_length() {
    return content_length;
}

size_t pcom::HttpHandler::get_body_size() {
    return body.size();
}

char* pcom::HttpHandler::byte_stream() {
    std::string dummy(header);

    if (body_complete == true) {
        dummy += body;
    }

    char *dummy_c_str = new char[dummy.size() + 1];
    strcpy(dummy_c_str, dummy.c_str());

    return dummy_c_str;
}

std::string pcom::HttpHandler::get_body() {
    if(!body_complete) {
        throw pcom::Errors("Body is not completed for reading.");
    }

    return body;
}

void pcom::HttpHandler::clear() {
    header.clear();
    body.clear();
    header_complete = false;
    body_complete = false;
    content_length = 0;
    host_url.clear();
    queries.clear();
    content_type.clear();
    cookies.clear();
    token.clear();
}
