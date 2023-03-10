#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer *_player,
                                     juce::AudioFormatManager &formatManagerToUse) : player(_player),
                                                                                     formatManager(formatManagerToUse) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(tableComponent);

    tableComponent.getHeader().addColumn("#", ColumnIds::idColumnId, 15);
    tableComponent.getHeader().addColumn("TITLE", ColumnIds::titleColumnId, 300);
    tableComponent.getHeader().addColumn("ALBUM", ColumnIds::albumColumnId, 100);
    tableComponent.getHeader().addColumn("DURATION", ColumnIds::durationColumnId, 60);
    tableComponent.getHeader().addColumn("", ColumnIds::actionColumnId, 100);
    tableComponent.getHeader().setStretchToFitActive(true);

    tableComponent.setModel(this);
}

void PlaylistComponent::paint(juce::Graphics &g) {}

void PlaylistComponent::resized() {
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows() {
    return (int) trackList.size();
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

    // converts duration (which is duration in seconds) to the following format: mm:ss
    int minutes = (int) trackTitle.duration / 60;
    int seconds = (int) trackTitle.duration / 3600;
    std::string duration = std::to_string(minutes) + ":" + std::to_string(seconds);

    std::map<int, std::string> columnData = {
            {ColumnIds::idColumnId,       trackTitle.id},
            {ColumnIds::titleColumnId,    trackTitle.title},
            {ColumnIds::albumColumnId,    trackTitle.album},
            {ColumnIds::durationColumnId, duration}
    };
    std::string cellColumnContent = columnData[columnId];

    g.setColour(juce::Colours::black);
    g.setFont(14.0f);
    g.drawText(cellColumnContent, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

juce::Component *PlaylistComponent::refreshComponentForCell(
        int rowNumber,
        int columnId,
        bool isRowSelected,
        Component *existingComponentToUpdate) {

    // if the column is the action column, create and return the action component
    if (columnId == ColumnIds::actionColumnId) {
        auto *actionComponent = dynamic_cast<juce::Component *>(existingComponentToUpdate);
        // create actionComponent if it does not exist
        if (actionComponent != nullptr) {
            return actionComponent;
        }

        actionComponent = new juce::Component();

        // create the "Edit" button
        auto *editButton = new juce::TextButton("Edit");
        editButton->addListener(this);
        editButton->setColour(juce::TextButton::buttonColourId, juce::Colours::lightblue);
        editButton->setBounds(0, 0, 40, 20);

        // create the "Delete" button
        auto *deleteButton = new juce::TextButton("Delete");
        deleteButton->addListener(this);
        deleteButton->setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        deleteButton->setBounds(40, 0, 40, 20);

        // add the buttons to the action component
        actionComponent->addAndMakeVisible(deleteButton);
        actionComponent->addAndMakeVisible(editButton);

        auto id{std::to_string(rowNumber)};
        editButton->setComponentID(id);
        deleteButton->setComponentID(id);

        return actionComponent;
    }

    return existingComponentToUpdate;
}

juce::var PlaylistComponent::getDragSourceDescription(const juce::SparseSet<int> &currentlySelectedRows) {
    std::cout << "PlaylistComponent::getDragSourceDescription" << std::endl;
    // the table only allows one row to be selected at a time, so that's why we only get the first element
    auto selectedRow = currentlySelectedRows[0];
    // get the track at the selected row
    auto track = trackList.at(currentlySelectedRows[0]);
    // create a dynamic object to hold the track data to be passed to the target component
    auto trackDynamicObj = new juce::DynamicObject();
    trackDynamicObj->setProperty("id", juce::var(track.id));
    trackDynamicObj->setProperty("title", juce::var(track.title));
    trackDynamicObj->setProperty("duration", juce::var(track.duration));
    trackDynamicObj->setProperty("fileURL", juce::var(track.fileURL.toString(true)));

    return trackDynamicObj;
}

void PlaylistComponent::buttonClicked(juce::Button *button) {

    // check if the button is delete or edit
    if (button->getName() == "Delete") {
        // delete the track
        auto id = std::stoi(button->getComponentID().toStdString());
        trackList.erase(trackList.begin() + id);
        tableComponent.updateContent();
    } else if (button->getName() == "Edit") {
        // edit the track
        auto id = std::stoi(button->getComponentID().toStdString());
        juce::Logger::writeToLog("Edit button clicked for track with id: " + std::to_string(id));
    }
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray &files) {
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray &trackFiles, int x, int y) {
    for (const auto &trackFile: trackFiles) {
        if (trackExists(trackFile)) {
            DBG("Track already exists in playlist");
            continue;
        }
        trackList.push_back(loadTrack(trackFile));
    }

    tableComponent.updateContent();
}

// This method loads a track from a file
PlaylistComponent::Track PlaylistComponent::loadTrack(const juce::File &trackFile) {
    // create a new track
    Track track;
    track.id = std::to_string(trackList.size() + 1);
    track.title = trackFile.getFileNameWithoutExtension().toStdString();
    track.album = "Album";
    track.fileURL = juce::URL{trackFile};

    // get the track's duration
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(trackFile));
    if (reader != nullptr) {
        const double durationInSeconds = reader->lengthInSamples / reader->sampleRate;
        track.duration = durationInSeconds;
    }

    return track;
}

// This method checks if a track with the same file URL already exists in the playlist
bool PlaylistComponent::trackExists(const juce::File &file) {
    for (const auto &track: trackList) {
        if (track.fileURL == juce::URL{file}) {
            return true;
        }
    }

    return false;
}

