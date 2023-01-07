#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <csignal>
#include <netdb.h>

int serverSock;
int clientSock;

void ctrl_c(int) {
    shutdown(clientSock, SHUT_RDWR);
}

void createServer(char * port) {
    addrinfo hints {.ai_flags=AI_PASSIVE, .ai_protocol = IPPROTO_TCP};
    addrinfo *resolved;
    if(int err = getaddrinfo(nullptr, port, &hints, &resolved)) {
        fprintf(stderr, "Resolving address failed: %s\n", gai_strerror(err));
        exit(1);
    }
    serverSock = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    const int one = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if(bind(serverSock, resolved->ai_addr, resolved->ai_addrlen)) {
        fprintf(stderr, "Failed to bind: %s\n", strerror(errno));
        exit(1);
    }
    freeaddrinfo(resolved);
    listen(serverSock, 1);
    clientSock = accept(serverSock, nullptr, nullptr);
    close(serverSock);
    if(clientSock==-1) {
        fprintf(stderr, "Failed to accept: %s\n", strerror(errno));
        exit(1);
    }
}

int main(int argc, char ** argv) {
    if(argc!=2) {
        fprintf(stderr, "Usage:\n%s <port>\n", argv[0]);
        return 1;
    }

    createServer(argv[1]);

    signal(SIGINT, ctrl_c);

    long ctrs[4] {};

    char buf[256], *eol;
    int pos{0};
    
    printf(" |%18ld|%18ld|%18ld|%18ld|\r", ctrs[0], ctrs[1], ctrs[2], ctrs[3]);
    fflush(stdout);
    
    while(true) {
        // dane z sieci zapisz do bufora, zaczynając od miejsca za wcześniej zapisanymi danymi
        int bytesRead = read(clientSock, buf+pos, 255-pos);
        if (bytesRead <= 0)
            break;
        // zaktualizuj ile łącznie danych jest w buforze
        pos+=bytesRead;
        // zapisz znak '\0' na końcu danych 
        buf[pos] = 0;
        
        // dopóki w danych jest znak nowej linii
        while(nullptr != (eol = strchr(buf, '\n'))){
            
            // przeczytaj komendę
            
            char cmd[256] {};
            int index{-1}, value{0};
            sscanf(buf, "%s%d%d", cmd, &index, &value);

            // usuń komendę z bufora
            
            // (pomocnicze) wylicz długość komendy
            int cmdLen = (eol-buf)+1;
            // przesuń pozostałe dane i znak '\0' na początek bufora
            memmove(buf, eol+1, pos-cmdLen+1);
            // zaktualizuj zmienną pamiętającą ile danych jest w buforze
            pos -= cmdLen;
            
            // wykonaj komendę
            
            if(index<0 || index>=4) continue;

            if(strcmp(cmd, "increment") == 0) ctrs[index]+=value;
            if(strcmp(cmd, "decrement") == 0) ctrs[index]-=value;
        
            printf(" |%18ld|%18ld|%18ld|%18ld|\r", ctrs[0], ctrs[1], ctrs[2], ctrs[3]);
            fflush(stdout);
        }
        
        // jeżeli w 255 znakach nie ma '\n', wyjdź.
        if(pos == 255)
            break;
    }

    printf("\n");
    close(clientSock);
    return 0;
}
