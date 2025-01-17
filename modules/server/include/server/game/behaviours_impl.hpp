#include <server/game/behaviour_base.h>
#include <server/game/behaviour_helper.hpp>
#include <shared/utils/utils.h>

namespace server
{
    namespace behaviour
    {
// False positive of clang-tidy
// NOLINTBEGIN(bugprone-macro-parentheses)
#define DEFINE_BEHAVIOUR(name)                                                                                         \
    class name : public server::base::Behaviour                                                                        \
    {                                                                                                                  \
    public:                                                                                                            \
        inline ret_t apply(server::GameState &state, const shared::PlayerBase::id_t &requestor_id,                     \
                           server::base::Behaviour::action_decision_t action_decision = std::nullopt);                 \
    };                                                                                                                 \
    inline server::base::Behaviour::ret_t name::apply(server::GameState &game_state,                                   \
                                                      const shared::PlayerBase::id_t &requestor_id,                    \
                                                      server::base::Behaviour::action_decision_t action_decision)
// NOLINTEND(bugprone-macro-parentheses)

// False positive of clang-tidy
// NOLINTBEGIN(bugprone-macro-parentheses)
#define DEFINE_TEMPLATED_BEHAVIOUR(name, template_type, template_name)                                                 \
    template <template_type template_name>                                                                             \
    class name : public server::base::Behaviour                                                                        \
    {                                                                                                                  \
    public:                                                                                                            \
        inline ret_t apply(server::GameState &state, const shared::PlayerBase::id_t &requestor_id,                     \
                           server::base::Behaviour::action_decision_t action_decision = std::nullopt);                 \
    };                                                                                                                 \
    template <template_type template_name>                                                                             \
    inline server::base::Behaviour::ret_t name<template_name>::apply(                                                  \
            server::GameState &game_state, const shared::PlayerBase::id_t &requestor_id,                               \
            server::base::Behaviour::action_decision_t action_decision)
// NOLINTEND(bugprone-macro-parentheses)

// ================================
// HELPER MACROS
// ================================

// call this at the top of your behaviour to log the call.
#define LOG_CALL LOG(INFO) << "Applying " << CLASS_NAME << " to player " << requestor_id

// call this if the linter is beeing a lil bitch.
#define SUPPRESS_UNUSED_VAR_WARNING(variable) (void)(variable)

#define ASSERT_DECISION                                                                                                \
    if ( action_decision == std::nullopt ) {                                                                           \
        LOG(ERROR) << "Expected a decision, but didnt receive one";                                                    \
        throw std::runtime_error("Expected a decision, but didnt receive one");                                        \
    }

#define ASSERT_NO_DECISION                                                                                             \
    if ( action_decision != std::nullopt ) {                                                                           \
        LOG(ERROR) << "Received a decision, but didnt excpect one";                                                    \
        throw std::runtime_error("Received a decision, but didnt excpect one");                                        \
    }

/**
 * @brief can be used like:
 * auto casted_decision = CAST_DECISION(expected_type);
 * @warning THIS WILL THROW
 */
#define TRY_CAST_DECISION(decision_type)                                                                               \
    [](server::base::Behaviour::action_decision_t &action_decision) -> decision_type *                                 \
    {                                                                                                                  \
        ASSERT_DECISION                                                                                                \
        auto *casted_decision = dynamic_cast<(decision_type) *>(action_decision->get());                               \
        if ( !casted_decision ) {                                                                                      \
            LOG(ERROR) << "Decision has wrong type! Expected: " << utils::demangle(typeid(decision_type).name())       \
                       << ", but got: " << utils::demangle(typeid(*action_decision->get()).name());                    \
            throw std::runtime_error("Decision has wrong type");                                                       \
        }                                                                                                              \
        return casted_decision;                                                                                        \
    }(action_decision)

#define BEHAVIOUR_DONE                                                                                                 \
    finished_behaviour = true;                                                                                         \
    return OrderResponse()


        // ================================
        // BEHAVIOUR IMPLEMENTATIONS
        // ================================
        DEFINE_TEMPLATED_BEHAVIOUR(GainCoins, int, coins)
        {
            LOG_CALL;
            ASSERT_NO_DECISION;

            auto &affected_player = game_state.getPlayer(requestor_id);
            affected_player.addTreasure(coins);

            BEHAVIOUR_DONE;
        }

        DEFINE_TEMPLATED_BEHAVIOUR(GainBuys, int, buys)
        {
            LOG_CALL;
            ASSERT_NO_DECISION;

            auto &affected_player = game_state.getPlayer(requestor_id);
            affected_player.addBuys(buys);

            BEHAVIOUR_DONE;
        }

