#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "domain/Track.h"

class Playlist {

public:

    Playlist();

    ~Playlist();

    void addTracks(const juce::StringArray &trackFiles);

    void addTrack(const juce::File &file);

    void removeTrack(int index);

    void editTrack(Track updatedTrack);
    void editTrack(int index, std::string title, std::string album);

    void setSearchQuery(std::string query);

    void save();

    void load();

    void clear();

    juce::Array<Track> getTracks();

private:
    juce::Array<Track> tracks;
    std::string searchQuery;

    // Constants
    juce::File playlistFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Otodecks/data/playlist.json");
};
