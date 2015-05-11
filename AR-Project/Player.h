#include <iostream>

#include "Dice.h"
using namespace std;

class Player {
  
	int score;
	int totalPoint;
  public:
    
	Player();
	int getScore();
	int sumValues (int, int);
	void rollDice();
	void setTotalPoint(int);
	int getTotalPoint();
};


 Player::Player(){
	 score = 0;
	 totalPoint=0;
 }
 
 void Player::rollDice () {

	 Dice *dice1 = new Dice();
	 
	 score = dice1->roll();
	 cout<<"rolldice: score :"<<score<<endl;
}


int Player::sumValues (int x1, int x2){
 return x1 + x2;
}
int Player::getScore()
{
	return score;
};	
void Player::setTotalPoint(int point)
{
	totalPoint=point;
};
int Player::getTotalPoint()
{
	return totalPoint;
};