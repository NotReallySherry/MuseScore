#include "sliding_window.h"
#include <iostream>

void test_sliding_window1() {
    /**********************test case 1******************/
    int note1_arr[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    int note2_arr[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    /**********************test case 2******************/
    // int note1_arr[] = {1, 1, 1, 0, 0, 0, 1, 1, 0, 1};
    // int note2_arr[] = {0, 1, 1, 1, 1, 0, 0, 0, 1, 1};
    /**********************test case 3*****************/
    // int note1_arr[] = {1, 0, 1, 0, 1, 1, 0, 1, 1, 0};
    // int note2_arr[] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
    std::vector<int> note1(note1_arr, note1_arr + sizeof(note1_arr) / sizeof(int));
    std::vector<int> note2(note2_arr, note2_arr + sizeof(note2_arr) / sizeof(int));

    // instantiate new sliding window
    SlidingWindow window(4, 2);
    // update the window with new notes in a loop
    for (int i = 0; i < 10; i++) {
        std::vector<int> new_notes;
        new_notes.push_back(note1[i]);
        new_notes.push_back(note2[i]);
        window.update(new_notes);
        window.print_window();
        std::vector<int> result = window.get_notes();
        ////// print the result /////
        std::cout << "result: ";
        for (int j = 0; j < result.size(); j++) {
            std::cout << result[j] << " ";
        }
        std::cout << std::endl;
        /////////////////////////////
    }
}

void test_sliding_window2(){
    // randomly fill notes array with 0 and 1
    std::vector<int> notes1;
    std::vector<int> notes2;
    for (int i = 0; i < 30; i++) {
        notes1.push_back(rand() % 2);
        notes2.push_back(rand() % 2);
    }
    /////////// print out the notes /////////////
    std::cout << "notes1: ";
    for (int i = 0; i < notes1.size(); i++) {
        std::cout << notes1[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "notes2: ";
    for (int i = 0; i < notes2.size(); i++) {
        std::cout << notes2[i] << " ";
    }
    std::cout << std::endl;
    ////////////////////////////

    SlidingWindow window(10, 5);

    // update the window with new notes in a loop and print out the results
    for (int i = 0; i < 30; i++) {
        std::vector<int> new_notes;
        new_notes.push_back(notes1[i]);
        new_notes.push_back(notes2[i]);
        window.update(new_notes);
        // window.print_window();
        std::vector<int> result = window.get_notes();
        ////// print the result /////
        std::cout << "result: ";
        for (int j = 0; j < result.size(); j++) {
            std::cout << result[j] << " ";
        }
        std::cout << std::endl;
        /////////////////////////////
    }
}

int main() {
    test_sliding_window1();
    return 0;
}