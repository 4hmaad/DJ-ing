
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class EditTrackDialog {
public:
    EditTrackDialog(const std::string &title, const std::string &album,
                    std::function<void(std::string &, std::string &)> callback);

private:
    std::unique_ptr<juce::AlertWindow> asyncAlertWindow;
    
};