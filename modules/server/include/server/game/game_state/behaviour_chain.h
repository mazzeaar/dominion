#pragma once

#include <server/game/game_state/behaviour_registry.h>
#include <shared/utils/logger.h>
#include <vector>

namespace server
{
    /**
     * @brief This class stores the state of the card that is currently being played. it will be accessed from the game
     * interface and handle incoming/outcoming messages. it will also apply the behaviours to the player/gamestate.
     */
    class BehaviourChain
    {
        static const size_t INVALID_IDX;
        static const std::string INVALID_CARD;

        std::string current_card;
        size_t behaviour_idx;
        std::unique_ptr<BehaviourRegistry> behaviour_registry;

        std::vector<std::unique_ptr<BehaviourBase>> behaviours_list;

        // To dicuss: Should we have a list of tuples (in_response_to, player_id) of expected responses?

    public:
        BehaviourChain() :
            current_card(INVALID_CARD), behaviour_idx(INVALID_IDX),
            behaviour_registry(std::make_unique<BehaviourRegistry>())
        {
            LOG(DEBUG) << "Created a new BehaviourChain";
        }

        void loadBehaviours(const std::string &card_id);
        void resetBehaviours();

        /**
         * @brief Using a raw pointer here as ownership transfer would be annoying.
         *
         * @param game_state
         * @param action_decision
         * @return std::optional<std::unique_ptr<shared::ActionOrder>>
         */
        std::optional<std::unique_ptr<shared::ActionOrder>>
        receiveAction(server::GameState &game_state, Player::id_t player_id,
                      std::optional<std::unique_ptr<shared::ActionDecision>> action_decision,
                      std::optional<std::string> in_response_to);

        bool empty() const { return behaviour_idx == INVALID_IDX && current_card == INVALID_CARD; }

    private:
        void advance() { ++behaviour_idx; }
        bool hasNext() const { return behaviour_idx < behaviours_list.size(); }

        const BehaviourBase &getBehaviour() const { return *behaviours_list[behaviour_idx]; }
    };

    inline constexpr size_t BehaviourChain::INVALID_IDX = static_cast<size_t>(-1);
    inline const std::string BehaviourChain::INVALID_CARD = "INVALID_CARD";
} // namespace server
