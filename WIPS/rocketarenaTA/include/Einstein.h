/*

Einstein class - header

*/

#ifndef __Einstein_h
#define __Einstein_h

#include <irrlicht.h>

#include <SceneNodeAnimatorTA.h>
#include "CollisionObjectAABBMesh.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

#include <Player.h>
#include <Rocket.h>


class cEinstein {
public:
	
	// constructor
	cEinstein();
	
	// destructor
	~cEinstein();
	
	// Initialisation
	void Initialize();
	
	// create trueaxis static object of the level
	TA::StaticObject *createLevelTA(TA::Physics& physics, IAnimatedMesh *imag, core::vector3df offset);
	
	// create trueaxis dynamic object for player
	void *createPlayerTA(ISceneNode *dummyNode, ISceneNode *skeletonNode);
	
	// converts irrlicht vector3d in trueaxis vec3
	TA::Vec3 convertIrrlichtPos(irr::core::vector3df pos);
	
	// create trueaxis dynamic object for rocket
	void *createRocketTA(ISceneNode *dummyNode, ISceneNode *rocketNode);
	
	// create player/rocket TA
	void createPlayerRocketTA(cPlayer *player, cRocket *rocket);
	
	// disable player/rocket TA
	void disconnectPlayerRocketTA(cPlayer *player);
	
	// return dynamic object character
	TA::DynamicObject *getCharacterController();
	
	// return dynamic object rocket
	TA::DynamicObject *getRocketController();
	
	// create cable claw TA
	void createClawTA(cRocket *rocket);
	
	// create pole TA
	TA::StaticObject *createPoleTA(TA::Physics &physics, scene::IAnimatedMesh *imag, core::vector3df position);
	
	// create dynamic object
	TA::DynamicObject *createDynamicObject(scene::ISceneNode *node1, scene::ISceneNode *node2);
	
	
private:
	
	TA::DynamicObject *characterController;
	TA::DynamicObject *rocketController;
	
};

#endif
