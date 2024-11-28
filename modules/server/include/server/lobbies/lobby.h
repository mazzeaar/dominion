#pragma once

#include <memory>
#include <string>

#include <server/game/game_interface.h>
#include <server/game/game_state.h>
#include <server/network/message_interface.h>

#include <shared/message_types.h>

namespace server
{
    /**
     * @brief A lobby is a container for a game that is being played.
     *
     * A lobby is created by a game master, who is the first player to join the lobby.
     * The game master can start the game when they want to.
     */
    class Lobby
    {
    public:
        /**
         * @brief Create a new game lobby.
         * Game master is added to the players of the lobby.
         *
         * @param game_master The player who created the lobby.
         */
        Lobby(const Player::id_t &game_master, const std::string &lobby_id);

        /**
         * @brief The lobby receives a generic message. It handles what it is responsible for and the rest gets passed
         * on to the game_interface
         *
         * @param message_interface
         * @param message
         */
        void handleMessage(MessageInterface &message_interface,
                           std::unique_ptr<shared::ClientToServerMessage> &message);

        /**
         * @brief Add a player to the lobby.
         *
         * @param message_interface The message interface to send messages to the players.
         * @param request The JoinLobbyRequestMessage to join the lobby with.
         *
         * @pre The lobby exists.
         */
        void join(MessageInterface &message_interface, std::unique_ptr<shared::JoinLobbyRequestMessage> &request);
        void startGame(MessageInterface &message_interface, std::unique_ptr<shared::StartGameRequestMessage> &request);

        void getGameState(MessageInterface &message_interface,
                          std::unique_ptr<shared::GameStateRequestMessage> &request);

        /**
         * @brief Get the players in the lobby.
         *
         * @return A const reference vector of player ids.
         */
        const std::vector<Player::id_t> &getPlayers() const { return players; }

        /**
         * @brief Get the id of the game master.
         *
         * @return The id of the game master.
         */
        const Player::id_t &getGameMaster() const { return game_master; };

    private:
        std::unique_ptr<server::GameInterface> game_interface;
        Player::id_t game_master;

        std::vector<Player::id_t> players;
        std::string lobby_id;

        /**
         * @brief Check if a player is in the lobby.
         *
         * @param player_id The id of the player to check.
         *
         * @return True if the player is in the lobby, false otherwise.
         */
        inline bool playerInLobby(const Player::id_t &player_id)
        {
            return std::any_of(players.begin(), players.end(), [&](const auto &player) { return player == player_id; });
        }

        /**
         * @brief The game_interface gets initialised only when we start a game, so we can check if a game is already
         * running this way.
         *
         * @return true
         * @return false
         */
        inline bool gameRunning() const { return game_interface != nullptr; }

        void sendGameState(MessageInterface &message_interface) const
        {
            std::for_each(players.begin(), players.end(),
                          [&](const auto &player_id)
                          {
                              LOG(INFO) << "Sending GameStateMessage in Lobby ID: " << lobby_id
                                        << " to Player ID: " << player_id;
                              message_interface.send<shared::GameStateMessage>(player_id, lobby_id,
                                                                               game_interface->getGameState(player_id));
                          });
        }
    };
} // namespace server