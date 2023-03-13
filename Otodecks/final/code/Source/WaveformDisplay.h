#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class WaveformDisplay : public juce::Component,
                        public juce::ChangeListener {
public:
    WaveformDisplay(juce::AudioFormatManager &formatManagerToUse,
                    juce::AudioThumbnailCache &cacheToUse, DJAudioPlayer *_player);

    ~WaveformDisplay();

    /** implement Component */
    void paint(juce::Graphics &) override;

    /** implement Component */
    void resized() override;

    /** implement ChangeListener */
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    /*
     * load a file into the waveform display
     *
     * @param audioFile the file to load
     */
    void loadURL(juce::URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

    /** implement MouseListener */
    void mouseDown(const juce::MouseEvent &event) override;

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    DJAudioPlayer *player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
