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


class Player
{
    int id;
    string nickname;
    string ip;

public:
    Player(int newid)
    {
        id = newid;
        nickname = "notset";
        ip = "notset";
    }

public:
    int setnickname(string newnickname)
    {
        if (newnickname.length() < 3)
            return -1;
        else
            nickname = newnickname;
        return 0;
    }

public:
    string getnickname() { return nickname; }

public:
    int getid() { return id; }

public:
    string getip() { return ip; }
};

class Game
{
    int id;
    short how_many_players;
    vector<Player> players; // 0-2 team1 3-5 team 2

    short round; // actual round number 0 if game not started
    string nicknames[6];
    /*string question;
    string responses[5];
    short points[5];*/

    short temp_points;
    short team1points, team2points;
    short team1fails, team2fails;
    short buttonwin;             // who won clash at the button 0-during clash 1-team1 2-team2
    short point_multiplier;      // first 3 rounds = 1 then goes one up every round;
    short point_treshhold = 100; // points needed to win the game
    int responding;
    short used_questions[15];
    string answers_to_send[6];
    string wronganwers[7];
    int wronganwers_counter = 0;
    int correctanswers[7];

public:
    Game(int newid, string nick1, string nick2, string nick3, string nick4, string nick5, string nick6)
    {
        id = newid;
        how_many_players = 0;
        nicknames[0] = nick1;
        nicknames[1] = nick2;
        nicknames[2] = nick3;
        nicknames[3] = nick4;
        nicknames[4] = nick5;
        nicknames[5] = nick6;
        round = 0;
        point_multiplier = 1;
        team1points = 0;
        team2points = 0;
        team1fails = 0;
        team2fails = 0;
        buttonwin = 0;
    }

public:
    string questions[1001];
    string answers[1001][6];
    int pointvalue[1001][6]; // points for each answer
    string pointstring[1001][6];
    string previous_ans_id = "-1";

    string getanswer(int p_nr) // temp function
    {
        
        string ans;
        string snewid;
        ifstream answer;
	int i =0;
	string pn=to_string(p_nr);

        //debug

        //getline(cin, ans);

        //correct way

        do
        {
            sleep(1);
            answer.open("answertmp.txt");
            getline(answer, ans);
            getline(answer, snewid);
            answer.close();
            i++;
           
           if(i==120)
           {
        ans=pn+" not responded";
        break;
        }
           
        } while (snewid == previous_ans_id);
        previous_ans_id = snewid;
        

            return ans;
    }

    void loaddata()
    { // not sure if it should be in Game class, loads questions answers and points from file to arrays

        memset(used_questions, 0, sizeof(used_questions));

        string line;
        ifstream baza;
        baza.open("Baza.csv");

        for (int i = 1; i < 1001; i++)
        {
            getline(baza, line);

            stringstream linestream(line);
            getline(linestream, questions[i], ';');
            for (int j = 1; j < 6; j++)
            {
                getline(linestream, answers[i][j], ';');
                string p;
                getline(linestream, p, ';');
                pointvalue[i][j] = stoi(p);
                pointstring[i][j] = p;
            }
        }
        baza.close();
    }
    void senddata(int res)
    {
        ofstream file;
        file.open("out.txt");

        for (int i = 0; i < 6; i++)
            file << nicknames[i] << endl;

        file << round << endl;
        file << team1points << endl;
        file << team2points << endl;
        file << team1fails << endl;
        file << team2fails << endl;
        file << res+1 << endl;
        for (int i = 0; i < 6; i++)
            file << answers_to_send[i] << endl;

        for (int i = 0; i < 7; i++)
            file << wronganwers[i] << endl;
        file.close();
    }

    void resetanswerstosend(int q_nr)
    {
        temp_points = 0;
        wronganwers_counter = 0;
        // reset answers
        answers_to_send[0] = questions[q_nr];
        answers_to_send[1] = "1. -----";
        answers_to_send[2] = "2. -----";
        answers_to_send[3] = "3. -----";
        answers_to_send[4] = "4. -----";
        answers_to_send[5] = "5. -----";
        // reset correct answers

        correctanswers[1] = 0;
        correctanswers[2] = 0;
        correctanswers[3] = 0;
        correctanswers[4] = 0;
        correctanswers[5] = 0;

        // reset wrong answers
        wronganwers[0] = "-----";
        wronganwers[1] = "-----";
        wronganwers[2] = "-----";
        wronganwers[3] = "-----";
        wronganwers[4] = "-----";
        wronganwers[5] = "-----";
        wronganwers[6] = "-----";
        wronganwers[7] = "-----";

        // reset fails
        team1fails = 0;
        team2fails = 0;
    }

