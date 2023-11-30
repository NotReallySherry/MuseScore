#include <iostream>
#include <string>

#include "process_notes.h"


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
    int window_size, notes_per_window, pulling_speed;
    std::cin >> window_size >> notes_per_window >> pulling_speed;

    std::cout << "notes_per_window: " << notes_per_window << ", pulling_speed: " << pulling_speed << ", window_size: " << window_size << std::endl;

    ProcessNotes process_notes(100, 7, 1);

    run_code(process_notes);

    return 0;
}