Here is an example of a C++ client that connects to a server, receives a file, and writes it to "in.txt":

Copy code
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>

void send_file_to_server(int sock) {
    std::ifstream file;
    file.open("answer.txt", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    file.seekg(0, file.end);
    int file_size = file.tellg();
    file.seekg(0, file.beg);
    char *file_buffer = new char[file_size];
    file.read(file_buffer, file_size);
    int sent_bytes = 0;
    while (sent_bytes < file_size) {
        sent_bytes += send(sock, file_buffer + sent_bytes, file_size - sent_bytes, 0);
    }
    file.close();
    delete[] file_buffer;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }
    std::ofstream file;
    file.open("in.txt", std::ios::binary | std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }
    char buffer[1024] = {0};
    int valread;
    while ((valread = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        file.write(buffer, valread);
    }
    file.close();
    send_file_to_server(sock);
    close(sock);
    return 0;
}