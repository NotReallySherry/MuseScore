#include "process_notes.h"
#include <iostream>
#include <assert.h>     /* assert */
#include <string>
#include <fstream>

// constructor
ProcessNotes::ProcessNotes(int window_size, int notes_per_window, int p_speed, int whole_note_length, std::ofstream& output_file) :
    window(window_size, notes_per_window),
    outfile(output_file),
    polling_speed(p_speed),
    whole_note_length(whole_note_length) {}

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
    long cycle = std::stol(notes[0]);
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
    int seen = 0;
    for (int i = 0; i < result.size(); i++) {
        // length dynamics
        if (result[i].first > 0 && result[i].second > 0) {
            // if the note is pressed for more than 1 whole note
            print_result_terminal_helper(i, result[i].first, result[i].second);
            seen = 1;
        }
    }
    if (seen == 1)
      std::cout << std::endl;
}

void ProcessNotes::print_result_guido(int note, int length, int dynamics) {
    int note_length_musically_raw = whole_note_length / length;
    int note_length_musically = round_to_power_of_two(note_length_musically_raw);
    std::string note_str = print_result_guido_note(note);
    std::string length_str = std::to_string(note_length_musically);
    std::string dynamics_str = print_result_terminal_helper_dynamics(dynamics);
    std::cout << note_str << ", " << length_str << " is being pressed " << dynamics_str << std::endl;
    outfile << note_str << "/" << length_str << " " << std::endl;
}

std::string ProcessNotes::print_result_guido_note(int note) {
    std::string result;
    switch (note) {
        case 0:
            result += "c";
            break;
        case 1: 
            result += "c#";
            break;
        case 2:
            result += "d";
            break;
        case 3:
            result += "d#";
            break;
        case 4:
            result += "e";
            break;
        case 5:
            result += "f";
            break;
        case 6:
            result += "f#";
            break;
        case 7:
            result += "g";
            break;
        case 8:
            result += "g#";
            break;
        case 9:
            result += "a";
            break;
        case 10:
            result += "a#";
            break;
        case 11:
            result += "b";
            break;
        case 12:
            result += "c";
            break;  
        case 13:
            result += "c#";
            break;
        case 14:
            result += "d";
            break;
        case 15:
            result += "d#";
            break;    
    }
    return result;
}

void ProcessNotes::print_result_terminal_helper(int note, int length, int dynamics) {
    std::cout << "note: " << note << ", length: " << length << ", dynamics: " << dynamics << std::endl;
    int note_length_musically_raw = whole_note_length / length;
    int note_length_musically = round_to_power_of_two(note_length_musically_raw);
    std::string note_str = print_result_terminal_helper_note(note);
    std::string length_str = print_result_terminal_helper_length(note_length_musically);
    std::string dynamics_str = print_result_terminal_helper_dynamics(dynamics);
    std::cout << note_str << ", " << length_str << " is being pressed " << dynamics_str << std::endl;
    // outfile << note_str << ", " << length_str << " is being pressed " << dynamics_str << std::endl;
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
            result += "C# / Db";
            break;
        case 2:
            result += "D";
            break;
        case 3:
            result += "D# / Eb";
            break;
        case 4:
            result += "E";
            break;
        case 5:
            result += "F";
            break;
        case 6:
            result += "F# / Gb";
            break;
        case 7:
            result += "G";
            break;
        case 8:
            result += "G# / Ab";
            break;
        case 9:
            result += "A";
            break;
        case 10:
            result += "A# / Bb";
            break;
        case 11:
            result += "B";
            break;
        case 12:
            result += "C";
            break;  
        case 13:
            result += "C# / Db";
            break;
        case 14:
            result += "D";
            break;
        default:
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
