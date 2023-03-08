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

    tableComponent.getHeader().addColumn("ID", 1, 50);
    tableComponent.getHeader().addColumn("Title", 2, 150);
    tableComponent.getHeader().addColumn("Album", 3, 150);
    tableComponent.getHeader().addColumn("Artist", 4, 150);
    tableComponent.getHeader().addColumn("Length", 5, 100);
    tableComponent.getHeader().addColumn("Action", 6, 100);

    tableComponent.getHeader().setStretchToFitActive(true);

    // Add some example data
    Track track1 = {"1", "Song 1", "Album 1", "Artist 1", "3:25"};
    Track track2 = {"2", "Song 2", "Album 2", "Artist 2", "4:10"};
    Track track3 = {"3", "Song 3", "Album 3", "Artist 3", "2:50"};
    Track track4 = {"4", "Song 4", "Album 4", "Artist 4", "3:25"};
    Track track5 = {"5", "Song 5", "Album 5", "Artist 5", "4:10"};
    Track track6 = {"6", "Song 6", "Album 6", "Artist 6", "2:50"};
    Track track7 = {"7", "Song 7", "Album 7", "Artist 7", "3:25"};
    Track track8 = {"8", "Song 8", "Album 8", "Artist 8", "4:10"};
    Track track9 = {"9", "Song 9", "Album 9", "Artist 9", "2:50"};
    Track track10 = {"10", "Song 10", "Album 10", "Artist 10", "3:25"};
    Track track11 = {"11", "Song 11", "Album 11", "Artist 11", "4:10"};
    Track track12 = {"12", "Song 12", "Album 12", "Artist 12", "2:50"};
    Track track13 = {"13", "Song 13", "Album 13", "Artist 13", "3:25"};
    Track track14 = {"14", "Song 14", "Album 14", "Artist 14", "4:10"};
    Track track15 = {"15", "Song 15", "Album 15", "Artist 15", "2:50"};
    Track track16 = {"16", "Song 16", "Album 16", "Artist 16", "3:25"};
    Track track17 = {"17", "Song 17", "Album 17", "Artist 17", "4:10"};
    Track track18 = {"18", "Song 18", "Album 18", "Artist 18", "2:50"};

    trackList = {track1, track2, track3};

    tableComponent.setModel(this);
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
    return trackList.size();
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
    auto trackTitle = trackList.at(rowNumber);
    std::string cellData;
    switch (columnId) {
        case ColumnIds::idColumnId:
            cellData = trackTitle.id;
            break;
        case ColumnIds::titleColumnId:
            cellData = trackTitle.title;
            break;
        case ColumnIds::albumColumnId:
            cellData = trackTitle.album;
            break;
        case ColumnIds::artistColumnId:
            cellData = trackTitle.artist;
            break;
        case ColumnIds::lengthColumnId:
            cellData = trackTitle.length;
            break;
        default:
            break;
    }

    g.setColour(juce::Colours::black);
    g.setFont(14.0f);
    g.drawText(cellData, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

//void PlaylistComponent::cellClicked(int rowNumber, int columnId, const juce::MouseEvent &) {
//    juce::Logger::writeToLog("[cellClicked] row: " + juce::String(rowNumber) + ", column: " + juce::String(columnId));
//    trackList.erase(trackList.begin() + rowNumber);
//    tableComponent.updateContent();
//}

juce::Component *PlaylistComponent::refreshComponentForCell(
        int rowNumber,
        int columnId,
        bool isRowSelected,
        Component *existingComponentToUpdate) {

    // if the column is the action column, create and return the action component
    if (columnId == ColumnIds::actionColumnId) {
        juce::Component *actionComponent = dynamic_cast<juce::Component *>(existingComponentToUpdate);
        // return actionComponent if it already exists
        if (actionComponent != nullptr) {
            return actionComponent;
        }

        actionComponent = new juce::Component();
//        actionComponent->setSize(80, 20);

        // create the "Edit" button
        juce::TextButton *editButton = new juce::TextButton("Edit");
        editButton->addListener(this);
        editButton->setColour(juce::TextButton::buttonColourId, juce::Colours::lightblue);
        editButton->setBounds(0, 0, 40, 20);

        // create the "Delete" button
        juce::TextButton *deleteButton = new juce::TextButton("Delete");
        deleteButton->addListener(this);
        deleteButton->setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        deleteButton->setBounds(40, 0, 40, 20);

        // add the buttons to the action component
        actionComponent->addAndMakeVisible(deleteButton);
        actionComponent->addAndMakeVisible(editButton);

        return actionComponent;
    }

    return existingComponentToUpdate;
}

////    if (columnId == ColumnIds::actionColumnId) {
////        if (existingComponentToUpdate == nullptr) {
////            juce::TextButton *btn = new juce::TextButton("play");
////            btn->addListener(this);
////            existingComponentToUpdate = btn;
////        }
////    }
//    return existingComponentToUpdate;
//}

void PlaylistComponent::buttonClicked(juce::Button *button) {
    juce::Logger::writeToLog("plc -> buttonClicked:" + button->getName());
}