#include "./include/httphandler.hpp"

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE sizeof(HEADER_TERMINATOR) - 1
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE sizeof(CONTENT_LENGTH) - 1

pcom::HttpHandler::HttpHandler() {
    clear();
}

void pcom::HttpHandler::add_bytes(const char* bytes, const size_t bytes_len) {
    if (header_complete == true) {
        size_t append_bytes = content_length - body.size();

        if (bytes_len <= append_bytes) {
            body.append(bytes, bytes_len);
        } else {
            body_complete = true;
            body.append(bytes, append_bytes);
        }
    } else {
        std::string dummy(bytes);

        std::size_t header_end = dummy.find(HEADER_TERMINATOR);

        if (header_end != std::string::npos) {
            header_end += HEADER_TERMINATOR_SIZE;
            header.append(dummy.substr(0, header_end));

            std::size_t content_length_start = dummy.find(CONTENT_LENGTH);

            if (content_length_start != std::string::npos) {
                content_length_start += CONTENT_LENGTH_SIZE;

                std::stringstream stream(dummy.substr(content_length_start));
                stream >> content_length;

                header_complete = true;
                add_bytes(bytes + header_end, bytes_len - header_end + 1);
            } else {
                throw pcom::Errors("Could not find the content length in the header.");
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
        dummy.append(body);
    }

    char *dummy_c_str = new char[dummy.size() + 1];
    strcpy(dummy_c_str, dummy.c_str());

    return dummy_c_str;
}

void pcom::HttpHandler::clear() {
    header.clear();
    body.clear();
    header_complete = false;
    body_complete = false;
    content_length = 0;
}