    int chose_question()
    { // returns unique int in range 1-1000 so we can chose questions from array
        int r = 1 + (rand() % (1000 - 1 + 1));
        for (int i = 0; i < 15; i++)
        {
            if (used_questions[i] == r)
                return chose_question(); // chose random number again with recurency;
            else if (used_questions[i] == 0)
            {
                used_questions[i] = r;
                return r;
            }
            else
                return r;
        }
        return -1;
    }

public:
    int addplayer(Player NewPlayer)
    {
        if (how_many_players < 6)
        {
            players.push_back(NewPlayer);
            nicknames[how_many_players] = NewPlayer.getnickname();
            how_many_players++;

            return how_many_players;
        }
        else
            return -1;
    }

    int removeplayer(int id)
    {
        for (int i = 0; i < 6; i++)
        {
            if (players[i].getid() == id)
            {
                players.erase(players.begin() + i);
                how_many_players--;
                return how_many_players;
            }
        }
        return -1;
    }

    int check_answer(int q_nr, string given_answer)
    { // return response number if correct and -1 if incorrect

        for (int i = 1; i < 6; i++)
        {
            if (given_answer == answers[q_nr][i] && correctanswers[i] != 1)
            {
                temp_points += pointvalue[q_nr][i];
                correctanswers[i] = 1;
                return i;
            }
            else
            {
                cout << given_answer << " " << answers[q_nr][i] << " " << i << " " << correctanswers[i] << endl;
            }
        }
        for (int i = 0; i < 7; i++)
        {
            if (wronganwers[i] == "-----")
            {
                wronganwers[i] = given_answer;
                wronganwers_counter++;
                return -1;
            }
        }

        return -1;
    }

    int reveal_answer(int q_nr, int a_nr)
    {
        if (a_nr == -1)
            return -1;

        char a_c = a_nr + 48;
        string x = "";
        x += a_c;
        answers_to_send[a_nr] = x + ". " + answers[q_nr][a_nr] + " " + pointstring[q_nr][a_nr];
        return a_nr;
    }

    int button_phase(int q_nr)
    { // first phase of the game

        cout << questions[q_nr] << endl;
        cout << q_nr << endl;
        resetanswerstosend(q_nr);
        int clash_player_1 = (round - 1) % 3;
        int clash_player_2 = (round - 1) % 3 + 3;
        buttonwin = 1 + rand() % 2; 

        for (int i = 0; i < 3; i++)
        {
            responding = clash_player_1;
            senddata(responding);
            string a1 = getanswer(clash_player_1);
            responding = clash_player_2;
            senddata(responding);
            string a2 = getanswer(clash_player_2);
            if (a1 != a2)
            {
                int res1 = reveal_answer(q_nr, check_answer(q_nr, a1));
                int res2 = reveal_answer(q_nr, check_answer(q_nr, a2));
                if (res1 == -1 && res2 == -1)
                {   
                    cout << res1 << " " << res2 << endl;
                    clash_player_1 = (clash_player_1 + 1) % 3;
                    clash_player_2 = clash_player_1 + 3;
                }
                else if (res1 != -1 && res1 < res2 && res2 != -1)
                    return 1;
                else if (res2 != -1 && res2 < res1 && res1 != -1)
                    return 2;
                else if (res1 != -1)
                    return 1;
                else if (res2 != -1)
                    return 2;
            }
            else if (reveal_answer(q_nr, check_answer(q_nr, a1)) != -1)
                return 1;

            // responding = clash_player_1;
            // senddata();
            // responding = clash_player_2;
            // senddata();
        }
        return -1;
    }

