//player.h
////////////
#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <irrlicht.h>
using namespace irr;
using namespace core;
//using namespace scene;
using namespace video;
using namespace io;
//using namespace std;

class  Player {
public:
    Player();

    //
    void setPlayer();
    void setPlayerName(stringc namec);
    void setPlayerColor(SColor c);
    //Called to see if sinking the 8 ball wins the game.
    //In non stripes/solids a true value means that player has just won
         bool canWin();
    
    //Sets what balls a player has to sink to win the game
         void setPlayerRange(unsigned x1, unsigned x2);    
    //1-7
         void setSolids();
    //9-15
         void setStripes();
    //sets a ball in target array to true
    //Effectivly informing the player that the ball has indeed been sunk
         void updateSunk(unsigned sunk);
         bool isActive();
         void getLeft(bool arr[]);

        unsigned ISunk(unsigned ref);
private:
     bool hasSunk;
     stringc name;    
     bool active;
     SColor color;
 //    bool human; //Allow hot seat gameplay
     bool target[15];
     bool targetSunk[15];
}; 
#endif

