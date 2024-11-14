#include <shared/game/game_state/player_base.h>

#include <gtest/gtest.h>

TEST(PlayerBaseTest, ConstructorWithId)
{
    std::string player_id = "player1";
    shared::PlayerBase player(player_id);
    EXPECT_EQ(player.getId(), player_id);
    EXPECT_EQ(player.getVictoryPoints(), 0);
}

TEST(PlayerBaseTest, CopyConstructor)
{
    shared::PlayerBase original("player1");

    shared::PlayerBase copy(original);

    EXPECT_EQ(copy.getId(), original.getId());
    EXPECT_EQ(copy.getVictoryPoints(), original.getVictoryPoints());
    EXPECT_EQ(copy.getActions(), original.getActions());
    EXPECT_EQ(copy.getBuys(), original.getBuys());
    EXPECT_EQ(copy.getTreasure(), original.getTreasure());
}

TEST(PlayerBaseTest, DecreaseAvailableActions)
{
    shared::PlayerBase player("player");
    player.decActions();
    EXPECT_EQ(player.getActions(), 0);
}

TEST(PlayerBaseTest, DecreaseAvailableBuys)
{
    shared::PlayerBase player("player");
    player.decBuys();
    EXPECT_EQ(player.getBuys(), 0);
}

TEST(PlayerBaseTest, DecreaseAvailableTreasure)
{
    shared::PlayerBase player("player");
    player.decTreasure(1);
    EXPECT_EQ(player.getTreasure(), 0);
}

TEST(PlayerBaseTest, ReducedEnemyDefaultConstructor)
{
    shared::ReducedEnemy enemy(shared::PlayerBase("player"), 0);
    EXPECT_EQ(enemy.getId(), "player");
    EXPECT_EQ(enemy.getVictoryPoints(), 0);
}

TEST(PlayerBaseTest, ReducedEnemyConstructorWithHandSize)
{
    unsigned int hand_size = 5;
    shared::ReducedEnemy enemy(shared::PlayerBase("player"), hand_size);
    // We don't have a getter for hand_size, so we can't directly test it
    EXPECT_EQ(enemy.getVictoryPoints(), 0);
}

TEST(PlayerBaseTest, ReducedEnemyCopyConstructor)
{
    shared::PlayerBase base_player("enemy1");
    shared::ReducedEnemy enemy(base_player, 5);
    EXPECT_EQ(enemy.getId(), "enemy1");
    EXPECT_EQ(enemy.getVictoryPoints(), 0);
}

TEST(PlayerBaseTest, ReducedPlayerCopyConstructor)
{
    shared::PlayerBase base_player("player1");
    std::vector<shared::CardBase::id_t> hand_cards = {"Card1", "Card2"};
    shared::ReducedPlayer player(base_player, hand_cards);
    EXPECT_EQ(player.getId(), "player1");
    EXPECT_EQ(player.getVictoryPoints(), 0);
}

// Since we don't have getters for 'hand_size' in ReducedEnemy or 'hand_cards' in ReducedPlayer,
// we cannot write tests to directly check those attributes unless you add appropriate getters.
