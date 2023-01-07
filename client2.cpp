#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
  // Read the nickname from the file
  std::ifstream infile("nickname.txt");
  std::string nickname;
  std::getline(infile, nickname);
  infile.close();

  // Create the socket
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

  // Connect to the server
  if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "Error connecting to server" << std::endl;
    return 1;
  }

  // Send the nickname to the server
  if (send(sockfd, nickname.c_str(), nickname.size(), 0) < 0) {
    std::cerr << "Error sending nickname to server" << std::endl;
    return 1;
  }

  // Enter the main loop
  while (true) {
    // Read the next message from the file
    std::ifstream infile("answer.txt");
    std::string message;
    std::getline(infile, message);
    infile.close();

    // Send the message to the server
    if (send(sockfd, message.c_str(), message.size(), 0) < 0) {
      std::cerr << "Error sending message to server" << std::endl;
      return 1;
    }

    // Wait for a response from the server
    char buffer[1024];
    int num_bytes = recv(sockfd, buffer, 1024, 0);
    if (num_bytes < 0) {
      std::cerr << "Error receiving message from server" << std::endl;
      return 1;
    }

    // Save the response to a file
    std::ofstream outfile("in.txt");
    outfile.write(buffer, num_bytes);
    outfile.close();
  }

  return 0;
}