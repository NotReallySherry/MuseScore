#include "process_notes.h"
#include <iostream>
#include <assert.h>     /* assert */

// constructor 
ProcessNotes::ProcessNotes(int window_size, int notes_per_window, int p_speed, int whole_note_length) : window(window_size, notes_per_window) {
    polling_speed = p_speed;
    whole_note_length = whole_note_length;
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
    for (int i = 1; i < notes.size(); i++) {
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
    // window.print_window();
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
    for (int i = 0; i < result.size(); i++) {
        // length dynamics
        if (result[i].first > 0 && result[i].second > 0) {
            // if the note is pressed for more than 1 whole note
            print_result_terminal_helper(i, result[i].first, result[i].second);
        } 
    }
    std::cout << std::endl;
}

void ProcessNotes::print_result_terminal_helper(int note, int length, int dynamics) {
    int note_length_musically_raw = whole_note_length / length;
    int note_length_musically = round_to_power_of_two(note_length_musically_raw);
    std::string note_str = print_result_terminal_helper_note(note);
    std::string length_str = print_result_terminal_helper_length(note_length_musically);
    std::string dynamics_str = print_result_terminal_helper_dynamics(dynamics);
    std::cout << note_str << ", " << length_str << " is being pressed " << dynamics_str << std::endl;
}

int ProcessNotes::round_to_power_of_two(int num) {
    int result = 1;
    while (result < num) {
        result *= 2;
    }
    return result;
}

std::string ProcessNotes::print_result_terminal_helper_note(int note) {
    std::string result = "note ";
    switch (note) {
        case 0:
            result += "C";
            break;
        case 1:
            result += "D";
            break;
        case 2:
            result += "E";
            break;
        case 3:
            result += "F";
            break;
        case 4:
            result += "G";
            break;
        case 5:
            result += "A";
            break;
        case 6:
            result += "B";
            break;
        default:
            result += "invalid";
            break;
    }
    return result;
}

std::string ProcessNotes::print_result_terminal_helper_length(int length) {
    std::string result;
    switch (length) {
        case 1:
            result += "whole";
            break;
        case 2:
            result += "half";
            break;
        case 4:
            result += "quarter";
            break;
        case 8:
            result += "eighth";
            break;
        case 16:
            result += "sixteenth";
            break;
        case 32:
            result += "thirty-second";
            break;
        case 64:
            result += "sixty-fourth";
            break;
        default:
            result += "invalid";
            break;
    }
    return result;
}  

std::string ProcessNotes::print_result_terminal_helper_dynamics(int dynamics) {
    std::string result;
    switch (dynamics) {
        case 0:
            result += "not pressed";
            break;
        case 1:
            result += "lightly";
            break;
        case 2:
            result += "medium";
            break;
        case 3:
            result += "hard";
            break;
        default:
            result += "❌invalid❌";
            break;
    }
    return result;
}


// void ProcessNotes::call_api(std::vector<std::pair<int, int> > result) {
//     for (int i = 0; i < result.size(); i++) {
//         if (result[i].first > 0) {
//             QKeyEvent pressEvent(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier, "A");
//         } 
//     }
// }