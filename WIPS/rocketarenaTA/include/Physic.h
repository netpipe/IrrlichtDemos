/*

Physic class - header

*/

#ifndef __Physic_h
#define __Physic_h

#include <irrlicht.h>
#include <iostream>

using namespace irr;

#include <Einstein.h>
#include <Init.h>
#include <World.h>
#include <Player.h>
#include <EventReceiver.h>
#include <Rocket.h>
#include <Pole.h>

#include <SceneNodeAnimatorTA.h>
#include "CollisionObjectAABBMesh.h"


class cPhysic {
public:
	
	// constructor
	cPhysic();
	
	// destructor
	~cPhysic();
	
	// Initialisation
	void Initialize(cInit *setInit);
	
	// creating collision between world/player/rocket
	void createWorld(cWorld *level, cPlayer *player, cRocket *rocket);
	
	// start physic world
	void beginPhysic(cPlayer *player, cRocket *rocket);
	
	// end physic world
	void endPhysic();
	
	// fire cube
	void fireCube();
	
	// returns position of all poles
	const core::array<core::vector3df> getPolePositionArray();
	
	// return pole class
	cPole *getPoles();
	
	
private:

	cInit *init;
	IrrlichtDevice *device;
	scene::ISceneManager *smgr;
	
	cEinstein einstein;
	cEventReceiver *eventReceiver;

	cPole poles;

	CSceneNodeAnimatorTA *animatorTA;

	scene::ISceneNode* cube;
	
	bool playerStillJumping;

	// variable for passing elapsed time to TA physics
	u32 previewTime;
	
	// check for high acceleration, store last amount of velocity
	f32 abc;

};

#endif
