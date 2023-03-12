/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager &formatManagerToUse,
                                 juce::AudioThumbnailCache &cacheToUse) :
        audioThumb(1000, formatManagerToUse, cacheToUse),
        fileLoaded(false),
        position(0) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay() {
}

void WaveformDisplay::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    if (fileLoaded) {
        g.setColour(juce::Colour::fromRGB(115, 163, 248));
        audioThumb.drawChannel(g,
                               getLocalBounds(),
                               0,
                               audioThumb.getTotalLength(),
                               0,
                               1.0f
        );
        g.setColour(juce::Colours::red);
        g.drawRect(position * getWidth(), 0, 8, getHeight(), 4);
    } else {
        g.setColour(juce::Colours::white);
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
                   juce::Justification::centred, true);
    }
}

void WaveformDisplay::resized() {}

void WaveformDisplay::loadURL(juce::URL audioURL) {
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded) {
        std::cout << "wfd: loaded! " << std::endl;
        repaint();
    } else {
        std::cout << "wfd: not loaded! " << std::endl;
    }

}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source) {
    std::cout << "wfd: change received! " << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos) {
    if (pos != position) {
        position = pos;
        repaint();
    }
}

void WaveformDisplay::mouseDown(const juce::MouseEvent &event) {
    // Update position based on mouse click position
    setPositionRelative((double) event.x / getWidth());
}




