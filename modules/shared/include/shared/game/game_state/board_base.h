
#pragma once

#include <set>
#include <vector>

#include <shared/game/cards/card_base.h>
#include <shared/game/cards/card_factory.h>
#include <shared/utils/assert.h>

#include <rapidjson/document.h>

namespace shared
{
    namespace board_config
    {
        static constexpr size_t KINGDOM_CARD_COUNT = 10;

        static constexpr size_t TREASURE_COPPER_COUNT = 60;
        static constexpr size_t TREASURE_SILVER_COUNT = 40;
        static constexpr size_t TREASURE_GOLD_COUNT = 30;

        static constexpr size_t VICTORY_CARDS_SMALL_GAME = 8; // for player count == 2
        static constexpr size_t VICTORY_CARDS_LARGE_GAME = 12; // for player count > 2
        static constexpr size_t CURSE_MULTIPLIER = 10; // 10 curse cards per player

        static constexpr size_t MIN_PLAYER_COUNT = 2;
        static constexpr size_t MAX_PLAYER_COUNT = 4;

        static constexpr size_t MAX_NUM_EMPTY_PILES = 3;

        static constexpr size_t getCopperCount(size_t num_players) { return TREASURE_COPPER_COUNT - (7 * num_players); }
        static constexpr size_t getCurseCardCount(size_t num_players) { return CURSE_MULTIPLIER * (num_players - 1); }
        static constexpr size_t getVictoryCardCount(size_t num_players)
        {
            return num_players == 2 ? VICTORY_CARDS_SMALL_GAME : VICTORY_CARDS_LARGE_GAME;
        }

        static constexpr bool validatePlayerCount(size_t num_players)
        {
            return MIN_PLAYER_COUNT <= num_players && num_players <= MAX_PLAYER_COUNT;
        }

    } // namespace board_config

    struct Pile
    {
        shared::CardBase::id_t card_id;
        mutable size_t count; // `mutable` allows modification const contexts

        /**
         * @brief Creates a new card pile with size the given size
         *
         * @param card_id The ID of the kingdom card.
         * @param pile_size Amount of cards in the pile
         */
        Pile(const shared::CardBase::id_t &card_id, size_t pile_size) : card_id(card_id), count(pile_size) {}

        bool operator==(const Pile &other) const;
        bool operator!=(const Pile &other) const;

        rapidjson::Document toJson() const;
        static std::unique_ptr<Pile> fromJson(const rapidjson::Value &json);

        /**
         * @brief Creates a new kingdom card pile with size 10; defined by board_config::KINGDOM_CARD_COUNT
         *
         * @param kingdom_card_id The ID of the kingdom card.
         * @return Pile
         */
        static Pile makeKingdomCard(const shared::CardBase::id_t &kingdom_card_id);

        bool empty() const { return count == 0; }

        struct PileComparator
        {
            using is_transparent = void; // enables heterogeneous lookup

            inline bool operator()(const Pile &a, const Pile &b) const
            {
                return (CardFactory::getCost(a.card_id) < CardFactory::getCost(b.card_id)) ||
                        ((CardFactory::getCost(a.card_id) == CardFactory::getCost(b.card_id)) &&
                         (a.card_id < b.card_id));
            }

            inline bool operator()(const Pile &a, const shared::CardBase::id_t &key) const
            {
                return (CardFactory::getCost(a.card_id) < CardFactory::getCost(key)) ||
                        ((CardFactory::getCost(a.card_id) == CardFactory::getCost(key)) && (a.card_id < key));
            }

            inline bool operator()(const shared::CardBase::id_t &key, const Pile &b) const
            {
                return (CardFactory::getCost(key) < CardFactory::getCost(b.card_id)) ||
                        ((CardFactory::getCost(key) == CardFactory::getCost(b.card_id)) && (key < b.card_id));
            }
        };
    };

    class Board
    {
    public:
        /**
         * @brief We use a shared_ptr instead of a unique_ptr because the reduced board and the server board
         * have exactly the same contents. This way we dont copy the contents each time we create a message.
         */
        using ptr_t = std::shared_ptr<Board>;
        using pile_container_t = std::set<Pile, Pile::PileComparator>;

        /**
         * @brief Constructs a shared_ptr on a ServerBoard for a given number of players and 10 kingdom cards.
         *
         * @param kingdom_cards 10 kingdom cards
         * @param player_count integer in range [2, 4]
         */
        static ptr_t make(const std::vector<shared::CardBase::id_t> &kingdom_cards, size_t player_count);

        bool operator==(const Board &other) const;
        bool operator!=(const Board &other) const;

        rapidjson::Document toJson() const;
        static ptr_t fromJson(const rapidjson::Value &json);

        virtual ~Board() = default;

        // enable move semantics
        Board(Board &&) noexcept = default;
        Board &operator=(Board &&) noexcept = default;

        // disable copy constructor and assignment
        Board(const Board &) = delete;
        Board &operator=(const Board &) = delete;

        bool isGameOver() const;
        size_t getEmptyPilesCount() const;

        pile_container_t &getVictoryCards() { return victory_cards; }
        pile_container_t &getTreasureCards() { return treasure_cards; }
        pile_container_t &getKingdomCards() { return kingdom_cards; }
        Pile &getCurseCardPile() { return curse_card_pile; }
        auto &getPlayedCards() { return played_cards; }


    protected:
        pile_container_t victory_cards;
        pile_container_t treasure_cards;
        pile_container_t kingdom_cards;
        Pile curse_card_pile;
        std::vector<shared::CardBase::id_t> trash;

        std::vector<shared::CardBase::id_t> played_cards;

        /**
         * @brief Constructs a new board. It is assumed that the type is valid, as we have no way of checking it.
         * ServerBoard validates card types.
         *
         * @param kingdom_cards 10 valid kingdom cards
         * @param player_count number of players in [2, 4]
         */
        Board(const std::vector<shared::CardBase::id_t> &kingdom_cards, size_t player_count);

        /**
         * @brief Initialises the treasure cards as follows:
         *
         * - copper_count = board_config::TREASURE_COPPER_COUNT - (7 * player_count)
         *
         * - silver_count = board_config::TREASURE_SILVER_COUNT
         *
         * - gold_count   = board_config::TREASURE_GOLD_COUNT
         *
         * @param player_count
         */
        static pile_container_t initialiseTreasureCards(size_t player_count);

        /**
         * @brief Initialised the victory cards as follows:
         *
         * - if player_count < 3:    board_config::VICTORY_CARDS_SMALL_GAME
         *
         * - if player_count >= 3:   board_config::VICTORY_CARDS_LARGE_GAME
         *
         * @param player_count
         */
        static pile_container_t initialiseVictoryCards(size_t player_count);

        /**
         * @brief Initialises the curse pile as:
         *
         * - curse_count = board_config::CURSE_MULTIPLIER * (player_count - 1)
         *
         * @param player_count
         */
        static Pile initialiseCursePile(size_t player_count);

    private:
        Board(pile_container_t victory_cards, pile_container_t treasure_cards, pile_container_t kingdom_cards,
              Pile curse_card_pile, std::vector<shared::CardBase::id_t> trash,
              std::vector<shared::CardBase::id_t> played_cards) :
            victory_cards(victory_cards),
            treasure_cards(treasure_cards), kingdom_cards(kingdom_cards), curse_card_pile(curse_card_pile),
            trash(trash), played_cards(played_cards)
        {}
    };

} // namespace shared
