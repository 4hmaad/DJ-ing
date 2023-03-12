#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "Playlist.h"

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::FileDragAndDropTarget,
                          public juce::DragAndDropContainer,
                          public juce::TextEditor::Listener {
public:
    PlaylistComponent(DJAudioPlayer *player,
                      juce::AudioFormatManager &formatManagerToUse);

    ~PlaylistComponent();

    void paint(juce::Graphics &) override;

    void resized() override;

    DJAudioPlayer *player;
    juce::AudioFormatManager &formatManager;

private:

    // UI elements
    juce::TextEditor searchBox;
    juce::TableListBox tableComponent;
    juce::TextButton clearPlaylist{"Clear"};

    // Listeners for UI elements
    void buttonClicked(juce::Button *button) override;

    void textEditorTextChanged(juce::TextEditor &editor) override;

    // TableListBoxModel methods
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

    // TableListBoxModel methods
    juce::Component *refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected,
                                             juce::Component *existingComponentToUpdate) override;

    juce::var getDragSourceDescription(const juce::SparseSet<int> &currentlySelectedRows) override;

    // FileDragAndDropTarget methods
    bool isInterestedInFileDrag(const juce::StringArray &files) override;

    void filesDropped(const juce::StringArray &files, int x, int y) override;

    // Constants
    enum ColumnIds {
        idColumnId = 1,
        titleColumnId = 2,
        albumColumnId = 3,
        durationColumnId = 5,
        actionColumnId = 6
    };

    Playlist playlist;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
