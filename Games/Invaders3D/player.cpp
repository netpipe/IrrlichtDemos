//player.cpp
////////////////
#include "player.h"

Player::Player(){
   unsigned i;
   for(i = 0; i < 15;i++){
      target[i] = false;
      targetSunk[i] = false;  
   }
   active = false;
   hasSunk = false;
}    

void Player::setPlayer(){
     //Any other player init code should go here
           //Set default name
           //Set default color
     active = true;
     return;
}
bool Player::isActive(){
     return active;
}
void Player::setPlayerName(stringc namec){
     //Check for invalid names
     //Set that shiznet
     name = namec;
     return;
}
void Player::getLeft(bool arr[]){
    for(unsigned i = 0; i < 15; i++){
       if(target[i] == true && targetSunk[i] == false)
          arr[i] = false;
       else
          arr[i] = true;
    }
}

void Player::setPlayerColor(SColor c){
     //Check for invalid colors
             //Implement me
     //Set that shiznet
          color = c;
     return;
}


void Player::setPlayerRange(unsigned x1, unsigned x2){
     for(unsigned i = x1;i < (x2+1);i++)
         target[i] = true;
     return;
}

void Player::setStripes(){
     setPlayerRange(8,14);
     return;
}

void Player::setSolids(){
    setPlayerRange(0,6);
    return;
}

void Player::updateSunk(unsigned sunk){
     if(active == true)
        targetSunk[sunk] = true;
}              

//Scratch Code list
//0 - Everythings A.O.K.
//1 - Scratch on the 8 ball

unsigned Player::ISunk(unsigned ref){
     if(hasSunk == false){
        if(ref <= 6)
           setSolids();
        else if (ref >= 8)
           setStripes();
        else if (ref == 8)
           return 1;

        hasSunk = true;
     }
     //Else check for scratch (8 is a scratch
     
     return 0;
        
}

bool Player::canWin(){
  if(active == true){
      unsigned i;
      for(i = 0; i < 15; i++){
         if(target[i] == true)
           break;
         else if(i == 14)
           return false;
      }
      for(;i < 15; i++){
         if(target[i] == true)
            if(targetSunk[i]==false)
               return false;
      }
      return true;
  }
  return false;
}
