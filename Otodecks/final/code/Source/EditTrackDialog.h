

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "domain/Track.h"
//==============================================================================

/*
 * EditTrackDialog is a dialog that allows the user to edit the title and album of a track
 * It is a wrapper around the JUCE AlertWindow class
 */
class EditTrackDialog {
public:
    EditTrackDialog(const std::string &title, const std::string &album,
                    std::function<void(std::string &, std::string &)> callback);

private:
    std::unique_ptr<juce::AlertWindow> asyncAlertWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditTrackDialog)
};
