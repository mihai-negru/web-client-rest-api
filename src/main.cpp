#include "./include/utils.hpp"
#include "./include/client.hpp"

int main(void) {

    /* Initialize the session data */
    const char *host_ip = "34.254.242.81";
    uint16_t host_port = 8080;
    std::unordered_map<pcom::InputCommand::Command, std::string> host_urls;

    /* Register all the valid urls from the server regarding to the commands */
    host_urls[pcom::InputCommand::Command::REGISTER] = "/api/v1/tema/auth/register";
    host_urls[pcom::InputCommand::Command::LOGIN] = "/api/v1/tema/auth/login";
    host_urls[pcom::InputCommand::Command::ENTER_LIBRARY] = "/api/v1/tema/library/access";
    host_urls[pcom::InputCommand::Command::GET_BOOKS] = "/api/v1/tema/library/books";
    host_urls[pcom::InputCommand::Command::GET_BOOK] = "/api/v1/tema/library/books/";
    host_urls[pcom::InputCommand::Command::ADD_BOOK] = "/api/v1/tema/library/books";
    host_urls[pcom::InputCommand::Command::DELETE_BOOK] = "/api/v1/tema/library/books";
    host_urls[pcom::InputCommand::Command::LOGOUT] = "/api/v1/tema/auth/logout";

    /* Initialize and client and start the session */
    try {
        pcom::Client client = pcom::Client(host_ip, host_port, host_urls);

        client.start_session();

    } catch(pcom::Errors& err) {
        std::cout << err.what() << '\n';
    }

    return 0;
}
