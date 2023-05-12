#include "./include/tcpsocket.hpp"
#include <fcntl.h>

#define MAX_BUFLEN 4096

pcom::TcpSocket::TcpSocket(const char *host_ip, uint16_t host_port) {
    if (host_ip == nullptr) {
        throw pcom::Errors("Wrong socket input values.");
    }

    if (host_port < 1024) {
        throw pcom::Errors("Reserved port number.");
    }

    fd = -1;

    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(host_port);
    inet_aton(host_ip, &addr.sin_addr);
}

pcom::TcpSocket::~TcpSocket() {
    if (fd > 0) {
        close(fd);
        fd = -1;
    }
}

void pcom::TcpSocket::open_connection() {
    if (fd >= 0) {
        close(fd);
        fd = -1;
    }

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) {
        throw pcom::Errors("Could not open a socket.");
    }

    if (connect(fd, (struct sockaddr *) &addr, sizeof addr) < 0) {
        throw pcom::Errors("Could not connect to the server.");
    }
}

void pcom::TcpSocket::send_stream(HttpHandler& handler) {
    if (fd < 0) {
        throw pcom::Errors("TCP socket is invalid for sending.");
    }

    const char *byte_stream = handler.byte_stream();
    ssize_t bytes_send = 0, tcp_bytes = 0;
    ssize_t stream_len = strlen(byte_stream);

    while (bytes_send < stream_len) {
        tcp_bytes = write(fd, byte_stream + bytes_send, stream_len - bytes_send);

        if (tcp_bytes <= 0) {
            delete[] byte_stream;

            throw pcom::Errors("Failed to send byte stream over the network.");
        }

        bytes_send += tcp_bytes;
    }

    delete[] byte_stream;
}

void pcom::TcpSocket::recv_stream(HttpHandler& handler) {
    if (fd < 0) {
        throw pcom::Errors("TCP socket is invalid for receiving.");
    }

    handler.clear();

    char byte_stream[MAX_BUFLEN];
    ssize_t tcp_bytes = 0;

    while (!handler.is_header_complete()) {
        tcp_bytes = read(fd, byte_stream, MAX_BUFLEN);

        if (tcp_bytes < 0) {
            throw pcom::Errors("Failed to recv header bytes from the network");
        }

        handler.add_bytes(byte_stream, tcp_bytes);
    }

    size_t body_len = handler.get_content_length();
    while (handler.get_body_size() < body_len) {
        tcp_bytes = read(fd, byte_stream, MAX_BUFLEN);

        if (tcp_bytes <= 0) {
            throw pcom::Errors("Failed to recv body bytes from the network");
        }

        handler.add_bytes(byte_stream, tcp_bytes);
    }
}
