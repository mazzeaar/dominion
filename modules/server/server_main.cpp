
#include <server/args.h>
#include <server/network/server_network_manager.h>

int main(int argc, char *argv[])
{
    server::ServerArgs args(argc, argv);

    shared::Logger::initialize();
    shared::Logger::setLevel(args.getLogLevel());
    shared::Logger::writeTo(args.getLogFile());

    // create a message interface, which creates a server network manager, which listens endlessly for connections
    server::ServerNetworkManager server(args.getPort());

    return 0;
}
