#include <string>

#include "server/server_network_manager.h"

int main()
{
    // create a message interface, which creates a server network manager, which listens endlessly for connections
    server::MessageInterface interface;
    interface.init();

    // Purely for testing purposes
    std::string input;
    std::string player_id("Kevin");
    std::cin >> input;
    std::cout << "Got Input" << std::endl;
    shared::ResultResponseMessage response("Gru", "Dave", true);
    interface.send_message(std::make_unique<shared::ResultResponseMessage>(response), player_id);

   std::cout << "Waiting for exit signal(type exit):" << std::endl;
    while ( input != "exit" ) {
        std::cin >> input;
    }
    return 0;
}