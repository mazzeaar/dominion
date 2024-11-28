#include <panels/victory_screen_panel.h>

namespace client
{
    VictoryScreenPanel::VictoryScreenPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
    {
        this->victory_screen_sizer = new wxBoxSizer(wxVERTICAL);
    }

    void VictoryScreenPanel::drawVictoryScreen(reduced::GameState &gameState)
    {
        // Clear the sizer
        this->victory_screen_sizer->Clear(true);

        // Add the title
        wxStaticText *title = new wxStaticText(this, wxID_ANY, "Victory Screen", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
        title->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        this->victory_screen_sizer->Add(title, 0, wxALIGN_CENTER, 10);

       // Create a vector of pairs with the name of each player and his points
        std::vector<std::pair<std::string, unsigned int>> players;
        players.push_back(std::make_pair(gameState.reduced_player->getId(), gameState.reduced_player->getVictoryPoints()));
        for (auto &enemy : gameState.reduced_enemies)
        {
            players.push_back(std::make_pair(enemy->getId(), enemy->getVictoryPoints()));
        }

        // Sort the players by their points
        std::sort(players.begin(), players.end(), [](const std::pair<std::string, unsigned int> &a, const std::pair<std::string, unsigned int> &b) {
            return a.second > b.second;
        });

        // Add the players to the sizer
        for (auto &player : players)
        {
            wxStaticText *player_text = new wxStaticText(this, wxID_ANY, player.first + ": " + std::to_string(player.second), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
            this->victory_screen_sizer->Add(player_text, 0, wxALIGN_CENTER, 10);
        }

        // Set the sizer
        this->SetSizerAndFit(this->victory_screen_sizer);

    }
} // namespace client