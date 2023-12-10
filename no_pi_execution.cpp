#include <iostream>
#include <string>
#include <fstream>

#include "process_notes.h"

/**
** polling speed: how many polling cycle per update on notes
change polling cycle to timestamp (time since epoch)
*/


void run_code(class ProcessNotes process_notes) {
    // read in command line arguments
    std::string notes;
    std::cin >> notes;
    while (notes != "quit") {
        // always update the process_notes regardless of pulling cycle
        process_notes.update(notes);

        int cycle = std::stoi(notes.substr(0, notes.find(",")));

        // get the processed notes at the pulling cycle
        if (cycle % process_notes.get_polling_speed() == 0) {
            std::vector<std::pair<int, int> > result = process_notes.get_notes();
            process_notes.print_get_notes_result(result);
        }
        
        std::cin >> notes;
    }
}


int main() {
    int window_size; // how "long" of notes to keep around as data to be used
    int notes_per_window; // now many notes are sticked with sensors
    int pulling_speed; // how many ms until update the notes onto the screen
    int whole_note_length; // how many polling cycles means a complete whole note
    
    std::cin >> window_size >> notes_per_window >> pulling_speed >> whole_note_length;

    if (whole_note_length % pulling_speed != 0 || whole_note_length / pulling_speed < 64) {
        std::cout << "invalid configuration input" << std::endl;
        return 1;
    }
    std::cout << "notes_per_window: " << notes_per_window << ", pulling_speed: " << pulling_speed << ", window_size: " << window_size << ", whole_note_length: " << whole_note_length << std::endl;

    // create an output file
    std::ofstream outfile;
    outfile.open("cppoutput.txt");

    ProcessNotes process_notes(window_size, notes_per_window, pulling_speed, whole_note_length, outfile);

    run_code(process_notes);

    outfile.close();
    return 0;
}