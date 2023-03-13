

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::DragAndDropTarget,
                public juce::Timer {
public:
    DeckGUI(DJAudioPlayer *player,
            juce::AudioFormatManager &formatManagerToUse,
            juce::AudioThumbnailCache &cacheToUse);

    ~DeckGUI();

    void paint(juce::Graphics &) override;

    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(juce::Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged(juce::Slider *slider) override;

    /** implement FileDragAndDropTarget */
    bool isInterestedInFileDrag(const juce::StringArray &files) override;

    /** implement FileDragAndDropTarget */
    void filesDropped(const juce::StringArray &files, int x, int y) override;

    /** implement DragAndDropTarget */
    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;

    /** implement DragAndDropTarget */
    void itemDropped(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;

    /** This is called every few ms to update the GUI */
    void timerCallback() override;

private:
    // the play/stop button default text is "Play"
    juce::TextButton playStopButton{"Play"};

    juce::ToggleButton loopButton{"LOOP"};

    juce::Slider volSlider;
    juce::Slider speedSlider;

    juce::Label volSliderLabel;
    juce::Label speedSliderLabel;

    WaveformDisplay waveformDisplay;

    DJAudioPlayer *player;

    /* This is a helper method that is used to play the track and update the GUI */
    void playTrack(juce::URL trackURL);

    /* This is a helper method that is used to update the play/stop button text and other relevant elements */
    void updatePlayStopButton();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