        DEFINE_TEMPLATED_BEHAVIOUR(GainActions, int, actions)
        {
            LOG_CALL;
            ASSERT_NO_DECISION;

            auto &affected_player = game_state.getPlayer(requestor_id);
            affected_player.addActions(actions);

            BEHAVIOUR_DONE;
        }

        DEFINE_TEMPLATED_BEHAVIOUR(DrawCards, int, n_cards)
        {
            LOG_CALL;
            ASSERT_NO_DECISION;

            auto &affected_player = game_state.getPlayer(requestor_id);
            affected_player.draw(n_cards);

            BEHAVIOUR_DONE;
        }

        DEFINE_TEMPLATED_BEHAVIOUR(DrawCardsEnemies, int, n_cards)
        {
            LOG_CALL;
            ASSERT_NO_DECISION;

            auto all_player_ids = game_state.getAllPlayerIDs();
            for ( auto &player_id : all_player_ids ) {
                if ( player_id == game_state.getCurrentPlayerId() ) {
                    continue;
                }
                auto &affected_player = game_state.getPlayer(player_id);
                affected_player.draw(n_cards);
            }

            BEHAVIOUR_DONE;
        }

        DEFINE_BEHAVIOUR(SeaHag)
        {
            LOG_CALL;
            ASSERT_NO_DECISION;

            // ensure play order
            helper::applyAttackToEnemies(game_state,
                                         [&](GameState &game_state, const shared::PlayerBase::id_t &enemy_id)
                                         {
                                             auto &affected_enemy = game_state.getPlayer(enemy_id);
                                             affected_enemy.move<shared::DRAW_PILE_TOP, shared::DISCARD_PILE>(1);
                                             if ( game_state.getBoard()->has("Curse") ) {
                                                 game_state.getBoard()->tryTake("Curse");
                                                 affected_enemy.add<shared::DRAW_PILE_TOP>("Curse");
                                             }
                                         });

            BEHAVIOUR_DONE;
        }

        DEFINE_BEHAVIOUR(Moneylender)
        {
            LOG_CALL;
            ASSERT_NO_DECISION;

            auto &affected_player = game_state.getPlayer(requestor_id);
            if ( affected_player.hasCard<shared::HAND>("Copper") ) {
                // Discard the copper
                affected_player.move<shared::HAND, shared::TRASH>("Copper");
                affected_player.addTreasure(3);
            }

            BEHAVIOUR_DONE;
        }

        DEFINE_BEHAVIOUR(TreasureTrove)
        {
            LOG_CALL;
            ASSERT_NO_DECISION;

            auto &affected_player = game_state.getCurrentPlayer();
            auto &board = *game_state.getBoard();
            if ( board.has("Copper") )
            {
                board.tryTake("Copper");
                affected_player.gain("Copper");
            }
            if ( board.has("Gold") )
            {
                board.tryTake("Gold");
                affected_player.gain("Gold");
            }
            BEHAVIOUR_DONE;
        }

        DEFINE_BEHAVIOUR(Poacher)
        {
            LOG_CALL;

            if ( !action_decision.has_value() ) {
                auto board = game_state.getBoard();
                auto cards_to_discard = board->getEmptyPilesCount();
                if ( cards_to_discard != 0) {
                    return {game_state.getCurrentPlayerId(),
                            std::make_unique<shared::ChooseFromHandOrder>(cards_to_discard, cards_to_discard,
                                                                          shared::ChooseFromOrder::AllowedChoice::DISCARD)};
                }
            }

            auto board = game_state.getBoard();
            auto cards_to_discard = board->getEmptyPilesCount();

            if ( cards_to_discard != 0 ) {
                auto decision =
                        helper::validateResponse(game_state, requestor_id, action_decision.value(), cards_to_discard, cards_to_discard);

                if ( decision.cards.size() == 0 ) {
                    BEHAVIOUR_DONE;
                }

                auto &affected_player = game_state.getCurrentPlayer();
                for ( const auto &card_id : decision.cards ) {
                    affected_player.move<shared::HAND, shared::DISCARD_PILE>(card_id);
                }
            }
            BEHAVIOUR_DONE;
        }

