
#include <gtest/gtest.h>
#include <vector>

#include <shared/message_types.h>
#include <shared/utils/test_helpers.h>

using namespace shared;

// ======= SERVER -> CLIENT ======= //

TEST(SharedLibraryTest, GameStateRequestMessageEquality)
{
    GameStateRequestMessage message1("game1", "player1", "message1");
    ASSERT_EQ(message1, message1);

    GameStateRequestMessage message2("game1", "player1", "message1");
    ASSERT_EQ(message1, message2);

    GameStateRequestMessage message3("game2", "player1", "message1");
    ASSERT_NE(message1, message3);

    GameStateRequestMessage message4("game1", "player1", "message2");
    ASSERT_NE(message1, message4);

    GameStateRequestMessage message5("game1", "player2", "message1");
    ASSERT_NE(message1, message5);
}

TEST(SharedLibraryTest, CreateLobbyRequestMessageEquality)
{
    CreateLobbyRequestMessage message1("game1", "player1", "message1");
    ASSERT_EQ(message1, message1);

    CreateLobbyRequestMessage message2("game1", "player1", "message1");
    ASSERT_EQ(message1, message2);

    CreateLobbyRequestMessage message3("game2", "player1", "message1");
    ASSERT_NE(message1, message3);

    CreateLobbyRequestMessage message4("game1", "player1", "message2");
    ASSERT_NE(message1, message4);

    CreateLobbyRequestMessage message5("game1", "player2", "message1");
    ASSERT_NE(message1, message5);
}

TEST(SharedLibraryTest, JoinLobbyRequestMessageEquality)
{
    JoinLobbyRequestMessage message1("game1", "player1", "message1");
    ASSERT_EQ(message1, message1);

    JoinLobbyRequestMessage message2("game1", "player1", "message1");
    ASSERT_EQ(message1, message2);

    JoinLobbyRequestMessage message3("game2", "player1", "message1");
    ASSERT_NE(message1, message3);

    JoinLobbyRequestMessage message4("game1", "player1", "message2");
    ASSERT_NE(message1, message4);

    JoinLobbyRequestMessage message5("game1", "player2", "message1");
    ASSERT_NE(message1, message5);
}

TEST(SharedLibraryTest, StartGameRequestMessageEquality)
{
    std::vector<CardBase::id_t> selected_cards = {"Adventurer", "Bureaucrat", "Cellar",     "Chapel", "CouncilRoom",
                                                  "Festival",   "Gardens",    "Laboratory", "Market", "Militia"};
    StartGameRequestMessage message1("game1", "player1", selected_cards, "message1");
    ASSERT_EQ(message1, message1);

    StartGameRequestMessage message2("game1", "player1", selected_cards, "message1");
    ASSERT_EQ(message1, message2);

    StartGameRequestMessage message3("game2", "player1", selected_cards, "message1");
    ASSERT_NE(message1, message3);

    StartGameRequestMessage message4("game1", "player1", selected_cards, "message2");
    ASSERT_NE(message1, message4);

    StartGameRequestMessage message5("game1", "player2", selected_cards, "message1");
    ASSERT_NE(message1, message5);

    std::vector<CardBase::id_t> selected_cards2 = {"Adventurer", "Bureaucrat", "Cellar",     "Chapel", "CouncilRoom",
                                                   "Festival",   "Gardens",    "Laboratory", "Market", "Moat"};
    StartGameRequestMessage message6("game1", "player1", selected_cards2, "message1");
    ASSERT_NE(message1, message6);
}

TEST(SharedLibraryTest, ActionDecisionMessage)
{
    std::unique_ptr<ActionDecision> decision1 = std::make_unique<PlayActionCardDecision>("Village");
    ActionDecisionMessage message1("game1", "player1", std::move(decision1), "message0", "message1");
    ASSERT_EQ(message1, message1);

    std::unique_ptr<ActionDecision> decision2 = std::make_unique<PlayActionCardDecision>("Village");
    ActionDecisionMessage message2("game1", "player1", std::move(decision2), "message0", "message1");
    ASSERT_EQ(message1, message2);

    std::unique_ptr<ActionDecision> decision3 = std::make_unique<PlayActionCardDecision>("Sentry");
    ActionDecisionMessage message3("game2", "player1", std::move(decision3), "message0", "message1");
    ASSERT_NE(message1, message3);

    std::unique_ptr<ActionDecision> decision4 = std::make_unique<PlayActionCardDecision>("Village");
    ActionDecisionMessage message4("game1", "player1", std::move(decision4), "message0", "message2");
    ASSERT_NE(message1, message4);

    std::unique_ptr<ActionDecision> decision5 = std::make_unique<PlayActionCardDecision>("Village");
    ActionDecisionMessage message5("game1", "player2", std::move(decision5), "message0", "message1");
    ASSERT_NE(message1, message5);

    std::unique_ptr<ActionDecision> decision6 = std::make_unique<BuyCardDecision>("Copper");
    ActionDecisionMessage message6("game1", "player1", std::move(decision6), "message0", "message1");
    ASSERT_NE(message1, message6);

    std::unique_ptr<ActionDecision> decision7 = std::make_unique<PlayActionCardDecision>("Village");
    ActionDecisionMessage message7("game1", "player1", std::move(decision7), "", "message1");
    ASSERT_NE(message1, message7);
}

