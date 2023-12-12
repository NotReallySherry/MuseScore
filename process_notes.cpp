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
    // if rest_count is larger than a whole note, output a rest and reduce rest_count
    if (window.rest_count >= whole_note_length) {
        std::cout << "rest" << std::endl;
        outfile << "r1 ";
        window.rest_count = window.rest_count - whole_note_length;
    } else if (window.rest_count == 0 && window.prev_rest_count > 0) {
        std::cout << "rest" << std::endl;
        std::string rest_length = round_to_valid_length(window.prev_rest_count);
        // outfile << "r" << rest_length << " ";
        window.prev_rest_count = 0;
    }

    // get the processed notes from the sliding window
    std::vector<std::pair<int, int> > result = window.get_notes();

    return result;
}

std::string ProcessNotes::round_to_valid_length(int length) {
    // Calculate the ratio of rest length to whole note length
    double ratio = static_cast<double>(length) / whole_note_length;

    // Define the valid note lengths and their representations
    double validNoteLengths[] = {1.0, 0.5, 0.25, 0.125, 0.0625}; // Valid lengths up to 64th note

    // Find the closest valid note length
    double closestNoteLength = validNoteLengths[0];
    for (int i = 1; i < sizeof(validNoteLengths) / sizeof(validNoteLengths[0]); ++i) {
        if (std::abs(ratio - validNoteLengths[i]) < std::abs(ratio - closestNoteLength)) {
            closestNoteLength = validNoteLengths[i];
        }
    }

    // Convert the closest valid note length to the corresponding representation
    int representation = static_cast<int>(1.0 / closestNoteLength);

    // Check for extra half (dot)
    if (ratio > closestNoteLength) {
        return std::to_string(representation) + ".";
    } else {
        return std::to_string(representation);
    }
}

int ProcessNotes::get_polling_speed() {
    return polling_speed;
}


void ProcessNotes::print_get_notes_result(std::vector<std::pair<int, int> > result) {
    int result_count = 0;
    std::vector<std::string> result_strs;
    std::vector<int> result_lengths;
    for (int i = 0; i < result.size(); i++) {
        // length dynamics
        if (result[i].first > 0 && result[i].second > 0) {
            // if the note is pressed for more than 1 whole note
            int length;
            std::string str = print_result_helper(i, result[i].first, result[i].second, &length);
            result_strs.push_back(str);
            result_lengths.push_back(length);
            result_count++;
        }
    }
    if (result_count > 0) {
        std::cout << std::endl;
        
        int majority_length = majority_vote(result_lengths);
        outfile << "<";
        for (int i = 0; i < result_strs.size(); i++) {
            outfile << result_strs[i] << " ";
        }
        outfile << ">" << majority_length << " ";
    }
    // flush the output file
    outfile.flush();
}

int ProcessNotes::majority_vote(std::vector<int> lengths) {
    int result = 0;
    int max_count = 0;
    for (int i = 0; i < lengths.size(); i++) {
        int count = 0;
        for (int j = 0; j < lengths.size(); j++) {
            if (lengths[j] == lengths[i]) {
                count++;
            }
        }
        if (count > max_count) {
            max_count = count;
            result = lengths[i];
        }
    }
    return result;
}

std::string ProcessNotes::print_result_helper(int note, int length, int dynamics, int* final_length) {
    int note_length_musically_raw = whole_note_length / length / polling_speed;
    int note_length_musically = round_to_power_of_two(note_length_musically_raw);
    if (note_length_musically <= 0) {
        note_length_musically = 1;
    }
    std::string note_str_terminal = print_result_terminal_helper_note(note);
    std::string note_str_abjad = print_result_abjad_note(note);
    std::string length_str = print_result_terminal_helper_length(note_length_musically);
    std::string dynamics_str = print_result_terminal_helper_dynamics(dynamics);
    std::cout << note_str_abjad << ", " << length_str << " is being pressed " << dynamics_str << std::endl;
    // std::cout << note_str_abjad << ", " << length_str << " is being pressed " << std::endl;
    *final_length = note_length_musically;
    return note_str_abjad + "'";
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
    }
    return result;
}

std::string ProcessNotes::print_result_abjad_note(int note) {
    std::string result;
    switch (note) {
        case 0:
            result += "c";
            break;
        case 1:
            result += "d";
            break;
        case 2:
            result += "e";
            break;
        case 3:
            result += "f";
            break;
        case 4:
            result += "g";
            break;
        case 5:
            result += "a";
            break;
        case 6:
            result += "b";
            break;
        case 7:
            result += "c'";
            break;
        case 8:
            result += "d'";
            break;
        case 9:
            result += "e'";
            break;
        case 10:
            result += "cs";
            break;
        case 11:
            result += "ds";
            break;
        case 12:
            result += "fs";
            break;
        case 13:
            result += "gs";
            break;
        case 14:
            result += "as";
            break;
        case 15: 
            result += "cs'";
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
        case 128:
            result += "hundred-twenty-eighth";
            break;
        case 256:
            result += "two-hundred-fifty-sixth";
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
