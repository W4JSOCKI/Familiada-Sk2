#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <csignal>
#include <netdb.h>
#include <thread>
#include <atomic>
#include <random>

int sock;

std::atomic<bool> quit{false};

void ctrl_c(int) {
    quit = true;
}

void connectToHost(const char * host, const char * port) {
    addrinfo hints {.ai_protocol = IPPROTO_TCP};
    addrinfo *resolved;
    if(int err = getaddrinfo(host, port, &hints, &resolved)) {
        fprintf(stderr, "Resolving address failed: %s\n", gai_strerror(err));
        exit(1);
    }
    sock = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    if(connect(sock, resolved->ai_addr, resolved->ai_addrlen)) {
        fprintf(stderr, "Failed to connect: %s\n", strerror(errno));
        exit(1);
    }
    freeaddrinfo(resolved);
}

void doWork() {
    std::mt19937 gen((std::random_device()()));
    std::uniform_int_distribution<uint8_t> distCtr(0, 3);
    std::uniform_int_distribution<uint8_t> distVal(0, 10);
    std::uniform_int_distribution<uint8_t> distTime(0, 10);
    //std::uniform_int_distribution<int32_t> distTime(10*1000, 20*1000);

    char buf[255];

    while (!quit) {
        int ctrIdx = distCtr(gen);
        int ctrVal = distVal(gen);

        std::this_thread::sleep_for(std::chrono::microseconds(distTime(gen)));
        // na końcu każdej wiadomości jest wysyłany dodatkowo znak nowej linii
        sprintf(buf, "increment %d %d\n", ctrIdx, ctrVal);
        if ((int)strlen(buf)!= write(sock, buf, strlen(buf))) return;

        std::this_thread::sleep_for(std::chrono::microseconds(distTime(gen)));
        sprintf(buf, "decrement %d %d\n", ctrIdx, ctrVal);
        if ((int)strlen(buf)!= write(sock, buf, strlen(buf))) return;
    }
}

int main(int argc, char ** argv) {
    if(argc !=3) {
        fprintf(stderr, "Usage:\n%s <ip> <port>\n", argv[0]);
        return 1;
    }

    connectToHost(argv[1], argv[2]);

    signal(SIGINT, ctrl_c);
    
    std::thread threads[9];
    for(auto &t : threads)
        t = std::thread(doWork);
    doWork();
    for(auto &t : threads)
        t.join();

    const linger lv {.l_onoff=1, .l_linger=60};
    setsockopt(sock, SOL_SOCKET, SO_LINGER, &lv, sizeof(lv));
    
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return 0;
}
