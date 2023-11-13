
#include "sliding_window.h"
#include <iostream>

SlidingWindow::SlidingWindow(int window_capacity, int notes_per_window) {
    capacity = window_capacity;
    notes_count = notes_per_window;
}

void SlidingWindow::update(std::vector<int> new_notes) {
    window.push_back(new_notes);
    if (window.size() > capacity) {
        window.erase(window.begin());
    }
}

std::vector<int> SlidingWindow::get_notes() {
    // result is the array of duration of notes being pressed
    std::vector<int> result;
    if (window.size() < 3) {
        for (int i = 0; i < notes_count; i++) {
            result.push_back(0);
        }
        return result;
    }
    std::vector<int> last_window = window.back();
    std::vector<int> last2_window = window[window.size() - 2];
    // loop through the set of notes in the window
    for (int i = 0; i < notes_count; i++) {
        // the note is current not pressed but pressed in the previous window
        if (last_window[i] == 0 && last2_window[i] == 1) {
            // count how many windows the note has been pressed
            int length = 0;
            for (int j = window.size() - 2; j >= 0; j--) {
                if (window[j][i] == 1) {
                    length++;
                } else {
                    break;
                }
            }
            result.push_back(length);
        } else {
            result.push_back(0);
        }
    }

    return result;
}

void SlidingWindow::print_window() {
    std::cout << "window content: ";
    for (int i = 0; i < window.size(); i++) {
        for (int j = 0; j < window[i].size(); j++) {
            std::cout << window[i][j] << " ";
        }
        std::cout << "- ";
    }
    std::cout << std::endl;
}