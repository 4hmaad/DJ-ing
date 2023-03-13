#pragma once

#include <string>
#include <vector>
#include "../../JuceLibraryCode/JuceHeader.h"

/*
 * Track class represents a single track in the playlist
 */
class Track {
public:

    std::string title;
    std::string album;
    float duration;
    juce::URL fileURL;

    Track(std::string title, std::string album, float duration, juce::URL fileURL);

    /*
     * Get the title of the track
     *
     * @return the title of the track
     */
    std::string getTitle();

    /*
     * Set the title of the track
     *
     * @param title - the title of the track
     * @return the updated title of the track
     */
    std::string setTitle(const std::string title);

    /*
     * Get the album of the track
     *
     * @return the album of the track
     */
    std::string getAlbum();

    /*
     * Set the album of the track
     *
     * @param album - the album of the track
     * @return the updated album of the track
     */
    std::string setAlbum(const std::string album);

    /*
     * Get the duration of the track in seconds
     *
     * @return the duration of the track
     */
    float getDuration();

    /*
     * Get the duration of the track in parsed string format (mm:ss)
     *
     * @return the duration of the track in parsed string format (mm:ss)
     */
    std::string getParsedDuration();

    /*
     * Get the file URL of the track
     *
     * @return the file URL of the track
     */
    juce::URL getFileURL();

    /*
     * Get the file URL of the track as a string
     *
     * @return the file URL of the track as a string
     */
    std::string getFileURLStr();


    /*
     * Parse a track from a file
     *
     * @param file - the file to parse
     * @return the parsed track
     */
    static Track parseFrom(const juce::File &file);


    /*
     * Parse a track from a var
     *
     * @param trackVar - the var to parse
     * @return the parsed track
     */
    static Track parseFrom(const juce::var &trackVar);

    /*
     * Convert a track to a var
     *
     * @param track - the track to convert
     * @return the converted var
     */
    static juce::var toVar(Track track);
};