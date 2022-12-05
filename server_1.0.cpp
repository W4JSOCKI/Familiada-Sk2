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

class Game {
    int id;
    short how_many_players;
    string [8] nicknames;//0-3 team1 4-7 team 2

    short round;// actual round number 0 if game not started

    string question;
    short how_many_responses;

    string responses[9];
    short points[9]// points per response

    short team1points,team2points;


}