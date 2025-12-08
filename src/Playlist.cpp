#include "Playlist.h"
#include "AudioTrack.h"
#include <iostream>
#include <algorithm>
Playlist::Playlist(const std::string& name) 
    : head(nullptr), playlist_name(name), track_count(0) {
    std::cout << "Created playlist: " << name << std::endl;
}
// TODO: Fix memory leaks!
// Students must fix this in Phase 1
Playlist::~Playlist() {
    #ifdef DEBUG
    std::cout << "Destroying playlist: " << playlist_name << std::endl;
    #endif 
    clear_playlist();
}

void Playlist::clear_playlist(){
    PlaylistNode* current_node = head;
    while(current_node != nullptr){
        PlaylistNode* next_node = current_node -> next;
        delete current_node;
        current_node = next_node;
    }
    head=nullptr;
    track_count=0;
}

Playlist::Playlist(Playlist&& other) noexcept 
    : head(other.head), playlist_name(std::move(other.playlist_name)), track_count(other.track_count) {
    other.head = nullptr;
    other.track_count=0;
}

Playlist& Playlist::operator=(Playlist&& other) noexcept {
    if (this == &other) {
        return *this; 
    }
    clear_playlist();

    head = other.head;
    playlist_name = std::move(other.playlist_name);

    other.head = nullptr;

    return *this;
}

Playlist::Playlist(const Playlist& other)
    :head(nullptr), playlist_name(other.playlist_name), track_count(0)
    {
        if(other.head == nullptr){
            return;
        }
        head = new PlaylistNode(other.head->track -> clone().release());
        track_count++;

        PlaylistNode* other_next_node = other.head->next;
        PlaylistNode* this_curr_node = head;

        while (other_next_node != nullptr) {
            this_curr_node->next = new PlaylistNode(other_next_node->track -> clone().release());
            
            this_curr_node = this_curr_node->next;
            other_next_node = other_next_node->next;
            track_count++;
        }

    }


Playlist& Playlist::operator=(const Playlist& other) {
    if (this == &other) {
        return *this; // Self-assignment check
    }
    clear_playlist(); 
    playlist_name = other.playlist_name;
    
    if (other.head == nullptr) {
        return *this;
    }

    head = new PlaylistNode(other.head->track -> clone().release());
    track_count++;

    PlaylistNode* other_next_node = other.head->next;
    PlaylistNode* this_curr_node = head;

    while (other_next_node != nullptr) {
        this_curr_node->next = new PlaylistNode(other_next_node->track -> clone().release());
        this_curr_node = this_curr_node->next;
        other_next_node = other_next_node->next;
        track_count++;
    }
    return *this;
}

void Playlist::add_track(AudioTrack* track) {
    if (!track) {
        std::cout << "[Error] Cannot add null track to playlist" << std::endl;
        return;
    }

    // Create new node - this allocates memory!
    PlaylistNode* new_node = new PlaylistNode(track);

    // Add to front of list
    new_node->next = head;
    head = new_node;
    track_count++;

    std::cout << "Added '" << track->get_title() << "' to playlist '" 
              << playlist_name << "'" << std::endl;
}

void Playlist::remove_track(const std::string& title) {
    PlaylistNode* current = head;
    PlaylistNode* prev = nullptr;

    // Find the track to remove
    while (current && current->track->get_title() != title) {
        prev = current;
        current = current->next;
    }

    if (current) {
        // Remove from linked list
        if (prev) {
            prev->next = current->next;
        } else {
            head = current->next;
        }
		delete current;
        track_count--;
        std::cout << "Removed '" << title << "' from playlist" << std::endl;

    } else {
        std::cout << "Track '" << title << "' not found in playlist" << std::endl;
    }
}

void Playlist::display() const {
    std::cout << "\n=== Playlist: " << playlist_name << " ===" << std::endl;
    std::cout << "Track count: " << track_count << std::endl;

    PlaylistNode* current = head;
    int index = 1;

    while (current) {
        std::vector<std::string> artists = current->track->get_artists();
        std::string artist_list;

        std::for_each(artists.begin(), artists.end(), [&](const std::string& artist) {
            if (!artist_list.empty()) {
                artist_list += ", ";
            }
            artist_list += artist;
        });

        AudioTrack* track = current->track;
        std::cout << index << ". " << track->get_title() 
                  << " by " << artist_list
                  << " (" << track->get_duration() << "s, " 
                  << track->get_bpm() << " BPM)" << std::endl;
        current = current->next;
        index++;
    }

    if (track_count == 0) {
        std::cout << "(Empty playlist)" << std::endl;
    }
    std::cout << "========================\n" << std::endl;
}

AudioTrack* Playlist::find_track(const std::string& title) const {
    PlaylistNode* current = head;

    while (current) {
        if (current->track->get_title() == title) {
            return current->track;
        }
        current = current->next;
    }

    return nullptr;
}

int Playlist::get_total_duration() const {
    int total = 0;
    PlaylistNode* current = head;

    while (current) {
        total += current->track->get_duration();
        current = current->next;
    }

    return total;
}

std::vector<AudioTrack*> Playlist::getTracks() const {
    std::vector<AudioTrack*> tracks;
    PlaylistNode* current = head;
    while (current) {
        if (current->track)
            tracks.push_back(current->track);
        current = current->next;
    }
    return tracks;
}