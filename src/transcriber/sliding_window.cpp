
#include "sliding_window.h"

SlidingWindow::SlidingWindow(int window_size, int notes_per_window) {
    size = window_size;
    notes_count = notes_per_window;
}

void SlidingWindow::update(std::vector<int> new_notes) {
    window.push_back(new_notes);
    if (window.size() > size) {
        window.erase(window.begin());
    }
}

std::vector<int> SlidingWindow::get_notes() {
    std::vector<int> result;
    for (int i = 0; i < window.size(); i++) { // loop through the time frame
        for (int j = 0; j < window[i].size(); j++) { // loop through the notes
            result.push_back(window[i][j]);
        }
    }
    return result;
}