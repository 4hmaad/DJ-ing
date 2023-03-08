/*
  ==============================================================================

    PlaylistComponent.h
    Created: 7 Mar 2023 12:45:53am
    Author:  Ahmad Nawaz Khan

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener {
public:
    PlaylistComponent();

    ~PlaylistComponent() override;

    void paint(juce::Graphics &) override;

    void resized() override;

private:

    struct Track {
        std::string id;
        std::string title;
        std::string album;
        std::string artist;
        std::string length;
    };

    std::vector<Track> trackList;
    juce::TableListBox tableComponent;

    // Enum for the columns
    enum ColumnIds {
        idColumnId = 1,
        titleColumnId = 2,
        albumColumnId = 3,
        artistColumnId = 4,
        lengthColumnId = 5,
        actionColumnId = 6
    };

    // buttons vectors
    std::vector<juce::TextButton*> editButtons;
    std::vector<juce::TextButton*> deleteButtons;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics &g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(juce::Graphics &g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

//    virtual void cellClicked(int rowNumber, int columnId, const juce::MouseEvent &) override;
//
    juce::Component *refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
                                             juce::Component *existingComponentToUpdate) override;

    void buttonClicked(juce::Button *button) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
