/*

World class - header

*/

#ifndef __World_h
#define __World_h

#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

#include <Init.h>
#include <Player.h>
#include <Flag.h>
#include <LightBillboard.h>


class cWorld {
public:
	
	// constructor
	cWorld();
	
	// destructor
	~cWorld();
	
	// Initialisation
	void Initialize(cInit *init);
	
	// returns the level
	IAnimatedMeshSceneNode *getLevel();
	
	// returns level scene node
	scene::IAnimatedMeshSceneNode *getLevelSceneNode();

	// return level scaleFactor
	const core::vector3df getScaleFactor();
	
	// add flags
	void addFlags(core::array<core::vector3df> positionArray);
	
	// animates the world
	void drawWorld(scene::ICameraSceneNode *camera);


private:
	
	IrrlichtDevice *device;
	ISceneManager *smgr;
	IVideoDriver *driver;
	
	scene::IAnimatedMeshSceneNode *level;
	IAnimatedMesh *levelMesh;
	core::vector3df scaleFactor;
	
	core::array<cFlag> flags;
	
	cLightBillboard lightBillboard;

	scene::IBillboardSceneNode *light1;
	scene::IBillboardSceneNode *light2;
	
	scene::IAnimatedMeshSceneNode *prop;
	
};

#endif