// ======= CLIENT -> SERVER ======= //

/* TODO: reenable
TEST(SharedLibraryTest, GameStateMessageEquality)
{
    // TODO: Implement game state
    ReducedGameState *game_state_p1 = nullptr;
    ASSERT_NE(game_state_p1, nullptr);
    ReducedGameState game_state1 = *game_state_p1;
    GameStateMessage message1("game1", "message1", game_state1, "message0");
    ASSERT_EQ(message1, message1);

    GameStateMessage message2("game1", "message1", game_state1, "message0");
    ASSERT_EQ(message1, message2);

    GameStateMessage message3("game2", "message1", game_state1, "message0");
    ASSERT_NE(message1, message3);

    GameStateMessage message4("game1", "message2", game_state1, "message0");
    ASSERT_NE(message1, message4);

    // TODO: Implement game state
    ReducedGameState *game_state_p2 = nullptr;
    ASSERT_NE(game_state_p2, nullptr);
    ReducedGameState game_state2 = *game_state_p2;
    GameStateMessage message5("game1", "message1", game_state2, "message0");
    ASSERT_NE(message1, message5);

    GameStateMessage message6("game1", "message1", game_state1);
    ASSERT_NE(message1, message6);
}
*/

TEST(SharedLibraryTest, CreateLobbyResponseMessageEquality)
{
    CreateLobbyResponseMessage message1("game1", "message0", "message1");
    ASSERT_EQ(message1, message1);

    CreateLobbyResponseMessage message2("game1", "message0", "message1");
    ASSERT_EQ(message1, message2);

    CreateLobbyResponseMessage message3("game2", "message0", "message1");
    ASSERT_NE(message1, message3);

    CreateLobbyResponseMessage message4("game1", "message0", "message2");
    ASSERT_NE(message1, message4);

    CreateLobbyResponseMessage message6("game1", "", "message1");
    ASSERT_NE(message1, message6);
}

TEST(SharedLibraryTest, JoinLobbyBroadcastMessageEquality)
{
    JoinLobbyBroadcastMessage message1("game1", {"player_1", "player_2"}, "message1");
    ASSERT_EQ(message1, message1);

    JoinLobbyBroadcastMessage message2("game1", {"player_1", "player_2"}, "message1");
    ASSERT_EQ(message1, message2);

    JoinLobbyBroadcastMessage message3("game2", {"player_1", "player_2"}, "message1");
    ASSERT_NE(message1, message3);

    JoinLobbyBroadcastMessage message4("game1", {"player_1", "player_2"}, "message2");
    ASSERT_NE(message1, message4);

    JoinLobbyBroadcastMessage message5("game1", {"player_1", "player_3"}, "message1");
    ASSERT_NE(message1, message5);
}

TEST(SharedLibraryTest, StartGameBroadcastMessageEquality)
{
    StartGameBroadcastMessage message1("game1", "message1");
    ASSERT_EQ(message1, message1);

    StartGameBroadcastMessage message2("game1", "message1");
    ASSERT_EQ(message1, message2);

    StartGameBroadcastMessage message3("game2", "message1");
    ASSERT_NE(message1, message3);

    StartGameBroadcastMessage message4("game1", "message2");
    ASSERT_NE(message1, message4);
}

TEST(SharedLibraryTest, EndGameBroadcastMessageEquality)
{
    std::vector<PlayerResult> results1 = {{"Alice", 12}, {"Bob", 8}, {"Charlie", -2}};

    EndGameBroadcastMessage message1("game1", results1, "message1");
    ASSERT_EQ(message1, message1);

    EndGameBroadcastMessage message2("game1", results1, "message1");
    ASSERT_EQ(message1, message2);

    EndGameBroadcastMessage message3("game2", results1, "message1");
    ASSERT_NE(message1, message3);

    std::vector<PlayerResult> results2 = {{"Alice", 12}, {"Bob", 8}, {"Charlie", -3}};
    EndGameBroadcastMessage message4("game1", results2, "message1");
    ASSERT_NE(message1, message4);

    EndGameBroadcastMessage message5("game1", results1, "message2");
    ASSERT_NE(message1, message5);
}

