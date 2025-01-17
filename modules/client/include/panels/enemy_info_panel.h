#pragma once

#include <shared/game/game_state/reduced_game_state.h>

#include <vector>
#include <wx/wx.h>

namespace client
{
    /**
     * @brief The panel that displays the information about the enemies
     * This includes the number of cards in the deck, hand and discard pile
     */
    class EnemyInfoPanel : public wxPanel
    {
    public:
        EnemyInfoPanel(wxWindow *parent, wxSize size);

        void drawEnemies(const std::vector<std::unique_ptr<reduced::Enemy>> &enemies,
                         const shared::PlayerBase::id_t &active_player);

    private:
    };

} // namespace client
