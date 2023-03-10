#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::FileDragAndDropTarget,
                          public juce::DragAndDropContainer {
public:
    PlaylistComponent(DJAudioPlayer *player,
                      juce::AudioFormatManager &formatManagerToUse);

    ~PlaylistComponent() override = default;

    void paint(juce::Graphics &) override;

    void resized() override;

    DJAudioPlayer *player;
    juce::AudioFormatManager &formatManager;

private:

    struct Track {
        std::string id;
        std::string title;
        std::string album;
        float duration;
        juce::URL fileURL;
    };

    std::vector<Track> trackList;
    juce::TableListBox tableComponent;

    // Enum for the columns
    enum ColumnIds {
        idColumnId = 1,
        titleColumnId = 2,
        albumColumnId = 3,
        durationColumnId = 5,
        actionColumnId = 6
    };

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
    juce::Component *refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
                                             juce::Component *existingComponentToUpdate) override;

    juce::var getDragSourceDescription(const juce::SparseSet<int> &currentlySelectedRows) override;

    void buttonClicked(juce::Button *button) override;

    bool isInterestedInFileDrag(const juce::StringArray &files) override;

    void filesDropped(const juce::StringArray &files, int x, int y) override;

//    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails);

    Track loadTrack(const juce::File &file);

    bool trackExists(const juce::File &file);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
