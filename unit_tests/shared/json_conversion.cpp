
#include "gtest/gtest.h"

#include <shared/message_types.h>

using namespace shared;

// ======= SERVER TO CLIENT MESSAGES ======= //

TEST(SharedLibraryTest, GameStateMessageTwoWayConversion)
{
    GameStateMessage original_message;
    original_message.game_id = "123";
    original_message.message_id = "456";
    original_message.in_response_to = "789";

    std::string json = original_message.to_json();

    std::unique_ptr<ServerToClientMessage> base_message;
    base_message = ServerToClientMessage::from_json(json);

    std::unique_ptr<GameStateMessage> parsed_message(dynamic_cast<GameStateMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
    ASSERT_TRUE(parsed_message->in_response_to);
    ASSERT_EQ(parsed_message->in_response_to.value(), "789");
}

TEST(SharedLibraryTest, CreateLobbyResponseMessageTwoWayConversion)
{
    std::vector<CardBase::id_t> available_cards = {"copper", "silver", "gold", "estate", "duchy", "province", "curse"};
    CreateLobbyResponseMessage original_message(available_cards, std::nullopt);
    original_message.game_id = "123";
    original_message.message_id = "456";

    std::string json = original_message.to_json();

    std::unique_ptr<ServerToClientMessage> base_message;
    base_message = ServerToClientMessage::from_json(json);

    std::unique_ptr<CreateLobbyResponseMessage> parsed_message(
            dynamic_cast<CreateLobbyResponseMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
    ASSERT_FALSE(parsed_message->in_response_to);
    ASSERT_EQ(parsed_message->available_cards,
              std::vector<std::string>({"copper", "silver", "gold", "estate", "duchy", "province", "curse"}));
}

TEST(SharedLibraryTest, JoinLobbyBroadcastMessageTwoWayConversion)
{
    shared::PlayerBase::id_t player_id = "player1";
    JoinLobbyBroadcastMessage original_message(player_id);
    original_message.game_id = "123";
    original_message.message_id = "456";

    std::string json = original_message.to_json();

    std::unique_ptr<ServerToClientMessage> base_message;
    base_message = ServerToClientMessage::from_json(json);

    std::unique_ptr<JoinLobbyBroadcastMessage> parsed_message(
            dynamic_cast<JoinLobbyBroadcastMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
    ASSERT_EQ(parsed_message->player_id, "player1");
}

TEST(SharedLibraryTest, StartGameBroadcastMessageTwoWayConversion)
{
    StartGameBroadcastMessage original_message;
    original_message.game_id = "123";
    original_message.message_id = "456";

    std::string json = original_message.to_json();

    std::unique_ptr<ServerToClientMessage> base_message;
    base_message = ServerToClientMessage::from_json(json);

    std::unique_ptr<StartGameBroadcastMessage> parsed_message(
            dynamic_cast<StartGameBroadcastMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
}

TEST(SharedLibraryTest, EndGameBroadcastMessageTwoWayConversion)
{
    EndGameBroadcastMessage original_message;
    original_message.game_id = "123";
    original_message.message_id = "456";

    std::string json = original_message.to_json();

    std::unique_ptr<ServerToClientMessage> base_message;
    base_message = ServerToClientMessage::from_json(json);

    std::unique_ptr<EndGameBroadcastMessage> parsed_message(
            dynamic_cast<EndGameBroadcastMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
}

TEST(SharedLibraryTest, ResultResponseMessageTwoWayConversion)
{
    bool success = true;
    std::string in_response_to = "hui";
    std::string additional_information = "hey";
    ResultResponseMessage original_message(success, in_response_to, additional_information);
    original_message.game_id = "123";
    original_message.message_id = "456";

    std::string json = original_message.to_json();

    std::unique_ptr<ServerToClientMessage> base_message;
    base_message = ServerToClientMessage::from_json(json);

    std::unique_ptr<ResultResponseMessage> parsed_message(
            dynamic_cast<ResultResponseMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
    ASSERT_TRUE(parsed_message->in_response_to);
    ASSERT_EQ(parsed_message->in_response_to.value(), "hui");
    ASSERT_TRUE(parsed_message->success);
    ASSERT_EQ(parsed_message->success, true);
    ASSERT_TRUE(parsed_message->additional_information);
    ASSERT_EQ(parsed_message->additional_information.value(), "hey");
}

TEST(SharedLibraryTest, ActionOrderMessageTwoWayConversion)
{
    ActionOrderMessage original_message;
    original_message.game_id = "123";
    original_message.message_id = "456";
    original_message.description = std::nullopt;

    std::string json = original_message.to_json();

    std::unique_ptr<ServerToClientMessage> base_message;
    base_message = ServerToClientMessage::from_json(json);

    std::unique_ptr<ActionOrderMessage> parsed_message(dynamic_cast<ActionOrderMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
    ASSERT_FALSE(parsed_message->description);
}

// ======= CLIENT TO SERVER MESSAGES ======= //

TEST(SharedLibraryTest, GameStateRequestMessageTwoWayConversion)
{
    GameStateRequestMessage original_message;
    original_message.game_id = "123";
    original_message.message_id = "456";

    std::string json = original_message.to_json();

    std::unique_ptr<ClientToServerMessage> base_message;
    base_message = ClientToServerMessage::from_json(json);

    std::unique_ptr<GameStateRequestMessage> parsed_message(
            dynamic_cast<GameStateRequestMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
}

TEST(SharedLibraryTest, CreateLobbyRequestMessageTwoWayConversion)
{
    CreateLobbyRequestMessage original_message;
    original_message.game_id = "123";
    original_message.message_id = "456";
    original_message.player_id = "player1";

    std::string json = original_message.to_json();

    std::unique_ptr<ClientToServerMessage> base_message;
    base_message = ClientToServerMessage::from_json(json);

    std::unique_ptr<CreateLobbyRequestMessage> parsed_message(
            dynamic_cast<CreateLobbyRequestMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
    ASSERT_EQ(parsed_message->player_id, "player1");
}

TEST(SharedLibraryTest, JoinLobbyRequestMessageTwoWayConversion)
{
    JoinLobbyRequestMessage original_message;
    original_message.game_id = "123";
    original_message.message_id = "456";
    original_message.player_id = "player1";

    std::string json = original_message.to_json();

    std::unique_ptr<ClientToServerMessage> base_message;
    base_message = ClientToServerMessage::from_json(json);

    std::unique_ptr<JoinLobbyRequestMessage> parsed_message(
            dynamic_cast<JoinLobbyRequestMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
    ASSERT_EQ(parsed_message->player_id, "player1");
}

TEST(SharedLibraryTest, StartGameRequestMessageTwoWayConversion)
{
    std::vector<std::string> cards = {"village",    "smithy",  "market", "council_room", "festival",
                                      "laboratory", "library", "mine",   "witch",        "adventurer"};

    StartGameRequestMessage original_message;
    original_message.game_id = "123";
    original_message.message_id = "456";
    original_message.selected_cards = cards;

    std::string json = original_message.to_json();

    std::unique_ptr<ClientToServerMessage> base_message;
    base_message = ClientToServerMessage::from_json(json);

    std::unique_ptr<StartGameRequestMessage> parsed_message(
            dynamic_cast<StartGameRequestMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
    ASSERT_EQ(parsed_message->selected_cards, cards);
}

TEST(SharedLibraryTest, ActionDecisionMessageTwoWayConversion)
{
    ActionDecisionMessage original_message;
    original_message.game_id = "123";
    original_message.message_id = "456";

    std::string json = original_message.to_json();

    std::unique_ptr<ClientToServerMessage> base_message;
    base_message = ClientToServerMessage::from_json(json);

    std::unique_ptr<ActionDecisionMessage> parsed_message(
            dynamic_cast<ActionDecisionMessage *>(base_message.release()));

    ASSERT_NE(parsed_message, nullptr);

    ASSERT_EQ(parsed_message->game_id, "123");
    ASSERT_EQ(parsed_message->message_id, "456");
}
