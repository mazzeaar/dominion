#include <algorithm>
#include <random>
#include <ranges>

#include <server/game/game_state/server_player.h>
#include <shared/utils/assert.h>

namespace server
{
    shared::ReducedPlayer::ptr_t Player::get_reduced_player()
    {
        return shared::ReducedPlayer::make(static_cast<shared::PlayerBase>(*this), hand_cards);
    }

    shared::ReducedEnemy::ptr_t Player::get_reduced_enemy()
    {
        return shared::ReducedEnemy::make(static_cast<shared::PlayerBase>(*this), hand_cards.size());
    }

    void Player::reset_values()
    {
        actions = 1;
        buys = 1;
        treasure = 0;
        victory_points = 0;
    }

    void Player::end_turn()
    {
        reset_values();

        move<HAND, DISCARD_PILE>();
        move<PLAYED_CARDS, DISCARD_PILE>();

        shuffle<DISCARD_PILE>();
        move<DISCARD_PILE, DRAW_PILE_BOTTOM>();

        draw(5);
    }

} // namespace server