        DEFINE_BEHAVIOUR(TreasureMap)
        {
            LOG_CALL;
            ASSERT_NO_DECISION;

            auto &affected_player = game_state.getPlayer(requestor_id);
            auto &board = *game_state.getBoard();
            if ( affected_player.hasCard<shared::HAND>("Treasure_Map") ) {
                affected_player.move<shared::HAND, shared::TRASH>("Treasure_Map");
                // Currently, ServerPlayer::move does not delete the card from
                // the hand, so we have to do it manually
                board.trashCard("Treasure_Map");
                if ( !board.removeFromPlayedCards("Treasure_Map") ) {
                    // We played a treasure map, so it should be in the played cards now
                    LOG(ERROR) << "Treasure_Map not found in played cards";
                    throw std::runtime_error("Treasure_Map not found in played cards");
                }
                board.trashCard("Treasure_Map");
                for ( int i = 0; i < 4; i++ ) {
                    if ( board.has("Gold") ) {
                        board.tryTake("Gold");
                        affected_player.add<shared::DRAW_PILE_TOP>("Gold");
                    }
                }
            }

            BEHAVIOUR_DONE;
        }


#define TODO_IMPLEMENT_ME                                                                                              \
    SUPPRESS_UNUSED_VAR_WARNING(game_state);                                                                           \
    SUPPRESS_UNUSED_VAR_WARNING(action_decision);                                                                      \
    SUPPRESS_UNUSED_VAR_WARNING(requestor_id);                                                                         \
    LOG(ERROR) << "BEHAVIOUR " << CLASS_NAME << " IS NOT IMPLEMENTED YET";                                             \
    throw std::runtime_error("not implemented");                                                                       \
    return OrderResponse()

        // ================================
        // PLACEHOLDER BEHAVIOUR
        // ================================
        DEFINE_BEHAVIOUR(NOT_IMPLEMENTED_YET) { TODO_IMPLEMENT_ME; }

        // ================================
        // Behaviours
        // ================================

        DEFINE_BEHAVIOUR(CurseEnemy)
        {
            /**
             * curses are applied clockwise, starting from the player to the right of the cur player
             */

            LOG_CALL;
            ASSERT_NO_DECISION;

            helper::applyAttackToEnemies(game_state,
                                         [&](GameState &game_state, const shared::PlayerBase::id_t &enemy_id)
                                         {
                                             if ( game_state.getBoard()->has("Curse") ) {
                                                 game_state.getBoard()->tryTake("Curse");
                                                 game_state.getPlayer(enemy_id).gain("Curse");
                                             }
                                         });

            BEHAVIOUR_DONE;
        }

        DEFINE_TEMPLATED_BEHAVIOUR(GainCardMaxCostHand, int, max_cost)
        {
            LOG_CALL;

            const bool has_action_decision = action_decision.has_value();

            if ( !has_action_decision ) {
                // choose any card
                return {requestor_id, std::make_unique<shared::GainFromBoardOrder>(max_cost)};
            }

            auto *gain_decision = dynamic_cast<shared::GainFromBoardDecision *>(action_decision.value().get());
            if ( gain_decision == nullptr ) {
                const auto *decision_ptr = action_decision.value().get();
                if ( decision_ptr != nullptr ) {
                    LOG(ERROR) << FUNC_NAME
                               << " got a wrong decision type! expected: shared::GainFromBoardDecision, got: "
                               << typeid(*decision_ptr).name();
                } else {
                    LOG(ERROR) << FUNC_NAME << " got a null pointer for action decision!";
                }
                throw std::runtime_error("Decision type is not allowed!");
            }

            const auto chosen_card_id = gain_decision->chosen_card;
            game_state.tryGain<shared::HAND>(requestor_id, chosen_card_id);

            BEHAVIOUR_DONE;
        }

        DEFINE_TEMPLATED_BEHAVIOUR(GainCardMaxCostDiscard, int, max_cost)
        {
            LOG_CALL;

            const bool has_action_decision = action_decision.has_value();
            const auto cur_player_id = game_state.getCurrentPlayerId();

            if (!has_action_decision) {
                // choose any card
                return { cur_player_id, std::make_unique<shared::GainFromBoardOrder>(max_cost) };
            }

            auto* gain_decision = dynamic_cast<shared::GainFromBoardDecision*>(action_decision.value().get());
            if (gain_decision == nullptr) {
                const auto* decision_ptr = action_decision.value().get();
                if (decision_ptr != nullptr) {
                    LOG(ERROR) << FUNC_NAME
                        << " got a wrong decision type! expected: shared::GainFromBoardDecision, got: "
                        << typeid(*decision_ptr).name();
                }
                else {
                    LOG(ERROR) << FUNC_NAME << " got a null pointer for action decision!";
                }
                throw std::runtime_error("Decision type is not allowed!");
            }

            const auto chosen_card_id = gain_decision->chosen_card;
            game_state.tryGain<shared::DISCARD_PILE>(cur_player_id, chosen_card_id);

            BEHAVIOUR_DONE;
        }

