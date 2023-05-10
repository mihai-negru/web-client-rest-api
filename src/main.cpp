#include "./include/utils.hpp"
#include "./include/tcpsocket.hpp"

int main(void) {
    try {
        pcom::TcpSocket sock = pcom::TcpSocket("34.254.242.81", 8080, AF_INET, SOCK_STREAM, IPPROTO_TCP);

        pcom::HttpHandler test = pcom::HttpHandler()
            .set_host_ip("34.254.242.81")
            .set_host_url("/api/v1/dummy")
            .set_content_type("application/x-www-form-urlencoded")
            .generate_post_request();

        sock.send_stream(test);

        sock.recv_stream(test);

        char *test_stream = test.byte_stream();
        std::cout << "Testing: " << test_stream << '\n';

        delete[] test_stream;
    } catch(pcom::Errors& e) {
        std::cout << e.what() << "\n";
    }

    return 0;
}
