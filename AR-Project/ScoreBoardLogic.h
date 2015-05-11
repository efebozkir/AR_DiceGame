#include <iostream>
#include "Player.h"

using namespace std;

class ScoreBoardLogic {
private:

    int player1Score;
    int player2Score;
    int round;
    static const int numberOfRounds = 3;

  public:
	Player *player1;
    Player *player2; 
	ScoreBoardLogic();
    void updateScoreBoard();
    int comparePlayers(int, int);
	int finishGame();
	int getRound();
	void setRound(int value);
	int getPlayer1Score();
	int getPlayer2Score();

};

 ScoreBoardLogic::ScoreBoardLogic(){

	  player1 = new Player();
	  player2 = new Player();
	  player1Score = 0;
	  player2Score = 0;
	  round = 0;

}

 void ScoreBoardLogic::updateScoreBoard () { 
	int result =  comparePlayers(player1->getScore(),player2->getScore());  
    if(result == 1)
	{
		player1Score++;     
	}
	else if(result == 2)
	{
	    player2Score++;    
	}
   else 
   {  
		   return;
	}
	 round++;


   if(round == numberOfRounds)
   {
	   if(player2Score>player1Score)
		   player2->setTotalPoint(player2->getTotalPoint()+1);
	   
	   else if(player2Score<player1Score)
		   player1->setTotalPoint(player1->getTotalPoint()+1);

		finishGame();
   }
}

int ScoreBoardLogic::comparePlayers(int p1, int p2){
  if( p1> p2)
    return 1;
  else if(p1 < p2)
    return 2;
  else 
    return 0;
}

int ScoreBoardLogic::getRound()
{
	return round;
}

int ScoreBoardLogic::getPlayer1Score()
{
	return player1Score;
}

int ScoreBoardLogic::getPlayer2Score()
{
	return player2Score;
}

void ScoreBoardLogic::setRound(int value)
{
	round = value;
}

int ScoreBoardLogic::finishGame(){
	player2Score=0;
	player1Score = 0;
	round =0;
  return comparePlayers(player1Score,player2Score);
}