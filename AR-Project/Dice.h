#include <iostream>
#include <ctime>

using namespace std;

class Dice {
    int result;
  public:
    int roll();    
};

 
 
int Dice::roll () {
 srand(time(NULL));
 for(int i=0; i<100; i++){
 result = (rand() % 6) + 1;
 }
 return result;
}
