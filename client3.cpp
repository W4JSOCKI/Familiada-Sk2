#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

using namespace std;
int PORT = 3333;
bool answered = false;

void send_file_to_server(int sock, const string &filename)
{
    std::ifstream file;
    file.open(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    file.seekg(0, file.end);
    int file_size = file.tellg();
    file.seekg(0, file.beg);
    char *file_buffer = new char[file_size];
    file.read(file_buffer, file_size);
    int sent_bytes = 0;
    while (sent_bytes < file_size)
    {
        sent_bytes += send(sock, file_buffer + sent_bytes, file_size - sent_bytes, 0);
    }
    file.close();
    delete[] file_buffer;
}

int recv_data(int sockfd, char *buf, size_t size) {
    int total = 0;
    while (total < size - 1) {
        int n = recv(sockfd, buf + total, 1, 0);
        if (n == -1) {
            return -1;
        } else if (n == 0) {
            break;
        } else {
            total += n;
            if (buf[total - 1] == '\0') {
                break;
            }
        }
    }
    buf[total] = '\0';
    
    return total;
}

bool compareLines(string filename)
{
string line1, line13;
ifstream file(filename);
getline(file, line1);


for (int i = 2; i <= 12; i++)
{
    getline(file, line13);
}


getline(file, line13);

file.close();


return line1 == line13;
}

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }

    send_file_to_server(sock, "nickname.txt");
    string answer;
    ifstream filein("answer.txt");
    getline(filein, answer);
    filein.close();
    while (1)
    {
        if (answered)
        {
            if (!compareLines("in.txt"))
            {
                answered = false;
            }
        }
        else if (compareLines("in.txt")) 
        {

            debug << "waiting for client.py" << endl;

            for (int i = 0; i<75; i++)
      
            {
                sleep(1);
                filein.open("answer.txt");
                filein.seekg(0, filein.beg);
                string tmp;
                getline(filein, tmp);
                if (answer != tmp)
                {
                    send_file_to_server(sock, "answer.txt");
                    answer = tmp;
                    answered = true;
                    filein.close();
                    break;
                }
                filein.close();
            }
        }
        
        std::ofstream file;
        char buffer[1024] = {0};
        int valread;
        if ((valread = recv(sock, buffer, sizeof(buffer), 0)) > 0)
        {
            file.open("in.txt", std::ios::binary | std::ios::out | std::ios::trunc);
            if (!file.is_open())
            {
                std::cerr << "Failed to open file" << std::endl;
                return 1;
            }
            file.write(buffer, valread);
        }
        file.close();
        
    }
    return 0;
}
