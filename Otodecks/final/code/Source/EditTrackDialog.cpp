
#include "../JuceLibraryCode/JuceHeader.h"
#include "EditTrackDialog.h"

//==============================================================================
EditTrackDialog::EditTrackDialog(const std::string &title, const std::string &album,
                                 std::function<void(std::string &, std::string &)> callback) {

    asyncAlertWindow = std::make_unique<juce::AlertWindow>("Edit Track",
                                                           "",
                                                           juce::MessageBoxIconType::InfoIcon);

    asyncAlertWindow->addTextEditor("title", title, "Title :");
    asyncAlertWindow->addTextEditor("album", album, "Album :");

    asyncAlertWindow->addButton("Save", 1, juce::KeyPress(juce::KeyPress::returnKey, 0, 0));
    asyncAlertWindow->addButton("Cancel", 0, juce::KeyPress(juce::KeyPress::escapeKey, 0, 0));

    asyncAlertWindow->enterModalState(true, juce::ModalCallbackFunction::create([this, callback](int result) {
        asyncAlertWindow->exitModalState(0);
        asyncAlertWindow->setVisible(false);

        std::string titleValue = asyncAlertWindow->getTextEditor("title")->getText().toStdString();
        std::string albumValue = asyncAlertWindow->getTextEditor("album")->getText().toStdString();
        callback(titleValue, albumValue);

        // delete the alert window after the callback is executed
        asyncAlertWindow = nullptr;
    }));
}
