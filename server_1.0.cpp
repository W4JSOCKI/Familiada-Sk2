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


using namespace std;

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
    int pointvalue [1001][6];
   
    void loaddata(){ // not sure if it should be in Game class
        
        memset(used_questions,0,sizeof(used_questions));
        
       string line;
       ifstream baza;
       baza.open("Baza.csv");
       for(int i=1; i<1001; i++){
        getline(baza,line);
        cout << line << endl;
        stringstream linestream(line);
        getline(linestream,questions[i],';');
        for(int j=1; j<6; j++){
            getline(linestream,answers[i][j],';');
            string p;
            getline(linestream,p,';');
           // pointvalue[i][j]=stoi(p);
        }
       }
      // baza.close();

    }
    
   int chose_question( ) {

    srand( (unsigned)time( NULL ) );
    int r=1 + (rand() % ( 1000 - 1 + 1 ));
    for(int i=0; i <15; i++){
        if(used_questions[i]==r)
          return chose_question();// chose random nuber again with recurency;
        else if(used_questions[i]==0){
           used_questions[i]=r;
           return r;
        }
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
    
    
    
    int play(){
        if(how_many_players==6){
            cout << "There is " << how_many_players << " players, we need 6 to start" << endl;
            return -1;
        }
        
    }
                                                     
};
int main()
{
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
       // for(int j=1; j<6; j++)
   //     cout << test.answers[i][j] << " " << endl;// test.pointvalue[i][j] << endl;
    }
    return 0;
}
