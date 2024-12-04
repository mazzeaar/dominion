
#pragma once

#include <string>

namespace shared
{
    enum class GamePhase
    {
        ACTION_PHASE,
        BUY_PHASE,
        PLAYING_ACTION_CARD,
    };

    std::string gamePhaseToString(GamePhase game_phase);
    GamePhase gamePhaseFromString(const std::string &game_phase);
} // namespace shared