#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}

DJLibraryService::~DJLibraryService(){
    for(AudioTrack* track_ptr: library){
        if(track_ptr != nullptr){
            delete track_ptr;
            track_ptr=nullptr;
        }
    }
}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for(SessionConfig::TrackInfo track: library_tracks){
        if(track.type == "MP3"){
            create_mp3_track(track);
        }
        if(track.type == "WAV"){
            create_wav_track(track);        
        }
        std::cout << "[INFO] Track library built: "<< library.size() <<" tracks loaded" << std::endl;
    }
    std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: "<< playlist_name <<std::endl;
    playlist = Playlist(playlist_name);
    for(int track_index: track_indices){
        if(track_index < 1 ||track_index > library.size()){
            std::cout << "[WARNING] Invalid track index: "<< track_index <<std::endl;
            continue;
        }

        AudioTrack* track_ptr = library[track_index-1] -> clone().release();

        if(track_ptr == nullptr){
            std::cout << "[ERROR] Track: failed to clone" << std::endl;
            continue;
        }
        
        track_ptr -> track_preparation();
        playlist.add_track(track_ptr);
        std::cout<< "Added \"" << track_ptr -> get_title() << "\" to playlist \""<< playlist_name <<"\""<<std::endl;
    }
    std::cout<< "[INFO] Playlist loaded: "<< playlist_name <<" ("<< playlist.get_track_count() <<" tracks)"<<std::endl;

    // For now, add a placeholder to fix the linker error
    (void)playlist_name;  // Suppress unused parameter warning
    (void)track_indices;  // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> track_titles = std::vector<std::string>();
    std::vector<AudioTrack*> tracks = playlist.getTracks();

    for(AudioTrack* track_ptr: tracks){
        track_titles.push_back(track_ptr -> get_title());
    }

    return track_titles;
}


void DJLibraryService::create_mp3_track(SessionConfig::TrackInfo track){
    MP3Track mp3_track = MP3Track(track.title, track.artists,track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
    AudioTrack* mp3_track_pointer = &mp3_track;
    library.push_back(mp3_track_pointer);
    std::cout << "MP3Track created: "<< mp3_track.get_bitrate() << " kbps"<< std::endl;
}

void DJLibraryService::create_wav_track(SessionConfig::TrackInfo track){
    WAVTrack wav_track = WAVTrack(track.title, track.artists,track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
    AudioTrack* wav_track_pointer = &wav_track;
    library.push_back(wav_track_pointer);
    std::cout << "WAVTrack created: "<< wav_track.get_sample_rate() << "Hz/"<< wav_track.get_bit_depth() <<"bit"<< std::endl;
}