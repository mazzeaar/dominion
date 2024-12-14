
#pragma once

#include <map>
#include <memory>
#include <string>

#include <server/lobbies/lobby.h>
#include <server/network/message_interface.h>

#include <shared/game/game_state/reduced_game_state.h>
#include <shared/message_types.h>
#include "server/network/basic_network.h"

namespace server
{
    /**
     * @brief The lobby manager contains all the running games / lobbies.
     *
     * The lobby manager is responsible for creating, joining and starting games.
     * It also receives actions from players and passes them on to the correct game.
     */
    class LobbyManager
    {
    public:
        // TODO: The message interface should not be passed to the constructor, but to the methods that need it.

        /**
         * @brief Create a new lobby manager.
         *
         * @param message_interface The message interface to send messages to the players.
         */
        LobbyManager(std::shared_ptr<MessageInterface> message_interface) : message_interface(message_interface){};

        /**
         * @brief The manager will now receive a message and only handle the lobby creation.
         * All other messages get passed on to the lobby
         *
         * @param message
         */
        void handleMessage(std::unique_ptr<shared::ClientToServerMessage> &message);

        /**
         * @brief Get the games that are currently running.
         *
         * THIS IS ONLY FOR TESTING. WOULD BE NICE TO REMOVE THIS
         *
         * @return A const reference to the map of lobby ids.
         */
        const std::map<std::string, std::shared_ptr<Lobby>> &getGames() const { return games; };

        /**
         * @brief Remove a player from his lobby and close the lobby if the game is in progress
         */
        void removePlayer(std::string &lobby_id, player_id_t &player_id);

    private:
        std::map<std::string, std::shared_ptr<Lobby>> games;
        std::shared_ptr<MessageInterface> message_interface;

        /**
         * @brief Create a new lobby.
         * This will create a new lobby and add it to the list of games. The game master will be added to the lobby.
         *
         * @param request The CreateLobbyRequestMessage to create the lobby with.
         */
        void createLobby(std::unique_ptr<shared::CreateLobbyRequestMessage> &request);

        /**
         * @brief Check if a lobby exists.
         *
         * @param lobby_id The id of the lobby to check.
         *
         * @return True if the lobby exists, false otherwise.
         */
        inline bool lobbyExists(std::string lobby_id) { return games.find(lobby_id) != games.end(); }
    };
} // namespace server
