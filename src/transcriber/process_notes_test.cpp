#include <fstream>
#include <iostream>
#include "process_notes.cpp"

void test_process_notes(){
    std::ifstream test_file("testing_data.txt");
    std::string line;
    while(std::getline(test_file, line)){
        std::cout << line << std::endl;
        std::vector<processedNotes> result = process_notes(line);
        for (int i = 0; i < result.size(); i++){
            std::cout << result[i].noteNumber << " " << result[i].duration << std::endl;
        }
    }
}

int main(){
    test_process_notes();
    return 0;
}