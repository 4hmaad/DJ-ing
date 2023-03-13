
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioSource {
public:

    DJAudioPlayer(juce::AudioFormatManager &_formatManager);

    ~DJAudioPlayer();

    /** implement AudioSource */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /** implement AudioSource */
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;

    /** implement AudioSource */
    void releaseResources() override;

    /** load an audio file from a URL */
    void loadURL(juce::URL audioURL);

    /** set the gain of the audio player */
    void setGain(double gain);

    /** set the speed of the audio player */
    void setSpeed(double ratio);

    /** set the position of the playhead */
    void setPosition(double posInSecs);

    /** set the position of the playhead relative to the length of the audio file */
    void setPositionRelative(double pos);

    /** start playing the audio file */
    void start();

    /** stop playing the audio file */
    void stop();

    /** pause playing the audio file */
    bool isLoaded();

    /** check if the audio file is playing */
    bool isPlaying();

    /** check if the audio file has finished playing */
    bool isFinished();

    /** check if the audio file is looping */
    void setLooping(bool shouldLoop);

    /** get the relative position of the playhead */
    double getPositionRelative();

private:
    juce::AudioFormatManager &formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};

};




