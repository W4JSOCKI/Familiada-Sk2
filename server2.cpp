#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

// A struct to store information about each connected client
struct Client {
  int sockfd;
  std::string nickname;
};

// A global vector to store all connected clients
std::vector<Client> clients;

// A function to read a message from a client
std::string take_answer(int i) {
  char buffer[1024];
  int num_bytes = recv(clients[i].sockfd, buffer, 1024, 0);
  if (num_bytes < 0) {
    std::cerr << "Error receiving message from client" << std::endl;
    return "";
  }
  return std::string(buffer, num_bytes);
}

int main() {
  // Create the server socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error creating socket" << std::endl;
    return 1;
  }

  // Set up the server address
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(3333);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Bind the socket to the address
  if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "Error binding socket" << std::endl;
    return 1;
  }

  // Listen for incoming connections
  listen(sockfd, 5);

  // Enter the main loop
  while (true) {
    // Wait for a new connection
    socklen_t clilen = sizeof(serv_addr);
    int newsockfd = accept(sockfd, (struct sockaddr*) &serv_addr, &clilen);
    if (newsockfd < 0) {
      std::cerr << "Error accepting connection" << std::endl;
      continue;
    }

    // Read the nickname from the client
    char buffer[1024];
    int num_bytes = recv(newsockfd, buffer, 1024, 0);
    if (num_bytes < 0) {
      std::cerr << "Error receiving nickname from client" << std::endl;
      close(newsockfd);
      continue;
    }
    std::string nickname(buffer, num_bytes);

    // Add the new client to the list
    Client new_client;
    new_client.sockfd = newsockfd;
    new_client.nickname = nickname;
    clients.push_back(new_client);

    // Print a message to the console
   
    std::cout << nickname << " with id " << clients.size() - 1 << " just connected" << std::endl;

    // If there are now 6 clients, start the loop
    if (clients.size() == 2) {
      for (int i = 0; i < 2; i++) {
      
      	  // Send the data from the file "ramka.txt" to all clients
        std::ifstream infile("ramka.txt");
        std::string line;
        std::string message;
        while (std::getline(infile, line)) 
        	message+=line;
          for (int j = 0; j < clients.size(); j++) {
            if (send(clients[j].sockfd, message.c_str(), message.size(), 0) < 0) {
              std::cerr << "Error sending message to client" << std::endl;
            }
          }
          infile.close();
        
        
      
      
        // Read the answer from the client
        std::cout << "s1" << std::endl;
        std::string answer = take_answer(i);
        std::cout << "s2" << std::endl;
        std::ofstream answer_file;
        answer_file.open("answertmp.txt");
        answer_file << answer << std::endl;
        answer_file.close();
        }
      }
    }
  

  return 0;
}
