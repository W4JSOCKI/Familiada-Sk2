#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <poll.h> 
#include <thread>
#include <fcntl.h>
#include <iostream>
using namespace std;

ssize_t readData(int fd, char * buffer, ssize_t buffsize){
	auto ret = read(fd, buffer, buffsize);
	cout << "read from " << fd << endl;
	if(ret==-1) error(1,errno, "read failed on descriptor %d", fd);
	return ret;
}

void writeData(int fd, char * buffer, ssize_t count){
	auto ret = write(fd, buffer, count);
	cout << "write to " << fd << endl;
	if(ret==-1) error(1, errno, "write failed on descriptor %d", fd);
	if(ret!=count) error(0, errno, "wrote less than requested to descriptor %d (%ld/%ld)", fd, count, ret);
}

int main(int argc, char ** argv){
	if(argc!=3) error(1,0,"Need 2 args");
	
	// Resolve arguments to IPv4 address with a port number
	addrinfo *resolved, hints={.ai_flags=0, .ai_family=AF_INET, .ai_socktype=SOCK_STREAM};
	int res = getaddrinfo(argv[1], argv[2], &hints, &resolved);
	if(res || !resolved) error(1, 0, "getaddrinfo: %s", gai_strerror(res));
	
	// create socket
	int sock = socket(resolved->ai_family, resolved->ai_socktype, 0);
	if(sock == -1) error(1, errno, "socket failed");
	cout << "socket" << endl;
	// attept to connect
	res = connect(sock, resolved->ai_addr, resolved->ai_addrlen);
	if(res) error(1, errno, "connect failed");
	
	// free memory
	freeaddrinfo(resolved);
	
/****************************/
	while(1){
	cout << "read from stdin, write to socket" << endl;
	ssize_t bufsize2 = 255, received2;
	char buffer2[bufsize2];
	int fdi=open("in.txt",0);
	received2 = readData(fdi, buffer2, bufsize2);
	writeData(sock, buffer2, received2);
	cout << "XD" << endl;
	
	
	
	
	 cout << "read from socket, write to stdout" << endl;
	ssize_t bufsize1 = 255, received1;
	char buffer1[bufsize1];
	received1 = readData(sock, buffer1, bufsize1);
	writeData(1, buffer1, received1);
	
/****************************/
	
	
/****************************/
	}
	close(sock);
	
	return 0;
}
