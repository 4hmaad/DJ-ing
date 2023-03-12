/*
  ==============================================================================

    Track.cpp
    Created: 12 Mar 2023 6:37:48pm
    Author:  Ahmad Nawaz Khan

  ==============================================================================
*/

#include "Track.h"

Track::Track(
        std::string title,
        std::string album,
        float duration,
        juce::URL fileURL) :
        title(title),
        album(album),
        duration(
                duration),
        fileURL(fileURL) {
}

std::string Track::getTitle() {
    return title;
}

std::string Track::setTitle(const std::string _title) {
    title = _title;
    return title;
}

std::string Track::getAlbum() {
    return album;
}

std::string Track::setAlbum(const std::string _album) {
    album = _album;
    return album;
}

float Track::getDuration() {
    return duration;
}

juce::URL Track::getFileURL() {
    return fileURL;
}

std::string Track::getFileURLStr() {
    return fileURL.toString(true).toStdString();
}

Track Track::parseFrom(const juce::File &file) {

    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    std::string title = file.getFileNameWithoutExtension().toStdString();
    std::string album = "Unknown";
    juce::URL fileURL = juce::URL{file};
    float duration;

    // get the track's duration
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader != nullptr) {
        const double durationInSeconds = reader->lengthInSamples / reader->sampleRate;
        duration = durationInSeconds;
    }

    return Track(title, album, duration, fileURL);
}

Track Track::parseFrom(const juce::var &trackVar) {
    std::string title = trackVar["title"].toString().toStdString();
    std::string album = trackVar["album"].toString().toStdString();
    float duration = trackVar["duration"];
    juce::URL fileURL = juce::URL{trackVar["fileURL"].toString()};

    return Track(title, album, duration, fileURL);
}

juce::var Track::toVar(Track track) {
    auto trackVar = new juce::DynamicObject();
    trackVar->setProperty("title", juce::var(track.getTitle()));
    trackVar->setProperty("album", juce::var(track.getAlbum()));
    trackVar->setProperty("duration", juce::var(track.getDuration()));
    trackVar->setProperty("fileURL", juce::var(track.getFileURLStr()));

    return trackVar;
}