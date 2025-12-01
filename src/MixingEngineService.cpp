#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(0), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks."<< std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks...."<< std::endl;
    for(size_t i = 0; i < 2 ; i++){
        if(decks[i]!=nullptr){
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ==="<< std::endl;
    
    PointerWrapper<AudioTrack> wrapped_ptr = track.clone();
    if(!wrapped_ptr){
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone";
        return -1;
    }

    AudioTrack* ptr = wrapped_ptr.release();
    
    if(isDeckEmpty()){ //initial state + first track
        decks[0] = ptr;
        active_deck = 0;
    }

    size_t target_deck_index = getInactiveDeck();
    std::cout << "[Deck Switch] Target deck " << target_deck_index << std::endl;

    if(decks[target_deck_index] != nullptr){
        delete decks[target_deck_index];
        decks[target_deck_index] = nullptr;
    }
    ptr -> track_preparation(); //load and analyze_beatgrid
    if(auto_sync){
        int active_deck_track_bpm = decks[active_deck] -> get_bpm();
        int current_track_bpm = ptr -> get_bpm();
        if(can_mix_tracks(ptr))
    }

    return -1; // Placeholder
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(isDeckEmpty() || !track){ //decks[active_deck] is the same as the deck being empty...
        return false;
    }

    int active_deck_track_bpm = decks[active_deck] -> get_bpm();
    int current_track_bpm = track -> get_bpm();

    if( abs(active_deck_track_bpm - current_track_bpm) <= bpm_tolerance){
        return true;
    }
    return false; 
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
}
