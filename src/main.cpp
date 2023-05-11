#include "./include/utils.hpp"
#include "./include/tcpsocket.hpp"
#include "./include/inputcommand.hpp"

#define loop for(;;)

int main(void) {
    loop {
        try {
            pcom::InputCommand cmd = pcom::InputCommand();

            cmd.listen_for_command();

            if (cmd.get_command_type() == pcom::InputCommand::Command::EXIT) {
                break;
            }

        } catch(pcom::Errors& e) {
            std::cout << e.what() << "\n";
        }
    }

    return 0;
}
