#pragma once

#include <string>
#include <vector>
#include <wx/sizer.h>
#include <wx/wx.h>

namespace client
{

    /**
     * @brief This class displays the lobby of the game.
     */
    class LobbyPanel : public wxPanel
    {
    public:
        /**
         * @brief Constructor for the lobby panel.
         * @param parent The parent window.
         */
        explicit LobbyPanel(wxWindow *parent);

        /**
         * @brief Refreshes the players in the lobby.
         * @param players The list of players.
         */
        void refreshPlayers(std::vector<std::string> &players);

        /// @brief Adds a player visually to the lobby
        /// This is specifically not doing any logic
        /// @pre playerCount < 4
        /// @post playerCount = playerCount + 1
        void AddPlayer(wxString name);

    private:
        wxGridSizer *NamesSizer;

        unsigned int playerCount;
    };

} // namespace client