#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/uio.h>
#include <unistd.h>
#include <poll.h>
#include "./utils.hpp"
#include "./errors.hpp"
#include "./httphandler.hpp"

class pcom::TcpSocket {
    private:
        int fd;
        struct sockaddr_in addr;

    public:
        TcpSocket(
            const char *host_ip,
            uint16_t host_port
        );

        void open_connection();
        void send_stream(HttpHandler& handler);
        void recv_stream(HttpHandler& handler);

        ~TcpSocket();
};
