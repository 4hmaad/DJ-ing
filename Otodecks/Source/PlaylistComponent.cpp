#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"
#include "EditTrackDialog.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer *_player,
                                     juce::AudioFormatManager &formatManagerToUse) : player(_player),
                                                                                     formatManager(formatManagerToUse) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(clearPlaylist);


    // set up search box
    searchBox.setTextToShowWhenEmpty("Search", juce::Colours::grey);
    searchBox.setJustification(juce::Justification::verticallyCentred);

    tableComponent.getHeader().addColumn("#", ColumnIds::idColumnId, 15);
    tableComponent.getHeader().addColumn("TITLE", ColumnIds::titleColumnId, 300);
    tableComponent.getHeader().addColumn("ALBUM", ColumnIds::albumColumnId, 100);
    tableComponent.getHeader().addColumn("DURATION", ColumnIds::durationColumnId, 60);
    tableComponent.getHeader().addColumn("", ColumnIds::actionColumnId, 100);
    tableComponent.getHeader().setStretchToFitActive(true);

    tableComponent.setModel(this);
    clearPlaylist.addListener(this);
    searchBox.addListener(this);

    playlist.load();
}

PlaylistComponent::~PlaylistComponent() {
    playlist.save();
}

void PlaylistComponent::paint(juce::Graphics &g) {
}

void PlaylistComponent::resized() {
    juce::Grid playListControlsGrid;
    playListControlsGrid.autoColumns = juce::Grid::TrackInfo(juce::Grid::Fr(1));
    playListControlsGrid.templateRows = juce::Grid::TrackInfo(juce::Grid::Fr(1));
    playListControlsGrid.items = {
            juce::GridItem(searchBox).withArea(1, 1).withMargin(juce::GridItem::Margin(10)),
            juce::GridItem(clearPlaylist).withArea(1, 2).withMargin(juce::GridItem::Margin(10))};

    playListControlsGrid.performLayout(getLocalBounds().removeFromTop(50));
    tableComponent.setBounds(0, 50, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows() {
    return playlist.getTracks().size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics &g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colour::fromRGB(50, 67, 75));
    } else {
        g.fillAll(juce::Colour::fromRGB(32, 43, 49));
    }
}

void PlaylistComponent::paintCell(juce::Graphics &g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected) {
    auto tracks = playlist.getTracks();
    auto track = tracks.getUnchecked(rowNumber);

    std::map<int, std::string> columnData = {
            {ColumnIds::idColumnId,       std::to_string(rowNumber + 1)},
            {ColumnIds::titleColumnId,    track.title},
            {ColumnIds::albumColumnId,    track.album},
            {ColumnIds::durationColumnId, "02:45"}
    };
    std::string cellColumnContent = columnData[columnId];

    g.setColour(juce::Colours::white);
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
        editButton->setBounds(20, 0, 40, 20);

        // create the "Delete" button
        auto *deleteButton = new juce::TextButton("Delete");
        deleteButton->addListener(this);
        deleteButton->setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        deleteButton->setBounds(70, 0, 40, 20);

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
    auto track = playlist.getTracks().getUnchecked(selectedRow);
    // create a dynamic object to hold the track data to be passed to the target component
    return Track::toVar(track);
}

void PlaylistComponent::buttonClicked(juce::Button *button) {

    // check if the button is delete or edit
    if (button->getName() == "Delete") {

        // shows juce message box to confirm deletion
        juce::AlertWindow::showOkCancelBox(
                juce::AlertWindow::QuestionIcon,
                "Delete Track",
                "Are you sure you want to delete this track?",
                "Yes",
                "No",
                nullptr,
                juce::ModalCallbackFunction::create([this, button](int result) {
                    if (result == 1) {
                        auto id = std::stoi(button->getComponentID().toStdString());
                        playlist.removeTrack(id);
                        tableComponent.updateContent();
                    }
                }));
    } else if (button->getName() == "Edit") {
        auto trackId = std::stoi(button->getComponentID().toStdString());
        auto track = playlist.getTracks().getUnchecked(trackId);

        new EditTrackDialog(track.getTitle(), track.getAlbum(), [this, trackId](std::string title, std::string album) {
            playlist.editTrack(trackId, title, album);
        });

    } else if (button == &clearPlaylist) {
        playlist.clear();
    }

    tableComponent.updateContent();
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray &files) {
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray &trackFiles, int x, int y) {
    playlist.addTracks(trackFiles);
    tableComponent.updateContent();
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor &editor) {
    playlist.setSearchQuery(editor.getText().toStdString());
    tableComponent.updateContent();
}

