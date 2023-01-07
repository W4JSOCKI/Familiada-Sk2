#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <time.h>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <unordered_set>
#include <signal.h>
#include <error.h>


using namespace std;

class Client;

int servFd;
int epollFd;
std::unordered_set<Client*> clients;

void ctrl_c(int);

void sendToAllBut(int fd, char * buffer, int count);

uint16_t readPort(char * txt);

void setReuseAddr(int sock);

struct Handler {
    virtual ~Handler(){}
    virtual void handleEvent(uint32_t events) = 0;
};

class Client : public Handler {
    int _fd;
public:
    Client(int fd) : _fd(fd) {
        epoll_event ee {EPOLLIN|EPOLLRDHUP, {.ptr=this}};
        epoll_ctl(epollFd, EPOLL_CTL_ADD, _fd, &ee);
    }
    virtual ~Client(){
        epoll_ctl(epollFd, EPOLL_CTL_DEL, _fd, nullptr);
        shutdown(_fd, SHUT_RDWR);
        close(_fd);
    }
    int fd() const {return _fd;}
    virtual void handleEvent(uint32_t events) override {
        if(events & EPOLLIN) {
            // klient coś chce
            char buffer[256];
            ssize_t count = read(_fd, buffer, 256);
            if(count > 0)
                sendToAllBut(_fd, buffer, count);
            else
                events |= EPOLLERR;
        }
        if(events & ~EPOLLIN){
            remove();
        }
    }
    void write(char * buffer, int count){
        if(count != ::write(_fd, buffer, count))
            remove();
        
    }
    void remove() {
        printf("removing %d\n", _fd);
        clients.erase(this);
        delete this;
    }
};

class : Handler {
    public:
    virtual void handleEvent(uint32_t events) override {
        if(events & EPOLLIN){
            sockaddr_in clientAddr{};
            socklen_t clientAddrSize = sizeof(clientAddr);
            
            auto clientFd = accept(servFd, (sockaddr*) &clientAddr, &clientAddrSize);
            if(clientFd == -1) error(1, errno, "accept failed");
            
            printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
            
            clients.insert(new Client(clientFd));
        }
        if(events & ~EPOLLIN){
            error(0, errno, "Event %x on server socket", events);
            ctrl_c(SIGINT);
        }
    }
} servHandler;

int main(int argc, char ** argv){
    if(argc != 2) error(1, 0, "Need 1 arg (port)");
    auto port = readPort(argv[1]);
    
    servFd = socket(AF_INET, SOCK_STREAM, 0);
    if(servFd == -1) error(1, errno, "socket failed");
    
    signal(SIGINT, ctrl_c);
    signal(SIGPIPE, SIG_IGN);
    
    setReuseAddr(servFd);
    
    sockaddr_in serverAddr{.sin_family=AF_INET, .sin_port=htons((short)port), .sin_addr={INADDR_ANY}};
    int res = bind(servFd, (sockaddr*) &serverAddr, sizeof(serverAddr));
    if(res) error(1, errno, "bind failed");
    
    res = listen(servFd, 1);
    if(res) error(1, errno, "listen failed");

    epollFd = epoll_create1(0);
    
    epoll_event ee {EPOLLIN, {.ptr=&servHandler}};
    epoll_ctl(epollFd, EPOLL_CTL_ADD, servFd, &ee);
    
    while(true){
        if(-1 == epoll_wait(epollFd, &ee, 1, -1)) {
            error(0,errno,"epoll_wait failed");
            ctrl_c(SIGINT);
        }
        ((Handler*)ee.data.ptr)->handleEvent(ee.events);
    }
}

uint16_t readPort(char * txt){
    char * ptr;
    auto port = strtol(txt, &ptr, 10);
    if(*ptr!=0 || port<1 || (port>((1<<16)-1))) error(1,0,"illegal argument %s", txt);
    return port;
}

void setReuseAddr(int sock){
    const int one = 1;
    int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if(res) error(1,errno, "setsockopt failed");
}

void ctrl_c(int){
    for(Client * client : clients)
        delete client;
    close(servFd);
    printf("Closing server\n");
    exit(0);
}

void sendToAllBut(int fd, char * buffer, int count){
    auto it = clients.begin();
    while(it!=clients.end()){
        Client * client = *it;
        it++;
        if(client->fd()!=fd)
            client->write(buffer, count);
    }
}



