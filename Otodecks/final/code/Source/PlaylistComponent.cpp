#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"
#include "EditTrackDialog.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(
        juce::AudioFormatManager &formatManagerToUse) :
        formatManager(formatManagerToUse) {

    getLookAndFeel().setUsingNativeAlertWindows(false);

    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(clearPlaylist);
    addAndMakeVisible(addTrack);

    // set up search box
    searchBox.setTextToShowWhenEmpty("Search", juce::Colours::grey);
    searchBox.setJustification(juce::Justification::verticallyCentred);

    tableComponent.getHeader().addColumn("#", ColumnIds::idColumnId, 30, 30, 30,
                                         juce::TableHeaderComponent::notSortable);
    tableComponent.getHeader().addColumn("TITLE", ColumnIds::titleColumnId, 400, 100, -1,
                                         juce::TableHeaderComponent::notSortable);
    tableComponent.getHeader().addColumn("ALBUM", ColumnIds::albumColumnId, 150, 30, -1,
                                         juce::TableHeaderComponent::notSortable);
    tableComponent.getHeader().addColumn("DURATION", ColumnIds::durationColumnId, 100, 20, 100,
                                         juce::TableHeaderComponent::notSortable);
    tableComponent.getHeader().addColumn("", ColumnIds::actionEditColumnId, 150, 40, 150,
                                         juce::TableHeaderComponent::notSortable);
    tableComponent.getHeader().addColumn("", ColumnIds::actionDeleteColumnId, 150, 40, 150,
                                         juce::TableHeaderComponent::notSortable);


    tableComponent.setModel(this);
    clearPlaylist.addListener(this);
    searchBox.addListener(this);
    addTrack.addListener(this);

    playlist.load();
}

PlaylistComponent::~PlaylistComponent() {
    playlist.save();

}

void PlaylistComponent::paint(juce::Graphics &g) {
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(getLocalBounds().removeFromTop(50), 2);
}

void PlaylistComponent::resized() {
    juce::Grid playListControlsGrid;
    playListControlsGrid.autoColumns = juce::Grid::TrackInfo(juce::Grid::Fr(1));
    playListControlsGrid.templateRows = juce::Grid::TrackInfo(juce::Grid::Fr(1));
    playListControlsGrid.items = {
            juce::GridItem(searchBox).withArea(1, 1).withMargin(juce::GridItem::Margin(10)),
            juce::GridItem(addTrack).withArea(1, 2).withMargin(juce::GridItem::Margin(10)),
            juce::GridItem(clearPlaylist).withArea(1, 3).withMargin(juce::GridItem::Margin(10))
    };

    playListControlsGrid.performLayout(getLocalBounds().removeFromTop(50));
    tableComponent.setBounds(0, 50, getWidth(), getHeight() - 50);

    tableComponent.getHeader().setStretchToFitActive(true);
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
            {ColumnIds::titleColumnId,    track.getTitle()},
            {ColumnIds::albumColumnId,    track.getAlbum()},
            {ColumnIds::durationColumnId, track.getParsedDuration()}
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
    if (columnId == ColumnIds::actionEditColumnId) {
        auto *editComponent = dynamic_cast<juce::TextButton *>(existingComponentToUpdate);
        // create actionComponent if it does not exist
        if (editComponent != nullptr) {
            return editComponent;
        }

        editComponent = new juce::TextButton("Edit");
        editComponent->addListener(this);

        auto id{std::to_string(rowNumber)};
        editComponent->setComponentID(id);

        existingComponentToUpdate = editComponent;
    } else if (columnId == ColumnIds::actionDeleteColumnId) {
        auto *deleteComponent = dynamic_cast<juce::TextButton *>(existingComponentToUpdate);
        // create actionComponent if it does not exist
        if (deleteComponent != nullptr) {
            return deleteComponent;
        }

        deleteComponent = new juce::TextButton("Delete");
        deleteComponent->addListener(this);
        deleteComponent->setColour(juce::TextButton::buttonColourId, juce::Colours::red);

        auto id{std::to_string(rowNumber)};
        deleteComponent->setComponentID(id);

        existingComponentToUpdate = deleteComponent;
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
    // get the track id from the button's component id

    if (button->getName() == "Delete") {
        auto trackId = std::stoi(button->getComponentID().toStdString());
        auto track = playlist.getTracks().getUnchecked(trackId);
        playlist.removeTrack(track.getFileURLStr());
    } else if (button->getName() == "Edit") {
        auto trackId = std::stoi(button->getComponentID().toStdString());
        auto track = playlist.getTracks().getUnchecked(trackId);

        new EditTrackDialog(track.getTitle(), track.getAlbum(),
                            [this, trackId](std::string &title, std::string &album) {
                                std::string trackFileURL = playlist.getTracks().getUnchecked(trackId).getFileURLStr();
                                playlist.editTrack(trackFileURL, title, album);
                            });
    } else if (button == &clearPlaylist) {
        // show message box to confirm clearing of playlist
        juce::AlertWindow::showOkCancelBox(
                juce::AlertWindow::QuestionIcon,
                "Clear Playlist",
                "Are you sure you want to clear the playlist?",
                "Yes",
                "No",
                nullptr,
                juce::ModalCallbackFunction::create([this](int result) {
                    if (result == 1) {
                        playlist.clear();
                        tableComponent.updateContent();
                    }
                }));
    } else if (button == &addTrack) {
        chooser.launchAsync(
                juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles |
                juce::FileBrowserComponent::canSelectMultipleItems,
                [this](const juce::FileChooser &fileChooser) {
                    auto results = fileChooser.getResults();
                    if (results.isEmpty()) return;

                    juce::StringArray trackFiles;
                    for (auto &result: results) {
                        trackFiles.add(result.getFullPathName());
                    }
                    playlist.addTracks(trackFiles);
                    tableComponent.updateContent();
                });
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

