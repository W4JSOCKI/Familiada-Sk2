#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <time.h>
#include <queue>
#include <string>


using namespace std;

class Player{
    int id;
    string nickname;
    string ip;
    Player(int newid){
        id=newid;
        nickname="notset";
        ip="notset";
    }
    
    public int setnickname(string newnickname) {
        if(newnickname.lenght()<3)
            return -1;
        else nickname=newnickname;
        return 0;
    }
    public string getnickname(){return nickname};
    public int getid(){return id};
    public string getip(){return ip};
    
    
}

class Game {
    int id;
    short how_many_players;
    vector <Player> players;//0-2 team1 3-5 team 2

    short round;// actual round number 0 if game not started

    string question;
    string responses[5];
    short points[5]// points per response

    short team1points,team2points;
    short team1fails,team2fails;
    short buttonwin;// who won clash at the button 0-during clash 1-team1 2-team2
    short point_multiplier;// first 3 rounds = 1 then goes one up every round;
    short point_treshhold =300; // points needed to win the game
    
    Game(int newid){
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
    
    public int addplayer(Player NewPlayer{
        if(how_many_players < 6){
        players.pushback(NewPlayer);
        how_many_players++;
        return how_many_players;}  
        else
            return -1;
    }
    
    public int removeplayer(int id){
        for(int i =0; i<6;i++){
            if(players[i].getid==id){
                players.erase(i);
                how_many_players--;
                return how_many_players;
            } 
        }
        return -1;
        
    }
                          
                              

}
