#include <server/network/basic_network.h>
#include <shared/utils/logger.h>

namespace server
{
    ssize_t BasicNetwork::sendToAddress(const std::string &message, const std::string &address)
    {
        LOG(INFO) << "Sending Message: " << message << " to Address: " << address;
        try {
            sockpp::tcp_socket *socket;

            {
                std::shared_lock<std::shared_mutex> lock(_rw_lock);
                socket = getSocket(address);
            }

            if ( !socket ) {
                LOG(ERROR) << "Failed to get socket for address: " << address;
                return ssize_t(-1);
            }

            std::stringstream ss_msg;
            ss_msg << std::to_string(message.size()) << ':' << message; // prepend message length
            ssize_t ret = socket->write(ss_msg.str()); // TODO: make this thread safe
            if ( ret < 0 ) {
                LOG(ERROR) << "Failed to send message to address: " << address
                           << ". Socket error: " << socket->last_error_str();
            } else {
                LOG(INFO) << "Successfully sent Message: " << message;
            }

            return ret;
        } catch ( const std::runtime_error &e ) {
            LOG(ERROR) << "Error in sendMessage: " << e.what();
            return ssize_t(-1); // indicate failure
        }
    }

    ssize_t BasicNetwork::sendToPlayer(const std::string &message, const player_id_t &player_id)
    {
        std::string address;

        {
            std::shared_lock<std::shared_mutex> lock(_rw_lock);
            address = getAddress(player_id);
        }

        return sendToAddress(message, address);
    }

    void BasicNetwork::addPlayerToAddress(const player_id_t &player_id, const std::string &address)
    {
        // check with shared lock
        {
            std::shared_lock<std::shared_mutex> shared_lock(_rw_lock);
            if ( !isNewPlayer(player_id) ) {
                LOG(WARN) << "Player with ID " << player_id << " is already registered.";
                return;
            }
        }

        // acquire unique lock and re-check
        {
            std::unique_lock<std::shared_mutex> lock(_rw_lock);
            if ( !isNewPlayer(player_id) ) {
                LOG(WARN) << "Player with ID " << player_id << " is already registered (after lock).";
                return;
            }
            LOG(INFO) << "Registering new client with ID: " << player_id;
            _player_id_to_address.emplace(player_id, address);
        }
    }

    void BasicNetwork::addAddressToSocket(const std::string &address, const sockpp::tcp_socket socket)
    {
        std::unique_lock<std::shared_mutex> lock(_rw_lock);

        if ( _address_to_socket.count(address) != 0 ) {
            LOG(ERROR) << "Address is already connected: " << address;
        } else {
            LOG(DEBUG) << "Adding address: " << address;
            _address_to_socket.emplace(address, socket.clone());
        }
    }

    void BasicNetwork::playerDisconnect(const player_id_t &player_id)
    {
        std::unique_lock<std::shared_mutex> lock(_rw_lock);

        auto it = _player_id_to_address.find(player_id);
        if ( it != _player_id_to_address.end() ) {
            std::string address = it->second;
            _player_id_to_address.erase(it);
            _address_to_socket.erase(address);
            LOG(INFO) << "Player with ID " << player_id << " disconnected and resources released.";
        } else {
            LOG(WARN) << "Attempted to disconnect player with ID " << player_id << ", but it was not found.";
        }
    }


    // ================================================================
    // LOCKS FOR FUNCTIONS BELOW MUST BE ACCUIRED BY CALLER
    // ================================================================

    bool BasicNetwork::isNewPlayer(const player_id_t &player_id)
    {
        // ASSUMING CALLER HOLDS THE LOCK!
        return _player_id_to_address.find(player_id) == _player_id_to_address.end();
    }

    sockpp::tcp_socket *BasicNetwork::getSocket(const std::string &address)
    {
        // ASSUMING CALLER HOLDS THE LOCK!
        auto it = _address_to_socket.find(address);
        if ( it != _address_to_socket.end() ) {
            return &(it->second);
        } else {
            LOG(ERROR) << "Cannot find socket for address: " << address;
            return nullptr;
        }
    }

    const std::string &BasicNetwork::getAddress(const player_id_t &player_id)
    {
        // ASSUMING CALLER HOLDS THE LOCK!
        auto it = _player_id_to_address.find(player_id);
        if ( it != _player_id_to_address.end() ) {
            return it->second;
        } else {
            LOG(ERROR) << "Cannot find address for player ID: " << player_id;
            throw std::runtime_error("Invalid player_id: " + player_id);
        }
    }
} // namespace server
