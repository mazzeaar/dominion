#include <client/client_network_manager.h>

#include <sstream>
#include <sockpp/exception.h>
#include "rapidjson/document.h"
#include "game_controller.h"

// initialize static members
sockpp::tcp_connector *ClientNetworkManager::_connection = nullptr;

bool ClientNetworkManager::_connectionSuccess = false;
bool ClientNetworkManager::_failedToConnect = false;


void ::ClientNetworkManager::init(const std::string &host, const uint16_t port)
{
    // initialize sockpp framework
    sockpp::socket_initializer sockInit;

    // reset connection status
    ClientNetworkManager::_connectionSuccess = false;
    ClientNetworkManager::_failedToConnect = false;

    // delete exiting connection and create new one
    if ( ClientNetworkManager::_connection != nullptr ) {
        ClientNetworkManager::_connection->shutdown();
        delete ClientNetworkManager::_connection;
    }
    ClientNetworkManager::_connection = new sockpp::tcp_connector();

    // try to connect to server
    if ( ClientNetworkManager::connect(host, port) ) {
        //        GameController::showStatus("Connected to " + host + ":" + std::to_string(port));
        ClientNetworkManager::_connectionSuccess = true;
        // start network thread
        ClientListener *clientlistener = new ClientListener(ClientNetworkManager::_connection);

        if ( clientlistener->Run() != wxTHREAD_NO_ERROR ) {
            //            GameController::showError("Connection error", "Could not create client network thread");
        }

    } else {
        ClientNetworkManager::_failedToConnect = true;
        //        GameController::showStatus("Not connected");
    }
}


bool ClientNetworkManager::connect(const std::string &host, const uint16_t port)
{

    // create sockpp address and catch any errors
    sockpp::inet_address address;
    try {
        address = sockpp::inet_address(host, port);
    } catch ( const sockpp::getaddrinfo_error &e ) {
        //        GameController::showError("Connection error", "Failed to resolve address " + e.hostname());
        return false;
    }

    // establish connection to given address
    if ( !ClientNetworkManager::_connection->connect(address) ) {
        //        GameController::showError("Connection error", "Failed to connect to server " + address.to_string());
        return false;
    }

    return true;
}


void ClientNetworkManager::sendRequest(std::string message)
{

    // wait until network is connected (max. 5 seconds)
    int connectionCheckCounter = 0;
    while ( !ClientNetworkManager::_connectionSuccess && !ClientNetworkManager::_failedToConnect &&
            connectionCheckCounter < 200 ) {
        wxMilliSleep(25);
        connectionCheckCounter++;
    }

    // do not continue if failed to connect to server
    if ( ClientNetworkManager::_failedToConnect ) {
        return;
    }

    if ( ClientNetworkManager::_connectionSuccess && ClientNetworkManager::_connection->is_connected() ) {

        // turn message into stream and prepend message length
        std::stringstream messageStream;
        messageStream << std::to_string(message.size()) << ':' << message;
        message = messageStream.str();

        // output message for debugging purposes
#ifdef PRINT_NETWORK_MESSAGES
        std::cout << "Sending request : " << message << std::endl;
#endif

        // send message to server
        ssize_t bytesSent = ClientNetworkManager::_connection->write(message);

        // if the number of bytes sent does not match the length of the message, probably something went wrong
        if ( bytesSent != ssize_t(message.length()) ) {
            // TODO Stuff for the gui team
            // GameController::showError("Network error", "Error writing to the TCP stream: " +
            // ClientNetworkManager::_connection->last_error_str());
        }

    } else {
        // TODO GUI TEAM
        // GameController::showError("Network error", "Lost connection to server");
    }
}


void ClientNetworkManager::receive_message(const std::string &message)
{

    // output message for debugging purposes
#ifdef PRINT_NETWORK_MESSAGES
    std::cout << "Received response : " << message << std::endl;
#endif
    try {
        std::unique_ptr<shared::ServerToClientMessage> res = shared::ServerToClientMessage::from_json(message);
        // TODO Process the server message
        std::cout << "Received Messaged: " << message << std::endl;
        client::GameController::receive_message(std::move(res));

    } catch ( std::exception &e ) {
        //        GameController::showError("JSON parsing error", "Failed to parse message from server:\n" + message +
        //        "\n" + (std::string) e.what());
    }
}

void ClientNetworkManager::shutdown() { ClientNetworkManager::_connection->shutdown(); }
