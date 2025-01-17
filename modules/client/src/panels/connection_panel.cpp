#include <panels/connection_panel.h>
#include <wx/gdicmn.h>
#include <wx/wx.h>

#include <dominion.h>
#include <game_controller.h>
#include <uiElements/image_panel.h>

// NOLINTBEGIN(bugprone-suspicious-enum-usage)
namespace client
{

    ConnectionPanel::ConnectionPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY)
    {

        wxColor white = wxColor(255, 255, 255);
        this->SetBackgroundColour(white);

        wxBoxSizer *VerticalLayout = new wxBoxSizer(wxVERTICAL);

        ImagePanel *LogoPanel = new ImagePanel(this, // parent element
                                               "DominionLogo", // asset name
                                               wxBITMAP_TYPE_PNG, // format of image
                                               wxDefaultPosition, // position
                                               wxSize(605, 229), // size
                                               0.0 // rotation
        );

        VerticalLayout->Add(LogoPanel, 0, wxALIGN_CENTER, 10);

        this->_serverAddressField = new InputField(this, // parent element
                                                   "Server address:", // label
                                                   120, // width of label
                                                   "127.0.0.1", // default value (variable from "default.conf")
                                                   240 // width of field
        );
        VerticalLayout->Add(this->_serverAddressField, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);

        this->_serverPortField =
                new InputField(this, // parent element
                               "Server port:", // label
                               120, // width of label
                               wxString::Format("%i", 50505), // default value (variable from "default.conf")
                               240 // width of field
                );
        VerticalLayout->Add(this->_serverPortField, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);

        this->_playerNameField = new InputField(this, // parent element
                                                "Player name:", // label
                                                120, // width of label
                                                "", // default value
                                                240 // width of field
        );
        VerticalLayout->Add(this->_playerNameField, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);

        this->_gameName = new InputField(this, // parent element
                                         "Game name:", // label
                                         120, // width of label
                                         "my dominion", // default value (variable from "default.conf")
                                         240 // width of field
        );
        VerticalLayout->Add(this->_gameName, 0, wxALIGN_CENTER | wxTOP | wxLEFT | wxRIGHT, 10);

        wxButton *createLobbyButton = new wxButton(this, wxID_ANY, "create Lobby", wxDefaultPosition, wxSize(100, 40));
        createLobbyButton->Bind(wxEVT_BUTTON,
                                [](const wxCommandEvent & /*event*/) { wxGetApp().getController().createLobby(); });
        VerticalLayout->Add(createLobbyButton, 0, wxALIGN_RIGHT | wxALL, 10);
        wxButton *joinLobbyButton = new wxButton(this, wxID_ANY, "join Lobby", wxDefaultPosition, wxSize(100, 40));
        joinLobbyButton->Bind(wxEVT_BUTTON,
                              [](const wxCommandEvent & /*event*/) { wxGetApp().getController().joinLobby(); });
        VerticalLayout->Add(joinLobbyButton, 0, wxALIGN_RIGHT | wxALL, 10);

        if ( wxGetApp().isDebugMode() ) {
            wxButton *skipToGameScreenButton =
                    new wxButton(this, wxID_ANY, "Game Screen", wxDefaultPosition, wxSize(100, 40));
            skipToGameScreenButton->Bind(wxEVT_BUTTON,
                                         [](const wxCommandEvent & /*event*/)
                                         { wxGetApp().getController().skipToGamePanel(); });
            VerticalLayout->Add(skipToGameScreenButton, 0, wxALIGN_RIGHT | wxALL, 10);

            wxButton *skipToVictoryScreenButton =
                    new wxButton(this, wxID_ANY, "Victory Screen", wxDefaultPosition, wxSize(100, 40));
            skipToVictoryScreenButton->Bind(wxEVT_BUTTON,
                                            [](const wxCommandEvent & /*event*/)
                                            { wxGetApp().getController().skipToVictoryScreen(); });
            VerticalLayout->Add(skipToVictoryScreenButton, 0, wxALIGN_RIGHT | wxALL, 10);

            wxButton *skipToCardSelection =
                    new wxButton(this, wxID_ANY, "Card Selection", wxDefaultPosition, wxSize(100, 40));
            skipToCardSelection->Bind(wxEVT_BUTTON,
                                      [](const wxCommandEvent & /*event*/)
                                      { wxGetApp().getController().skipToCardSelectionPanel(); });
            VerticalLayout->Add(skipToCardSelection, 0, wxALIGN_RIGHT | wxALL, 10);
        }

        this->SetSizerAndFit(VerticalLayout);
    }


    wxString ConnectionPanel::getServerAddress() { return this->_serverAddressField->getValue(); }


    wxString ConnectionPanel::getServerPort() { return this->_serverPortField->getValue(); }


    wxString ConnectionPanel::getPlayerName() { return this->_playerNameField->getValue(); }

    wxString ConnectionPanel::getGameName() { return this->_gameName->getValue(); }

} // namespace client
// NOLINTEND(bugprone-suspicious-enum-usage)
