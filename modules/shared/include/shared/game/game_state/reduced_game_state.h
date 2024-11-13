#pragma once

#include <vector>

#include <shared/game/game_state/board_base.h>
#include <shared/game/game_state/player_base.h>
namespace shared
{
    class ReducedGameState
    {
    public:
        ReducedGameState() {}
        // Constructor to use on the server side
        ReducedGameState(Board board, ReducedPlayer player, std::vector<ReducedEnemy> enemies,
                         PlayerBase::id_t active_player) :
            board(board), player(player), enemies(enemies), active_player(active_player)
        {}

        Board board;
        ReducedPlayer player;
        std::vector<ReducedEnemy> enemies;
        PlayerBase::id_t active_player;
    };
} // namespace shared