TEST(SharedLibraryTest, ResultResponseMessageEquality)
{
    bool success = false;
    ResultResponseMessage message1("game1", success, "message0", "failed because of reasons", "message1");
    ASSERT_EQ(message1, message1);

    ResultResponseMessage message2("game1", success, "message0", "failed because of reasons", "message1");
    ASSERT_EQ(message1, message2);

    ResultResponseMessage message3("game2", success, "message0", "failed because of reasons", "message1");
    ASSERT_NE(message1, message3);

    ResultResponseMessage message4("game1", success, "message0", "failed because of reasons", "message2");
    ASSERT_NE(message1, message4);

    ResultResponseMessage message5("game1", !success, "message0", "failed because of reasons", "message1");
    ASSERT_NE(message1, message5);

    ResultResponseMessage message6("game1", success, "message1", "failed because of reasons", "message1");
    ASSERT_NE(message1, message6);

    ResultResponseMessage message7("game1", success, "message0", "", "message1");
    ASSERT_NE(message1, message7);
}


TEST(SharedLibraryTest, ActionOrderMessageEquality)
{
    size_t n_players = 3;

    auto kingdom_cards = test_helper::getValidRandomKingdomCards(10);
    auto hand_cards = test_helper::getRandomCards(5);
    std::vector<size_t> enemy_hand_cards = {5, 5};

    std::unique_ptr<ActionOrder> order1 = std::make_unique<ChooseFromStagedOrder>(
            1, 1, shared::ChooseFromOrder::AllowedChoice::DISCARD, std::vector<shared::CardBase::id_t>(1, "a card"));
    ActionOrderMessage message1(
            "game1", std::move(order1),
            test_helper::getReducedGameStatePtr(n_players, kingdom_cards, hand_cards, enemy_hand_cards), "description",
            "message1");
    ASSERT_EQ(message1, message1);

    std::unique_ptr<ActionOrder> order2 = std::make_unique<ChooseFromStagedOrder>(
            1, 1, shared::ChooseFromOrder::AllowedChoice::DISCARD, std::vector<shared::CardBase::id_t>(1, "a card"));
    ActionOrderMessage message2(
            "game1", std::move(order2),
            test_helper::getReducedGameStatePtr(n_players, kingdom_cards, hand_cards, enemy_hand_cards), "description",
            "message1");
    ASSERT_EQ(message1, message2);

    std::unique_ptr<ActionOrder> order3 = std::make_unique<ChooseFromStagedOrder>(
            1, 1, shared::ChooseFromOrder::AllowedChoice::DISCARD, std::vector<shared::CardBase::id_t>(1, "a card"));
    ActionOrderMessage message3("game2", std::move(order3), test_helper::getReducedGameStatePtr(n_players),
                                "description", "message1");
    ASSERT_NE(message1, message3);

    std::unique_ptr<ActionOrder> order4 = std::make_unique<ChooseFromStagedOrder>(
            1, 1, shared::ChooseFromOrder::AllowedChoice::DISCARD, std::vector<shared::CardBase::id_t>(1, "a card"));
    ActionOrderMessage message4("game1", std::move(order4), test_helper::getReducedGameStatePtr(n_players),
                                "description", "message2");
    ASSERT_NE(message1, message4);

    std::unique_ptr<ActionOrder> order5 = std::make_unique<ChooseFromStagedOrder>(
            1, 1, shared::ChooseFromOrder::AllowedChoice::DISCARD, std::vector<shared::CardBase::id_t>(1, "a card"));
    ActionOrderMessage message5("game1", std::move(order5), test_helper::getReducedGameStatePtr(n_players),
                                "description", "message1");
    ASSERT_NE(message1, message5);

    std::unique_ptr<ActionOrder> order6 = std::make_unique<ChooseFromStagedOrder>(
            1, 1, shared::ChooseFromOrder::AllowedChoice::DISCARD, std::vector<shared::CardBase::id_t>(1, "a card"));
    ActionOrderMessage message6("game1", std::move(order6), test_helper::getReducedGameStatePtr(n_players), "message1");
    ASSERT_NE(message1, message6);

    std::unique_ptr<ActionOrder> order7 = std::make_unique<ChooseFromStagedOrder>(
            1, 1, shared::ChooseFromOrder::AllowedChoice::DISCARD, std::vector<shared::CardBase::id_t>(1, "a card"));
    ActionOrderMessage message7("game1", std::move(order7), test_helper::getReducedGameStatePtr(n_players),
                                "description0", "message1");
    ASSERT_NE(message1, message7);
}
