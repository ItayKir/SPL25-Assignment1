#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if(cache.contains(track.get_title())){
        cache.get(track.get_title());
        return 1;
    }

    AudioTrack* ptr = track.clone().release();
    if(ptr == nullptr){
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone";
        return -99;
    }

    ptr -> load();
    ptr -> analyze_beatgrid();

    bool resp = cache.put(PointerWrapper<AudioTrack>(ptr));
    if(resp){ //evicted
        return -1;
    }
    return 0; //resp retruedn false, no tracks evicted when putting a new track in cache
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return cache.get(track_title); //get() will return nullptr if not found.
}
