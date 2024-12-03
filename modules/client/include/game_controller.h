#pragma once

#include <client_network_manager.h>
#include <gui.h>
#include <shared/game/game_state/player_base.h>
#include <windows/game_window.h>

#include <shared/message_types.h>

namespace client
{

    enum ClientState
    {
        LOGIN_SCREEN,
        CREATING_LOBBY,
        JOINING_LOBBY,
        IN_LOBBY,
        IN_GAME
    };

    class GameController
    {

    public:
        /**
         * @brief Initialize the game controller
         * @param gameWindow The main window of the game
         */
        GameController(GameWindow *game_window);

        /**
         * @brief Destroy the game controller
         */
        ~GameController() = default;

        /**
         * @brief Show the connection panel and send create lobby request
         */
        void createLobby();

        /**
         * @brief Join a Lobby
         */
        void joinLobby();

        /**
         * @brief Show the main game panel
         *
         */
        void startGame();

        void buyCard(const std::string &card_id);
        void playCard(const std::string &card_id);
        void endActionPhase();
        void endTurn();
        void sendRequest(std::unique_ptr<shared::ClientToServerMessage> req);

        /**
         * @brief Receive a message from the server
         * @param msg The message received
         */
        void receiveMessage(std::unique_ptr<shared::ServerToClientMessage> msg);

        // TODO: This is for testing purposes only and will be removed later
        void skipToVictoryScreen();

        // TODO: This is for testing purposes only and will be removed later
        void skipToGamePanel();

        void showError(const std::string &title, const std::string &message) { _gui->showError(title, message); }

        void showStatus(const std::string &message) { _gui->showStatus(message); }

    private:
        void receiveActionOrderMessage(std::unique_ptr<shared::ActionOrderMessage> msg);
        void receiveCreateLobbyResponseMessage(std::unique_ptr<shared::CreateLobbyResponseMessage> msg);
        void receiveJoinLobbyBroadcastMessage(std::unique_ptr<shared::JoinLobbyBroadcastMessage> msg);
        void receiveResultResponseMessage(std::unique_ptr<shared::ResultResponseMessage> msg);
        void receiveGameStateMessage(std::unique_ptr<shared::GameStateMessage> msg);
        void receiveStartGameBroadcastMessage(std::unique_ptr<shared::StartGameBroadcastMessage> msg);

        shared::PlayerBase::id_t getPlayerName();

        std::unique_ptr<Gui> _gui;

        ClientNetworkManager *_clientNetworkManager;

        ClientState _clientState;
        std::unique_ptr<reduced::GameState> _gameState;
        shared::PlayerBase::id_t _playerName;
        std::string _gameName;
    };

} // namespace client
