/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 7 Mar 2023 12:45:53am
    Author:  Ahmad Nawaz Khan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent() {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(tableComponent);

    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("", 2, 200);
    tableComponent.setModel(this);

    trackTitles = {"Track 1", "Track 2", "Track 3"};
}

PlaylistComponent::~PlaylistComponent() {
}

void PlaylistComponent::paint(juce::Graphics &g) {
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(),
               juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized() {
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows() {
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics &g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::orange);
    } else {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics &g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected) {
    auto trackTitle = trackTitles.at(rowNumber);
    g.drawText(trackTitle,
               2, 0,
               width - 4, height,
               juce::Justification::centredLeft,
               true);
}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const juce::MouseEvent &) {
    juce::Logger::writeToLog("[cellClicked] row: " + juce::String(rowNumber) + ", column: " + juce::String(columnId));
    trackTitles.erase(trackTitles.begin() + rowNumber);
    tableComponent.updateContent();
}

juce::Component *PlaylistComponent::refreshComponentForCell(
        int rowNumber,
        int columnId,
        bool isRowSelected,
        Component *existingComponentToUpdate) {
    if (columnId == 2) {
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton *btn = new juce::TextButton("play");
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button *button) {
    juce::Logger::writeToLog("[buttonClicked] " + button->getName());
}