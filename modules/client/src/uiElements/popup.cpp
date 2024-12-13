#include <uiElements/popup.h>

#include <shared/utils/logger.h>
#include <wx/display.h>
#include <wx/filename.h>
#include <wx/wx.h>

namespace client
{
    // NOLINTBEGIN(bugprone-suspicious-enum-usage)
    void showCardPopup(wxWindow *parent, const std::string &card_id)
    {
        LOG(INFO) << "Showing card preview for card " << card_id;
        // Create a dialog
        wxDialog *dialog = new wxDialog(parent, wxID_ANY, "Card Preview", wxDefaultPosition, wxSize(500, 750));

        // Load the card image
        wxString cardPath = wxString("assets") + wxFileName::GetPathSeparator() + wxString(card_id) + ".png";
        wxImage cardImage(cardPath, wxBITMAP_TYPE_PNG);

        wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);
        if ( cardImage.IsOk() ) {
            cardImage = cardImage.Scale(400, 600, wxIMAGE_QUALITY_HIGH);
            wxBitmap cardBitmap(cardImage);

            // Add the image to the dialog

            wxStaticBitmap *cardDisplay = new wxStaticBitmap(dialog, wxID_ANY, cardBitmap);
            dialogSizer->Add(cardDisplay, 1, wxALIGN_CENTER | wxALL, 10);
        }
        // Set the sizer and show the dialog
        dialog->SetSizer(dialogSizer);

        // Bind the escape key to close the dialog
        dialog->Bind(wxEVT_KEY_DOWN,
                     [dialog](wxKeyEvent &event)
                     {
                         if ( event.GetKeyCode() == WXK_ESCAPE ) {
                             dialog->EndModal(wxID_CANCEL); // Close the dialog
                         } else {
                             event.Skip(); // Pass the event to the default handler
                         }
                     });

        // Bind right click to close the dialog
        dialog->Bind(wxEVT_RIGHT_DOWN,
                     [dialog](wxMouseEvent /*&event*/)
                     {
                         dialog->EndModal(wxID_CANCEL); // Close the dialog
                     });

        dialog->SetFocus(); // Ensure the dialog captures keyboard events

        // Manually center the dialog on the primary screen
        wxDisplay display(wxDisplay::GetFromWindow(parent));
        wxRect screenRect = display.GetClientArea(); // Get screen dimensions excluding taskbars

        int screenWidth = screenRect.GetWidth();
        int screenHeight = screenRect.GetHeight();

        int dialogWidth = dialog->GetSize().GetWidth();
        int dialogHeight = dialog->GetSize().GetHeight();

        int posX =
                screenRect.GetX() + (screenWidth - dialogWidth) / 2; // Add screenRect.GetX() for multi-monitor setups
        int posY = screenRect.GetY() + (screenHeight - dialogHeight) / 2;

        dialog->SetPosition(wxPoint(posX, posY));

        // Showing the dialog until it is manually closed
        dialog->ShowModal();
    }
    // NOLINTEND(bugprone-suspicious-enum-usage)
} // namespace client
