
#pragma once

#include <shared/game/game_state/board_base.h>
#include <shared/game/game_state/game_phase.h>

#include <wx/wx.h>
#include "../uiElements/pile_panel.h"

namespace client
{
    /**
     * @brief The panel that draws the board
     * This specifically means the piles of cards and if necessary
     * the buttons to buy cards
     *
     */
    class BoardPanel : public wxPanel
    {
    public:
        /**
         * @brief Construct a new Board Panel object
         *
         * @param parent
         * @param size
         */
        BoardPanel(wxWindow *parent, wxSize size);

        /**
         * @brief Draw the board
         *
         * @param Board
         */
        void drawBoard(std::shared_ptr<shared::Board> board, bool is_active, unsigned int treasure,
                       shared::GamePhase phase);

    private:
        /**
         * @brief Make a pile buyable
         * by making it clickable and attaching an Event handler that sends a buy request
         */
        void makeBuyable(PilePanel *pile);

        std::shared_ptr<shared::Board> board_;
    };

} // namespace client
