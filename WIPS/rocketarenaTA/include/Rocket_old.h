/*

Rocket class - header

*/

#ifndef __Rocket_h
#define __Rocket_h

#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

#include <Init.h>
#include <BMaths.h>


class cRocket {
public:
	
	// constructor
	cRocket();
	
	// destructor
	~cRocket();
	
	// Initialisation
	void Initialize(cInit *setInit);
	
	// returns dummy scene node
	scene::ISceneNode *getDummySceneNode();
	
	// return rocket scene node
	scene::ISceneNode *getRocketSceneNode();

	
private:
	
	cInit *init;
	IrrlichtDevice *device;
	ISceneManager *smgr;
	IVideoDriver *driver;
	
	ISceneNode *rocketDummy;
	IAnimatedMeshSceneNode *rocket;
	
};

#endif