        DEFINE_BEHAVIOUR(CardToDrawPile)
        {
            LOG_CALL;

            if ( !action_decision.has_value() ) {
                // send out gain card oder
                return {game_state.getCurrentPlayerId(),
                        std::make_unique<shared::ChooseFromHandOrder>(
                                1, 1, shared::ChooseFromOrder::AllowedChoice::DRAW_PILE)};
            }

            auto deck_choice = helper::validateResponse(game_state, requestor_id, action_decision.value(), 1, 1);

            const auto move_card_id = deck_choice.cards.at(0);
            game_state.getCurrentPlayer().move<shared::CardAccess::HAND, shared::CardAccess::DRAW_PILE_TOP>(
                    move_card_id);

            BEHAVIOUR_DONE;
        }

        DEFINE_BEHAVIOUR(Mine)
        {
            LOG_CALL;

            const bool has_action_decision = action_decision.has_value();
            const auto player_id = game_state.getCurrentPlayerId();
            auto &affected_player = game_state.getPlayer(requestor_id);

            if ( !has_action_decision ) {
                if ( !affected_player.hasType<shared::CardAccess::HAND>(shared::CardType::TREASURE) ) {
                    LOG(INFO) << "Player: "
                              << " has no treasure in hand, returning";
                    BEHAVIOUR_DONE;
                }

                return {requestor_id,
                        std::make_unique<shared::ChooseFromHandOrder>(
                                1, 1, shared::ChooseFromOrder::AllowedChoice::DISCARD, shared::CardType::TREASURE)};
            }

            if ( dynamic_cast<shared::DeckChoiceDecision *>(action_decision.value().get()) != nullptr ) {
                auto trash_decision = helper::validateResponse(game_state, requestor_id, action_decision.value(), 1, 1,
                                                               shared::CardType::TREASURE);

                const auto card_id = trash_decision.cards.at(0);
                affected_player.move<shared::CardAccess::HAND, shared::CardAccess::TRASH>(card_id);
                const auto max_cost = shared::CardFactory::getCost(card_id) + 3;
                return {player_id, std::make_unique<shared::GainFromBoardOrder>(max_cost, shared::CardType::TREASURE)};

            } else if ( auto *card_choice =
                                dynamic_cast<shared::GainFromBoardDecision *>(action_decision.value().get()) ) {
                auto card_id = card_choice->chosen_card;

                if ( !shared::CardFactory::isTreasure(card_id) ) {
                    LOG(ERROR) << FUNC_NAME << "Player: " << requestor_id << " tried to select card: " << card_id
                               << " which does not have type Treasure";
                    throw std::runtime_error("CardType not allowed!");
                }

                game_state.tryGain<shared::HAND>(player_id, card_id);
            }

            BEHAVIOUR_DONE;
        }

        DEFINE_BEHAVIOUR(Remodel)
        {
            LOG_CALL;

            const bool has_action_decision = action_decision.has_value();
            const auto player_id = game_state.getCurrentPlayerId();
            auto &player = game_state.getCurrentPlayer();

            if ( !has_action_decision ) {
                return {player_id,
                        std::make_unique<shared::ChooseFromHandOrder>(1, 1,
                                                                      shared::ChooseFromOrder::AllowedChoice::TRASH)};
            }

            if ( dynamic_cast<shared::DeckChoiceDecision *>(action_decision.value().get()) != nullptr ) {
                auto trash_decision = helper::validateResponse(game_state, requestor_id, action_decision.value(), 1, 1);

                const auto card_id = trash_decision.cards.at(0);
                player.move<shared::CardAccess::HAND, shared::CardAccess::TRASH>(card_id);
                game_state.getBoard()->trashCard("card_id");
                const auto max_cost = shared::CardFactory::getCost(card_id) + 2;
                return {player_id, std::make_unique<shared::GainFromBoardOrder>(max_cost)};

            } else if ( auto *card_choice =
                                dynamic_cast<shared::GainFromBoardDecision *>(action_decision.value().get()) ) {
                auto card_id = card_choice->chosen_card;
                game_state.tryGain<shared::DISCARD_PILE>(player_id, card_id);
            }

            BEHAVIOUR_DONE;
        }


        DEFINE_TEMPLATED_BEHAVIOUR(TrashCards, int, num_cards)
        {
            LOG_CALL;
            if ( !action_decision.has_value() ) {
                return {game_state.getCurrentPlayerId(),
                        std::make_unique<shared::ChooseFromHandOrder>(0, 4,
                                                                      shared::ChooseFromOrder::AllowedChoice::TRASH)};
            }

            auto trash_decision =
                    helper::validateResponse(game_state, requestor_id, action_decision.value(), 0, num_cards);

            auto &affected_player = game_state.getPlayer(requestor_id);
            for ( const auto &card_id : trash_decision.cards ) {
                affected_player.move<shared::CardAccess::HAND, shared::CardAccess::TRASH>(card_id);
            }

            BEHAVIOUR_DONE;
        }

