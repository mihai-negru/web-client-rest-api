#include "./include/tcpsocket.hpp"

#define MAX_BUFLEN 4096

pcom::TcpSocket::TcpSocket(
    const char *host_ip, uint16_t host_port, int ip_type, int socket_type, int flag
) {
    if ((host_ip == nullptr) || (ip_type < 0) || (socket_type < 0) || (flag < 0)) {
        throw pcom::Errors("Wrong socket input values.");
    }

    if (host_port < 1024) {
        throw pcom::Errors("Reserved port number.");
    }

    struct sockaddr_in serv_addr;
    
    fd = socket(ip_type, socket_type, flag);
    if (fd < 0) {
        throw pcom::Errors("Could not open a socket.");
    }

    memset(&serv_addr, 0, sizeof serv_addr);
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(host_port);
    inet_aton(host_ip, &serv_addr.sin_addr);

    if (connect(fd, (struct sockaddr *) &serv_addr, sizeof serv_addr) < 0) {
        throw pcom::Errors("Could not connect to the server.");
    }
}

pcom::TcpSocket::~TcpSocket() {
    if (fd > 0) {
        close(fd);
    }
}

void pcom::TcpSocket::send_stream(HttpHandler& handler) {
    const char *byte_stream = handler.byte_stream();
    ssize_t bytes_send = 0, tcp_bytes = 0;
    ssize_t stream_len = strlen(byte_stream);

    while (bytes_send < stream_len) {
        tcp_bytes = send(fd, byte_stream + bytes_send, stream_len - bytes_send, 0);

        if (tcp_bytes <= 0) {
            delete[] byte_stream;

            throw pcom::Errors("Failed to send byte stream over the network.");
        }

        bytes_send += tcp_bytes;
    }

    delete[] byte_stream;
}

void pcom::TcpSocket::recv_stream(HttpHandler& handler) {
    handler.clear();

    char byte_stream[MAX_BUFLEN];
    ssize_t tcp_bytes = 0;

    while (!handler.is_header_complete()) {
        tcp_bytes = recv(fd, byte_stream, MAX_BUFLEN, 0);

        if (tcp_bytes <= 0) {
            throw pcom::Errors("Failed to recv header bytes from the network");
        }

        handler.add_bytes(byte_stream, tcp_bytes);
    }

    size_t body_len = handler.get_content_length();
    while (handler.get_body_size() < body_len) {
        tcp_bytes = recv(fd, byte_stream, MAX_BUFLEN, 0);

        if (tcp_bytes <= 0) {
            throw pcom::Errors("Failed to recv body bytes from the network");
        }

        handler.add_bytes(byte_stream, tcp_bytes);
    }
}
