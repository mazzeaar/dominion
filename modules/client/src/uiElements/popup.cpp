#include <uiElements/popup.h>

#include <shared/utils/logger.h>
#include <wx/display.h>
#include <wx/wx.h>

namespace client
{
    void showCardPopup(wxWindow *parent, const std::string &card_id)
    {
        LOG(INFO) << "Showing card preview for card " << card_id;
        // Create a dialog
        wxDialog *dialog = new wxDialog(parent, wxID_ANY, "Card Preview", wxDefaultPosition, wxSize(400, 600));

        // Load the card image
        std::string cardPath = "assets/" + card_id + ".png";
        wxBitmap cardBitmap(cardPath, wxBITMAP_TYPE_PNG);

        // Add the image to the dialog
        wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);
        wxStaticBitmap *cardImage = new wxStaticBitmap(dialog, wxID_ANY, cardBitmap);
        dialogSizer->Add(cardImage, 1, wxALIGN_CENTER | wxALL, 10);

        // Set the sizer and show the dialog
        dialog->SetSizer(dialogSizer);

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
        dialog->Destroy();
    }

} // namespace client