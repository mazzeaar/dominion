#pragma once

#include <string>
#include <vector>

#include <iomanip> // for operator<<
#include <iostream> // for operator<<
#include <memory>

#include <rapidjson/document.h>
#include <shared/game/cards/card_base.h>

namespace shared
{
    enum CardAccess
    {
        DISCARD_PILE = 1,
        HAND = 2,
        DRAW_PILE_TOP = 4,
        DRAW_PILE_BOTTOM = 8,

        PLAYED_CARDS = 16,
        TRASH = 32,
        STAGED_CARDS = 64 // ex: sentry could move here
    };

    std::string toString(CardAccess val);

    class PlayerBase
    {
    public:
        using id_t = std::string;

        PlayerBase(id_t player_id);
        PlayerBase(const PlayerBase &other);

        PlayerBase &operator=(const PlayerBase &other);

        PlayerBase(PlayerBase &&other) noexcept;
        PlayerBase &operator=(PlayerBase &&other) noexcept;

        virtual ~PlayerBase() = default;

        bool operator==(const PlayerBase &other) const;

        id_t getId() const { return player_id; }

        unsigned int getActions() const { return actions; }
        unsigned int getBuys() const { return buys; }
        unsigned int getTreasure() const { return treasure; }
        unsigned int getDrawPileSize() const { return draw_pile_size; }
        unsigned int getDiscardPileSize() const { return discard_pile.size(); }
        CardBase::id_t getTopDiscardCard() const { return discard_pile.empty() ? "" : discard_pile.back(); }
        std::vector<CardBase::id_t> getPlayedCards() const { return played_cards; }

        /**
         * @brief Decrements actions by one, or keeps it at 0.
         */
        void decActions();

        /**
         * @brief Decrements buys by one, or keeps it at 0.
         */
        void decBuys();

        /**
         * @brief Decrements treasure by min(dec_amount, treasure_amount)
         *
         * @param dec_amount
         */
        void decTreasure(const unsigned int dec_amount);

    protected:
        id_t player_id;

        unsigned int actions;
        unsigned int buys;
        unsigned int treasure;

        CardBase::id_t current_card;
        std::vector<CardBase::id_t> discard_pile;
        unsigned int draw_pile_size;

        std::vector<CardBase::id_t> played_cards;

        /**
         * @brief Convert the player to a `rapidjson::Document` JSON object.
         */
        rapidjson::Document toJson() const;
        /**
         * @brief Initialize a player from a `rapidjson::Value` JSON object.
         */
        static std::unique_ptr<PlayerBase> fromJson(const rapidjson::Value &json);
    };

} // namespace shared
