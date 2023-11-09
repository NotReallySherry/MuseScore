#include "sliding_window.h"
#include <iostream>

void test_sliding_window() {
    // instantiate new sliding window
    SlidingWindow window(4, 2);
    // update the window with new notes in a loop
    for (int i = 0; i < 10; i++) {
        std::vector<int> new_notes;
        new_notes.push_back(i);
        new_notes.push_back(i + 1);
        // update the window
        window.update(new_notes);
        // print out the notes in the window
        std::vector<int> notes = window.get_notes();
        for (int j = 0; j < notes.size(); j++) {
            std::cout << notes[j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    test_sliding_window();
    return 0;
}