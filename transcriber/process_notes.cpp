#include "process_notes.h"
#include <iostream>
#include <assert.h>     /* assert */

// constructor 
ProcessNotes::ProcessNotes(int window_size, int notes_per_window, int p_speed) : window(window_size, notes_per_window) {
    polling_speed = p_speed;
}

// update the sliding window with new notes
void ProcessNotes::update(std::string new_notes) {
    // string format: 
    // pulling cycle, bpm, note1:dynamic1, note2:dynamic2, note3:dynamic3, note4:dynamic4, note5:dynamic5, note6:dynamic6, note7:dynamic7
    // dynamics comes in 0-3, 0 is not pressed, 1 is pressed lightly, 2 is pressed medium, 3 is pressed hard
    // split the string by comma
    std::vector<std::string> notes;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = new_notes.find(delimiter)) != std::string::npos) {
        token = new_notes.substr(0, pos);
        notes.push_back(token);
        new_notes.erase(0, pos + delimiter.length());
    }

    int cycle = std::stoi(notes[0]);
    if (cycle % polling_speed != 0) {
        return;
    }

    // update the sliding window
    std::vector<int> new_notes_vector;
    for (int i = 2; i < notes.size(); i++) {
        // split the note and dynamic
        std::string note = notes[i].substr(0, notes[i].find(":"));
        std::string dynamic = notes[i].substr(notes[i].find(":") + 1);
        // convert the note and dynamic to int
        int note_int = std::stoi(note);
        int dynamic_int = std::stoi(dynamic);
            assert(dynamic_int >= 0 && dynamic_int <= 3);
        // add the note to the new_notes_vector
        new_notes_vector.push_back(dynamic_int);
    }
    window.update(new_notes_vector);

    /////////// print out the window ///////////
    window.print_window();
    ///////////////////////////////////////////
}

// get the processed notes from the sliding window
std::vector<std::pair<int, int> > ProcessNotes::get_notes() {
    return window.get_notes();
}

int ProcessNotes::get_polling_speed() {
    return polling_speed;
}

void ProcessNotes::print_get_notes_result(std::vector<std::pair<int, int> > result) {
    std::cout << "notes result: ";
    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i].first << " " << result[i].second << " ";
    }
    std::cout << std::endl;
}