
#include "sliding_window.h"
#include <iostream>
#include <iterator>

SlidingWindow::SlidingWindow(int window_capacity, int notes_per_window) {
    capacity = window_capacity;
    notes_count = notes_per_window;
    rest_count = 0;
    prev_rest_count = 0;
}

void SlidingWindow::update(std::vector<int> new_notes) {
    window.push_back(new_notes);
    if (window.size() > capacity) {
        window.erase(window.begin());
    }
}

std::vector<std::pair<int, int> > SlidingWindow::get_notes() {
    // result is the array of length:average_dynamics pairs
    std::vector<std::pair<int, int> > result;
    if (window.size() < 2) {
        for (int i = 0; i < notes_count; i++) {
            result.push_back(std::make_pair(0, 0));
        }
        return result;
    }
    std::vector<int> last_window = window.back();
    std::vector<int> last2_window = window[window.size() - 2];
    bool is_rest = true;

    // loop through the set of notes in the window
    for (int i = 0; i < notes_count; i++) {
        if (last_window[i] > 0) {
            is_rest = false;
        }

        // the note is current not pressed but pressed in the previous window
        if (last_window[i] == 0 && last2_window[i] > 0) {
            int dynamics = 0;
            // count how many windows the note has been pressed
            int length = 0;
            for (int j = window.size() - 2; j >= 0; j--) {
                if (window[j][i] > 0) {
                    length++;
                    dynamics += window[j][i];
                } else {
                    break;
                }
            }
            int avg_dynamics = dynamics / length;
            result.push_back(std::make_pair(length, avg_dynamics));
        } else {
            result.push_back(std::make_pair(0, 0));
        }
    }

    if (is_rest) {
        rest_count++;
    } else {
        prev_rest_count = rest_count;
        rest_count = 0;
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