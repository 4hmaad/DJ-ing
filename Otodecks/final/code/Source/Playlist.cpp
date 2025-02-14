#include "Playlist.h"

Playlist::Playlist() {
}

Playlist::~Playlist() {
}

void Playlist::addTracks(const juce::StringArray &trackFiles) {
    for (auto &trackFile: trackFiles) {
        addTrack(trackFile);
    }
}

void Playlist::addTrack(const juce::File &file) {
    Track addedTrack = Track::parseFrom(file);
    //  check if track already exists, then don't add it
    for (auto &track: tracks) {
        if (track.getFileURLStr() == addedTrack.getFileURLStr()) {
            DBG("Track already exists");
            return;
        }
    }

    // otherwise add it to the playlist
    tracks.add(addedTrack);
}

void Playlist::removeTrack(std::string fileUrlOfTrackToDelete) {
    for (int i = 0; i < tracks.size(); i++) {
        if (tracks.getUnchecked(i).getFileURLStr() == fileUrlOfTrackToDelete) {
            tracks.remove(i);
            return;
        }
    }
}

void Playlist::editTrack(std::string fileURLOfTrackToEdit, std::string title, std::string album) {
    for (auto &track: tracks) {
        if (track.getFileURLStr() == fileURLOfTrackToEdit) {
            track.setTitle(title);
            track.setAlbum(album);
            return;
        }
    }
}

void Playlist::setSearchQuery(std::string query) {
    searchQuery = query;
}

void Playlist::save() {
    // to remove the existing playlists
    if (playlistFile.existsAsFile()) {
        playlistFile.deleteFile();
    }

    playlistFile.create();
    juce::var tracksList = juce::var(juce::Array<juce::var>());
    for (auto &track: tracks) {
        tracksList.append(Track::toVar(track));
    }

    juce::FileOutputStream outputStream(playlistFile);
    juce::JSON::writeToStream(outputStream, juce::JSON::parse(juce::JSON::toString(tracksList)));
}

void Playlist::load() {
    juce::FileInputStream inputStream(playlistFile);
    juce::var playlist = juce::JSON::parse(inputStream);
    juce::Array<Track> loadedTracks;
    for (auto &track: *playlist.getArray()) {
        loadedTracks.add(Track::parseFrom(track));
    }

    tracks = loadedTracks;
}

void Playlist::clear() {
    tracks.clear();
}

juce::Array<Track> Playlist::getTracks() {
    if (searchQuery.empty()) {
        return tracks;
    }

    juce::Array<Track> filteredTracks = juce::Array<Track>();
    for (auto &track: tracks) {
        std::string title = track.getTitle();
        std::transform(title.begin(), title.end(), title.begin(), ::tolower);

        std::string album = track.getAlbum();
        std::transform(album.begin(), album.end(), album.begin(), ::tolower);

        std::string query = searchQuery;
        std::transform(query.begin(), query.end(), query.begin(), ::tolower);

        // if some part of title matches the query, add it to the filtered tracks
        if (title.find(query) != std::string::npos) {
            filteredTracks.add(track);
        }
    }

    return filteredTracks;
}