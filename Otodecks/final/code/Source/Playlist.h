#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "domain/Track.h"

class Playlist {

public:

    Playlist();

    ~Playlist();

    /*
     * Add tracks to the playlist from a list of files
     *
     * @param trackFiles - the list of files to add
     */
    void addTracks(const juce::StringArray &trackFiles);

    /*
     * Add a track to the playlist from a file
     *
     * @param file - the file to add
     */
    void addTrack(const juce::File &file);

    /*
     * Remove a track from the playlist
     *
     * @param fileUrlOfTrackToDelete - the file url of the track to delete
     */
    void removeTrack(std::string fileUrlOfTrackToDelete);

    /*
     * Edit a track in the playlist (not the file)
     *
     * @param fileURLOfTrackToEdit - the file url of the track to edit
     * @param title - the new title of the track
     * @param album - the new album of the track
     */
    void editTrack(std::string fileURLOfTrackToEdit, std::string title, std::string album);

    /*
     * Set the search query for the playlist (pass an empty string to clear the search query)
     *
     * @param query - the search query
     */
    void setSearchQuery(std::string query);

    /*
     * Persists the playlist to a file on the disk
     * Called when the app closes
     */
    void save();

    /*
     * Loads the playlist from a file on the disk
     * Called when the app starts
     */
    void load();

    /*
     * Clears the playlist
     */
    void clear();

    /*
     * Get the tracks in the playlist
     *
     * @return the tracks in the playlist
     */
    juce::Array<Track> getTracks();

private:
    juce::Array<Track> tracks;
    std::string searchQuery;

    // Constants
    juce::File playlistFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile(
            "Otodecks/data/playlist.json");
};
