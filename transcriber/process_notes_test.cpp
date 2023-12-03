#include "process_notes.h"
#include <iostream>
#include <string>

// test cases for process_notes implementation
void test_process_notes1() {
    // instantiate new process_notes
    ProcessNotes process_notes(100, 7, 1, 64);

    // update the process_notes with new notes in a loop
    int noteC[] = {1, 0, 2, 1, 0, 0, 0, 1, 0};
    int noteD[] = {2, 2, 3, 2, 2, 2, 1, 2, 0};
    int noteE[] = {0, 0, 0, 1, 1, 0, 0, 0, 0};
    int noteF[] = {1, 1, 0, 0, 1, 2, 1, 3, 0};
    int noteG[] = {2, 2, 2, 3, 2, 2, 2, 2, 2};
    int noteA[] = {0, 3, 2, 3, 1, 3, 0, 3, 0};
    int noteB[] = {1, 2, 2, 3, 2, 2, 3, 2, 0};

    // generate new notes based on the notes array above
    for (int i = 0; i < 9; i++) {
        std::string new_notes = std::to_string(i + 1) + ",";
        new_notes += std::to_string(0) + ":" + std::to_string(noteC[i]) + ",";
        new_notes += std::to_string(1) + ":" + std::to_string(noteD[i]) + ",";
        new_notes += std::to_string(2) + ":" + std::to_string(noteE[i]) + ",";
        new_notes += std::to_string(3) + ":" + std::to_string(noteF[i]) + ",";
        new_notes += std::to_string(4) + ":" + std::to_string(noteG[i]) + ",";
        new_notes += std::to_string(5) + ":" + std::to_string(noteA[i]) + ",";
        new_notes += std::to_string(6) + ":" + std::to_string(noteB[i]) + ",";

        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "new notes: " << new_notes << std::endl;
        process_notes.update(new_notes);

        // get the processed notes
        std::vector<std::pair<int, int> > notes = process_notes.get_notes();

        std::cout << "got notes " + std::to_string(i) + " : ";
        for (int j = 0; j < notes.size(); j++) {
            std::cout << "(" << notes[j].first << ", " << notes[j].second << ") ";
        }
        std::cout << std::endl;
    }

}

void test_process_notes2() {
    ProcessNotes process_notes(300, 3, 2, 128);
    int noteC[] = {1, 3, 0, 2, 1, 1, 2, 1, 2, 0, 0, 0, 1, 0};
    int noteD[] = {2, 2, 3, 2, 0, 2, 1, 2, 0, 0, 2, 2, 2, 0};
    int noteE[] = {2, 0, 0, 1, 2, 2, 0, 0, 2, 2, 3, 2, 0, 0};

    for (int i = 0; i < 256; i++) {
        int j = i % 14;
        std::string new_notes = std::to_string(i) + ",";
        new_notes += std::to_string(0) + ":" + std::to_string(noteC[j]) + ",";
        new_notes += std::to_string(1) + ":" + std::to_string(noteD[j]) + ",";
        new_notes += std::to_string(2) + ":" + std::to_string(noteE[j]) + ",";

        // std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "new notes: " << new_notes << std::endl;
        // process_notes.update(new_notes);

        // // get the processed notes
        // std::vector<std::pair<int, int> > notes = process_notes.get_notes();

        // std::cout << "got notes " + std::to_string(i) + " : ";
        // for (int j = 0; j < notes.size(); j++) {
        //     std::cout << "(" << notes[j].first << ", " << notes[j].second << ") ";
        // }
        // std::cout << std::endl;
    }
}

int main() {
    test_process_notes2();
    return 0;
}