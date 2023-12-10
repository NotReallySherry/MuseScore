#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#include "process_notes.h"

/**
** polling speed: how many polling cycle per update on notes
change polling cycle to timestamp (time since epoch)
*/

int socket_setup(void) {
    int clientSocket;
    struct sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("169.254.88.83");

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        perror("Connect failed: ");
        return -1;
    }
    std::cout << "Connected to the server." << std::endl;

    const char* data_to_send = "h";
    send(clientSocket, data_to_send, strlen(data_to_send), 0);
    return clientSocket;
}

void run_code(class ProcessNotes process_notes) {
    // read in command line arguments
    std::string notes;
    //std::cin >> notes;
    int clientSocket = socket_setup();
    if (clientSocket == -1) {
      return;
    }

    char buffer[1024];
    while (notes != "quit") {
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            std::cerr << "Connection closed by server\n";
            break;
        }

        buffer[bytesRead] = '\0';
        notes = std::string(buffer);

        // always update the process_notes regardless of pulling cycle
        process_notes.update(notes);

        long cycle = std::stol(notes.substr(0, notes.find(",")));

        // get the processed notes at the pulling cycle
        if (cycle % process_notes.get_polling_speed() == 0) {
            std::vector<std::pair<int, int> > result = process_notes.get_notes();
            process_notes.print_get_notes_result(result);
        }
    }
}


int main(int argc, char* argv[]) {

    int window_size; // how "long" of notes to keep around as data to be used
    int notes_per_window; // now many notes are sticked with sensors
    int pulling_speed; // how many ms until update the notes onto the screen
    int whole_note_length; // how many polling cycles means a complete whole note
    for (size_t i = 1; i < argc; ++i) {
        std::string sint(argv[i]);
        switch (i) {
            case 1:
              window_size = std::stoi(sint);
              break;
            case 2:
              notes_per_window = std::stoi(sint);
              break;
            case 3:
              pulling_speed = std::stoi(sint);
              break;
            case 4:
              whole_note_length = std::stoi(sint);
              break;
        }
    }

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
