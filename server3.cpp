#include <iostream>
#include <vector>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>

const int MAX_CLIENTS = 6;
const int PORT = 3333;
const int MAX_EVENTS = MAX_CLIENTS + 1;
const std::string FILENAME = "answer.txt";

void save_to_file(int client_fd) {
    std::ofstream file;
    file.open("answer.txt", std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    // read data from client_fd
    char buffer[1024] = {0};
    int valread = read(client_fd, buffer, 1024);
    if (valread < 0) {
        std::cerr << "Failed to read data from client" << std::endl;
        return;
    }
    file << buffer;
    file.close();
}


int main()
{
    int server_fd, client_fd, epoll_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    std::vector<int> clients;
    struct epoll_event event;
    struct epoll_event *events;

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Failed to create server socket" << std::endl;
        return 1;
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "Failed to set socket options" << std::endl;
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Failed to bind socket to address" << std::endl;
        return 1;
    }

    // Listen for connections
    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        std::cerr << "Failed to listen for connections" << std::endl;
        return 1;
    }

    // Create epoll file descriptor
    if ((epoll_fd = epoll_create1(0)) < 0)
    {
        std::cerr << "Failed to create epoll file descriptor" << std::endl;
        return 1;
    }

    event.data.fd = server_fd;
    event.events = EPOLLIN;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) < 0)
    {
        std::cerr << "Failed to add server socket to epoll" << std::endl;
        return 1;
    }

    events = (epoll_event *)calloc(MAX_EVENTS, sizeof event);

    while (true)
    {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; i++)
        {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
            {
                std::cerr << "Error on epoll wait" << std::endl;
                close(events[i].data.fd);
                continue;
            }
            else if (server_fd == events[i].data.fd)
            {
                // New connection
                while (clients.size() < MAX_CLIENTS && (client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) >= 0)
                {
                    std::cout << "Assigned unique ID " << clients.size() + 1 << " to new client" << std::endl;
                    clients.push_back(client_fd);
                    event.data.fd = client_fd;
                    event.events = EPOLLIN | EPOLLET;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0)
                    {
                        std::cerr << "Failed to add client to epoll" << std::endl;
                        return 1;
                    }
                }
                if (client_fd < 0)
                {
                    std::cerr << "Failed to accept connection" << std::endl;
                    return 1;
                }
            }
            else
            {
                // Data from client
                int done = 0;
                int client_index = -1;
                for (int j = 0; j < clients.size(); j++)
                {
                    if (clients[j] == events[i].data.fd)
                    {
                        client_index = j;
                        break;
                    }
                }
                if (client_index < 0)
                {
                    std::cerr << "Error finding client index" << std::endl;
                    return 1;
                }
                save_to_file(events[i].data.fd);
                if (done)
                {
                    std::cout << "Closing connection with client " << client_index + 1 << std::endl;
                    close(events[i].data.fd);
                }
            }
        }
    }
    free(events);
    close(server_fd);
    return 0;
}