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
        std::vector<std::pair<int, int> > result = process_notes.get_notes();
            process_notes.print_get_notes_result(result);
    }
}

int receive_start_signal(int clientSocket) {
    char buffer[1024];
    // continuously receive data from the server until the server sends a start signal
    while (true) {
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            std::cerr << "Connection closed by server\n";
            return -1;
        }

        buffer[bytesRead] = '\0';
        std::string message = std::string(buffer);
        // if the notes string starts with "start"
        if (message.find("start") == 0) {
            // get the substring after "start"
            std::string num = message.substr(6);
            int bpm = std::stoi(num);
            return bpm;
        }
    }
}


int main(int argc, char* argv[]) {

    int window_size = 16384; // how "long" of notes to keep around as data to be used
    int notes_per_window = 16; // now many notes are sticked with sensors

    // create an output file
    std::ofstream outfile;
    outfile.open("cppoutput.txt");

    // set up socket to read from
    int clientSocket = socket_setup();
    if (clientSocket == -1) {
      return 1;
    }

    while (true) {
        // wait for the start signal from the server
        int pulling_speed = receive_start_signal(clientSocket);
        int whole_note_length = 64*pulling_speed; // how many polling cycles means a complete whole note

        std::cout << "***start recording*** pulling_speed: " << pulling_speed << ", whole_note_length: " << whole_note_length << std::endl;

        ProcessNotes process_notes(window_size, notes_per_window, pulling_speed, whole_note_length, outfile);
        // run the code to get piano inputs and processing algorithm
        run_code(process_notes);
        
        // generate the sheet music
        system("python3 gen_music_sheet.py");

        // clear the output file
        std::ofstream ofs;
        ofs.open("cppoutput.txt", std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }

    outfile.close();
    return 0;
}
