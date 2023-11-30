#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#include "process_notes.h"

int socket_setup(void) {
  int clientSocket;
  struct sockaddr_in serverAddr;

  clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = inet_addr("172.24.57.188");

  if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
    perror("Connect failed: ");
    return -1;
  }
  std::cout << "Connected to the server." << std::endl;

  const char* data_to_send = "Hello from C++!";
  send(clientSocket, data_to_send, strlen(data_to_send), 0);
  return clientSocket;
  /*try {
    while (true) {
      char buffer[1024];
      ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

      if (bytesRead <= 0) {
        std::cerr << "Connection closed by server\n";
        break;
      }

      buffer[bytesRead] = '\0';
      std::cout << "Recieved message: " << buffer << "\n";
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
  }

  close(clientSocket);*/
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
        //std::cin >> notes;
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
          std::cerr << "Connection closed by server\n";
          break;
        }

        buffer[bytesRead] = '\0';
        std::cout << "Recieved message: " << buffer << "\n";
        notes = std::string(buffer);

        // always update the process_notes regardless of pulling cycle
        process_notes.update(notes);

        int cycle = std::stoi(notes.substr(0, notes.find(",")));

        // get the processed notes at the pulling cycle
        if (cycle % process_notes.get_polling_speed() == 0) {
            std::vector<std::pair<int, int> > result = process_notes.get_notes();
            process_notes.print_get_notes_result(result);
        }
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
