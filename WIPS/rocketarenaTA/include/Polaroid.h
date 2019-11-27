/*

Camera class - header

*/

#ifndef __cPolaroid_h
#define __cPolaroid_h

#include <irrlicht.h>

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;

#include <Init.h>
#include <Player.h>
#include <EventReceiver.h>
#include <Rocket.h>


class cPolaroid {
public:
	
	// constructor
	cPolaroid();
	
	// destructor
	~cPolaroid();
	
	// Initializing
	void Initialize(cInit *setInit);
	
	void drawPolaroid(cPlayer *player);
	
	// returns camera scene node
	ICameraSceneNode *getCameraSceneNode();
	
	
private:
	
	cInit *init;

	cEventReceiver *eventReceiver;

	scene::ICameraSceneNode *camera;
	
};

#endif
