
#include <shared/game/game_state/player_base.h>
#include <shared/utils/json.h>

namespace shared
{
    PlayerBase::PlayerBase(id_t player_id) :
        player_id(player_id), victory_points(0), actions(1), buys(1), treasure(0), draw_pile_size(0)
    {}

    PlayerBase::PlayerBase(const PlayerBase &other) = default;

    PlayerBase &PlayerBase::operator=(const PlayerBase &other)
    {
        if ( this != &other ) {
            player_id = other.player_id;
            victory_points = other.victory_points;
            actions = other.actions;
            buys = other.buys;
            treasure = other.treasure;
            current_card = other.current_card;
            discard_pile = other.discard_pile;
            draw_pile_size = other.draw_pile_size;
        }
        return *this;
    }

    PlayerBase::PlayerBase(PlayerBase &&other) noexcept :
        player_id(std::move(other.player_id)), victory_points(other.victory_points), actions(other.actions),
        buys(other.buys), treasure(other.treasure), current_card(std::move(other.current_card)),
        discard_pile(std::move(other.discard_pile)), draw_pile_size(other.draw_pile_size)
    {}

    PlayerBase &PlayerBase::operator=(PlayerBase &&other) noexcept
    {
        if ( this != &other ) {
            victory_points = other.victory_points;
            actions = other.actions;
            buys = other.buys;
            treasure = other.treasure;
            current_card = std::move(other.current_card);
            discard_pile = std::move(other.discard_pile);
            draw_pile_size = other.draw_pile_size;
        }
        return *this;
    }

    void PlayerBase::decActions()
    {
        if ( actions == 0 ) {
            return;
        }

        actions--;
    }

    void PlayerBase::decBuys()
    {
        if ( buys == 0 ) {
            return;
        }

        buys--;
    }

    void PlayerBase::decTreasure(const unsigned int dec_amount)
    {
        if ( treasure < dec_amount ) {
            return;
        }

        treasure -= dec_amount;
    }

    bool PlayerBase::operator==(const PlayerBase &other) const
    {
        return (player_id == other.player_id) && (victory_points == other.victory_points) &&
                (actions == other.actions) && (buys == other.buys) && (treasure == other.treasure) &&
                (current_card == other.current_card) && (discard_pile == other.discard_pile) &&
                (draw_pile_size == other.draw_pile_size) && (played_cards == other.played_cards);
    }

    rapidjson::Document PlayerBase::toJson() const
    {
        rapidjson::Document doc;
        doc.SetObject();

        ADD_STRING_MEMBER(this->player_id.c_str(), player_id);
        ADD_UINT_MEMBER(this->victory_points, victory_points);
        ADD_UINT_MEMBER(this->actions, actions);
        ADD_UINT_MEMBER(this->buys, buys);
        ADD_UINT_MEMBER(this->treasure, treasure);
        ADD_STRING_MEMBER(this->current_card.c_str(), current_card);
        ADD_ARRAY_OF_STRINGS_MEMBER(this->discard_pile, discard_pile);
        ADD_UINT_MEMBER(this->draw_pile_size, draw_pile_size);
        ADD_ARRAY_OF_STRINGS_MEMBER(this->played_cards, played_cards);

        return doc;
    }

    std::unique_ptr<PlayerBase> PlayerBase::fromJson(const rapidjson::Value &json)
    {
        PlayerBase::id_t player_id;
        GET_STRING_MEMBER(player_id, json, "player_id");
        std::unique_ptr<PlayerBase> player(new PlayerBase(player_id));
        GET_UINT_MEMBER(player->victory_points, json, "victory_points");
        GET_UINT_MEMBER(player->actions, json, "actions");
        GET_UINT_MEMBER(player->buys, json, "buys");
        GET_UINT_MEMBER(player->treasure, json, "treasure");
        GET_STRING_MEMBER(player->current_card, json, "current_card");
        GET_STRING_ARRAY_MEMBER(player->discard_pile, json, "discard_pile");
        GET_UINT_MEMBER(player->draw_pile_size, json, "draw_pile_size");
        GET_STRING_ARRAY_MEMBER(player->played_cards, json, "played_cards");
        return player;
    }

} // namespace shared
