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

    std::string getTitle();
    std::string setTitle(const std::string title);

    std::string getAlbum();
    std::string setAlbum(const std::string album);

    float getDuration();

    juce::URL getFileURL();

    static Track parseFrom(const juce::File &file);
    static Track parseFrom(const juce::var &trackVar);

    static juce::var toVar(Track track);

};