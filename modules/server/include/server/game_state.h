#pragma once

#include <deque>
#include <memory>
#include <vector>

#include <shared/action_decision.h>
#include <shared/game_state.h>

#include <server/message_interface.h>

namespace server
{

    class ServerBoard : public shared::Board
    {
    public:
        bool buy(shared::CardBase::id_t card);
        void trash(shared::CardBase::id_t card);
    };

    class Player : public shared::PlayerBase
    {
    public:
        std::deque<shared::CardBase::id_t> draw_pile;
        std::vector<shared::CardBase::id_t> discard_pile;
        std::vector<shared::CardBase::id_t> hand_cards;
        shared::CardBase::id_t currently_playing_card;
    };

    class GameState
    {
    public:
        // TODO: This won't work, refactor
        shared::ReducedGameState get_reduced_state(shared::PlayerBase::id_t player);
        void start_game();
        void end_game();
        void start_turn();
        void end_turn();
        void switch_player(shared::PlayerBase::id_t player);
        bool try_buy(shared::PlayerBase::id_t player, shared::CardBase::id_t card);
        bool trash(shared::PlayerBase::id_t player, shared::CardBase::id_t card);
        bool discard(shared::PlayerBase::id_t player, shared::CardBase::id_t card);
        bool draw(shared::PlayerBase::id_t player, unsigned int count);
        bool play(shared::PlayerBase::id_t player, unsigned int index);
        bool is_game_over();

        std::vector<Player> players;
        ServerBoard board;
        shared::PlayerBase::id_t current_player;
    };

    class Game
    {
    public:
        void receive_action(shared::PlayerBase::id_t player, std::unique_ptr<shared::ActionDecision> action,
                            MessageInterface &message_interface);

    private:
        GameState gameState;
    };
} // namespace server