class Player{
    int id;
    string nickname;
    string ip;

  public:  Player(int newid){
        id=newid;
        nickname="notset";
        ip="notset";
    }
    
    public: int setnickname(string newnickname) {
        if(newnickname.length()<3)
            return -1;
        else nickname=newnickname;
        return 0;
    }
    public: string getnickname(){return nickname;}
    public:  int getid(){return id;}
    public: string getip(){return ip;}
    
    
};

class Game {
    int id;
    short how_many_players;
    vector <Player> players;//0-2 team1 3-5 team 2

    short round;// actual round number 0 if game not started
    string nicknames[6];
    string question;
    string responses[5];
    short points[5];// points per response
    
    short temp_points;
    short team1points,team2points;
    short team1fails,team2fails;
    short buttonwin;// who won clash at the button 0-during clash 1-team1 2-team2
    short point_multiplier;// first 3 rounds = 1 then goes one up every round;
    short point_treshhold =300; // points needed to win the game
    short used_questions [15];
    
  public:  Game(int newid){
        id=newid;
        how_many_players=0;
        for(int i=0; i<6;i++)
            nicknames[i]="0";
        round=0;
        point_multiplier=1;
        team1points=0;
        team2points=0;
        team1fails=0;
        team2fails=0;
        buttonwin=0;
         
    }
    public:
    string questions [1001];
    string answers [1001][6];
    int pointvalue [1001][6];// points for each answer
   
    void loaddata(){ // not sure if it should be in Game class, loads questions answers and points from file to arrays
        
        memset(used_questions,0,sizeof(used_questions));
        
       string line;
       ifstream baza;
       baza.open("C:/Programowanie/Familiada-Sk2/Baza.csv");
    
       
       for(int i=1; i<1001; i++){
        getline(baza,line);
     
        stringstream linestream(line);
        getline(linestream,questions[i],';');
        for(int j=1; j<6; j++){
            getline(linestream,answers[i][j],';');
            string p;
            getline(linestream,p,';');
           pointvalue[i][j]=stoi(p);
        }
       }
      baza.close();

    }
    
   int chose_question( ) {// returns unique int in range 1-1000 so we can chose questions from array
        int r=1 + (rand() % ( 1000 - 1 + 1 ));
        for(int i=0; i <15; i++){
            if(used_questions[i]==r)
                return chose_question();// chose random number again with recurency;
            else if(used_questions[i]==0){
                used_questions[i]=r;
                return r;
            }
            else return r;
        }
   
    }   
    
    public: int addplayer(Player NewPlayer){
        if(how_many_players < 6){
        players.push_back(NewPlayer);
        how_many_players++;
        return how_many_players;}  
        else
            return -1;
    }
    
    int removeplayer(int id){
        for(int i =0; i<6;i++){
            if(players[i].getid()==id){
                players.erase(players.begin()+i);
                how_many_players--;
                return how_many_players;
            } 
        }
        return -1;
    }

    int take_answer(int player_n){// return response number if correct and -1 if incorrect

        ifstream a_file;
        a_file.open("Answers.txt");//TODO take answer from socket

        string given_answer;
        getline(a_file,given_answer);

        for(int i=0; i<5;i++){
            if(given_answer==responses[i]){
                temp_points+=points[i];
                return i;
            }
        }

        return -1;


    }

    int button_phase(){// first phase of the game

        int clash_player_1= (round-1)%3;
        int clash_player_2= (round-1)%3+3;
        buttonwin=1+rand()%2;// TODO (which player press button first)

        

    }
    
    
    
    int play(){
        if(how_many_players==6){
            cout << "There is " << how_many_players << " players, we need 6 to start" << endl;
            return -1;
        }
        
    }
                                                     
};


/*int main()
{   srand( time( NULL ) );
    Game test(1);
    Player player1(1);
    Player player2(2);
    player1.setnickname("Hutnor");
    player2.setnickname("szbanek");
    cout << test.addplayer(player1) << endl;
    cout << test.addplayer(player2) << endl;
    test.loaddata();

   

   
    for(int i=1; i<50; i++)
    {
        cout << test.questions[i] << endl;
        for(int j=1; j<6; j++)
       cout << test.answers[i][j] << " " <<  test.pointvalue[i][j] << endl;
    }//checking if database loads correctly
    return 0;
}*/


