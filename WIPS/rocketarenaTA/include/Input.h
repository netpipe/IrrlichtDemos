/*

Input - header file

*/

#ifndef __Input_h
#define __Input_h

#include <irrlicht.h>

using namespace irr;

#include <Init.h>
#include <EventReceiver.h>
#include <Player.h>
#include <Skeleton.h>
#include <Hud.h>


class cInput {
public:	
	
	// constructor
	cInput();
	
	// destructor
	~cInput();
	
	// Initialisation
	void Initialize(cInit *init);
	
	// check for any keyboard/mouse input
	void checkForInput(cPlayer *player, cRocket *rocket, cHud *hud);
	
	
private:
	
	cEventReceiver *eventReceiver;
	
	
};

#endif