    int play()
    {
        // if(how_many_players!=6){
        //     cout << "There is " << how_many_players << " players, we need 6 to start" << endl;
        //     return -1;
        // }
        round = 1;
        while (team1points < point_treshhold && team2points < point_treshhold)
        {
            int q_nr = chose_question();
            
            if (button_phase(q_nr) == 1)
            {
                cout << "team 1 takes after button" << endl;
                int responding = 0;
                while (team1fails < 3)
                {
                    senddata(responding);
                    string a1 = getanswer(responding); 
                    if (reveal_answer(q_nr, check_answer(q_nr, a1)) == -1)
                        team1fails++;
                    else
                    {
                        cout << "correct answer for team 1: " << a1 << "team 1 fails: " << team1fails << endl;
                        int j = 0;
                        for (int i = 1; i < 6; i++)
                        {
                            if (correctanswers[i] == 1)
                            {
                                j++;
                            }
                        }
                        if (j == 5)
                        {
                            cout << "team 1 gets " << temp_points << endl;
                            team1points += temp_points;
                            temp_points = 0;
                            cout << "team 1 points " << team1points << " team 2 points " << team2points << endl;

                            break;
                        }
                    }
                    responding = (responding + 1) % 3;
                }
                if (team1fails == 3)
                {
                    cout << "team 2 takes over" << endl;
                    responding = 3;
                    senddata(responding);
                    string a1 = getanswer(3);
                    if (reveal_answer(q_nr, check_answer(q_nr, a1)) != -1)
                    {
                        cout << "team 2 gets " << temp_points << endl;
                        team2points += temp_points;
                        temp_points = 0;
                        cout << "team 1 points " << team1points << " team 2 points " << team2points << endl;
                    }
                    else
                    {
                        cout << "team 1 gets " << temp_points << endl;
                        team1points += temp_points;
                        temp_points = 0;
                        cout << "team 1 points " << team1points << " team 2 points " << team2points << endl;
                    }
                }
                senddata(responding);
            }
            else
            {
                cout << "team 2 takes after button" << endl;
                int responding = 39;
                while (team2fails < 3)
                {
                    senddata(responding);
                    string a1 = getanswer(responding);
                    if (reveal_answer(q_nr, check_answer(q_nr, a1)) == -1)
                        team2fails++;
                    else
                    {
                        cout << "correct anser for team 2: " << a1 << "team 2 fails: " << team2fails << endl;
                        int j = 0;
                        for (int i = 1; i < 6; i++)
                        {
                            if (correctanswers[i] == 1)
                            {
                                j++;
                            }
                        }
                        if (j == 5)
                        {
                            cout << "team 2 gets " << temp_points << endl;
                            team2points += temp_points;
                            temp_points = 0;
                            cout << "team 1 points " << team1points << " team 2 points " << team2points << endl;
                            break;
                        }
                    }
                    responding = (responding + 1) % 3 + 3;
                    senddata(responding);
                }
                if (team2fails == 3)
                {   
                    responding = 0;
                    cout << "team 1 takes over" << endl;
                    senddata(responding);
                    string a1 = getanswer(0);
                    if (reveal_answer(q_nr, check_answer(q_nr, a1)) != -1)
                    {
                        cout << "team 1 gets " << temp_points << endl;
                        team1points += temp_points;
                        temp_points = 0;
                        cout << "team 1 points " << team1points << " team 2 points " << team2points << endl;
                    }
                    else
                    {
                        cout << "team 2 gets " << temp_points << endl;
                        team2points += temp_points;
                        temp_points = 0;
                        cout << "team 1 points " << team1points << " team 2 points " << team2points << endl;
                    }
                }
                senddata(responding);
            }
            round++;
        }

        if (team1points > team2points)
        {
            cout << "Team 1 wins" << endl;
            responding = -1;
        }
        else
        {
            cout << "Team 2 wins" << endl;
        }

        return 0;
    }
};

int main(int argc, char *argv[])
{
    srand(time(NULL));
   Game test(1, argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
    //Game test(1, "Hutnor", "szbanek", "Irssus", "Pandzica", "Voyager", "Makary");
    // Player player1(1);
    // Player player2(2);
    // Player player3(3);
    // Player player4(4);
    // Player player5(5);
    // Player player6(6);
    // player1.setnickname("Hutnor");
    // player2.setnickname("szbanek");
    // player3.setnickname("Irssus");
    // player4.setnickname("Pandzica");
    // player5.setnickname("Voyager");
    // player6.setnickname("Makary");
    // cout << test.addplayer(player1) << endl;
    // cout << test.addplayer(player2) << endl;
    // test.addplayer(player3);
    // test.addplayer(player4);
    // test.addplayer(player5);
    // test.addplayer(player6);
    test.loaddata();
    test.play();

    // for(int i=1; i<50; i++)
    // {
    //     cout << test.questions[i] << endl;
    //     for(int j=1; j<6; j++)
    //    cout << test.answers[i][j] << " " <<  test.pointvalue[i][j] << endl;
    // }//checking if database loads correctly
    return 0;
}