        DEFINE_BEHAVIOUR(DiscardAndRedrawAnyAmount)
        {
            LOG_CALL;

            const auto max_discard_amount = game_state.getCurrentPlayer().get<shared::CardAccess::HAND>().size();
            if ( !action_decision.has_value() ) {
                return {requestor_id,
                        std::make_unique<shared::ChooseFromHandOrder>(0, max_discard_amount,
                                                                      shared::ChooseFromOrder::AllowedChoice::TRASH)};
            }

            auto discard_decision =
                    helper::validateResponse(game_state, requestor_id, action_decision.value(), 0, max_discard_amount);

            // stop behaviour if no cards are selected
            // otherwise draw(0) would draw the entire draw pile
            if ( discard_decision.cards.empty() ) {
                BEHAVIOUR_DONE;
            }

            auto &affected_player = game_state.getPlayer(requestor_id);
            affected_player.draw(discard_decision.cards.size());
            for ( const auto &card_id : discard_decision.cards ) {
                affected_player.move<shared::CardAccess::HAND, shared::CardAccess::DISCARD_PILE>(card_id);
            }

            BEHAVIOUR_DONE;
        }

        class MilitiaAttack : public server::base::Behaviour
        {
            std::unordered_map<shared::PlayerBase::id_t, unsigned int> expect_response;

        public:
            inline ret_t apply(server::GameState &state, const shared::PlayerBase::id_t &requestor_id,
                               server::base::Behaviour::action_decision_t action_decision = std::nullopt) override;
        };
        inline server::base::Behaviour::ret_t
        MilitiaAttack::apply(server::GameState &game_state, const shared::PlayerBase::id_t &requestor_id,
                             server::base::Behaviour::action_decision_t action_decision)
        {
            LOG_CALL;
            // check if anything has to be done
            if ( !action_decision.has_value() ) {
                auto order = helper::sendAttackToEnemies(
                        game_state,
                        [this](GameState &game_state, const shared::PlayerBase::id_t &enemy_id)
                        {
                            const auto &enemy = game_state.getPlayer(enemy_id);
                            const auto hand_size = enemy.get<shared::HAND>().size();


                            if ( hand_size <= 3 || enemy.canBlock() ) {
                                // no order for this player
                                return std::unique_ptr<shared::ChooseFromHandOrder>(nullptr);
                            }

                            const unsigned int n_cards_to_discard = hand_size - 3;

                            this->expect_response.emplace(enemy_id, n_cards_to_discard);

                            return std::make_unique<shared::ChooseFromHandOrder>(
                                    n_cards_to_discard, n_cards_to_discard,
                                    shared::ChooseFromOrder::AllowedChoice::TRASH);
                        });

                bool has_no_orders = std::all_of(order.begin(), order.end(),
                                                 [](const auto &map_entry) { return map_entry.second == nullptr; });

                if ( has_no_orders ) {
                    BEHAVIOUR_DONE;
                } else {
                    return order;
                }
            }

            auto enemy_iter = this->expect_response.find(requestor_id);
            if ( enemy_iter == this->expect_response.end() ) {
                LOG(WARN) << "Not expecting a response from enemy: " << requestor_id;
                throw exception::NotYourTurn();
            }

            const auto n_cards_to_discard = this->expect_response.at(requestor_id);
            auto decision = helper::validateResponse(game_state, requestor_id, action_decision.value(),
                                                     n_cards_to_discard, n_cards_to_discard);

            auto &affected_enemy = game_state.getPlayer(requestor_id);
            for ( const auto &card_id : decision.cards ) {
                affected_enemy.move<shared::HAND, shared::DISCARD_PILE>(card_id);
            }

            this->expect_response.erase(enemy_iter);

            if ( !this->expect_response.empty() ) {
                return OrderResponse();
            }

            BEHAVIOUR_DONE;
        }

// ================================
// UNDEF MACROS
// ================================
#undef DEFINE_BEHAVIOUR
#undef BEHAVIOUR_DONE
#undef DEFINE_TEMPLATED_BEHAVIOUR
#undef TRY_CAST_DECISION
#undef LOG_CALL
#undef SUPPRESS_UNUSED_VAR_WARNING
#undef ASSERT_DECISION
#undef ASSERT_NO_DECISION
    } // namespace behaviour
} // namespace server
