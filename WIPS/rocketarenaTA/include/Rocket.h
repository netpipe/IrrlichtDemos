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
#include <Cable.h>

#include <SceneNodeAnimatorTA.h>
#include "CollisionObjectAABBMesh.h"
#include <Effect.h>


class cRocket {
public:
	
	// constructor
	cRocket();
	
	// destructor
	~cRocket();
	
	// Initialisation
	bool Initialize(cInit *setInit);
	
	// returns dummy scene node
	scene::ISceneNode *getDummySceneNode();
	
	// return rocket scene node
	scene::ISceneNode *getRocketSceneNode();
	
	// draw rocket
	void drawRocket(core::vector3df lookAt);
	
	// shoot right cable out
	void shootRightCable();
	
	// shoot left cable out
	void shootLeftCable();
	
	// return right claw scene node
	const scene::ISceneNode *getRightClawSceneNode();
	
	// return left claw scene node
	const scene::ISceneNode *getLeftClawSceneNode();
	
	// setting claw TA controller
	void setClawTAController(bool right, TA::DynamicObject *clawController);
	
	// check if cable is out
	bool isCableOut();
	
	// check if right cable is out
	bool isRightCableOut();
	
	// check if left cable is out
	bool isLeftCableOut();
	
	// get right cable as line3d
	core::line3d<f32> getRightCable();
	
	// get left cable as line3d
	core::line3d<f32> getLeftCable();
	
	// set, if cable is linked to pole
	void setCableLinked(bool right, bool set, core::vector3df position);
	
	// return if right cable is linked
	bool isRightCableLinked();
	
	// return if left cable is linked
	bool isLeftCableLinked();
	
	// return true if left or right cable is linked
	bool isAnyCableLinked();
	
	// set the distance (radius) between linked rocket and pole
	void setDistanceRocketPole(f32 dist);

	// get distance (radius) between linked rocket and pole
	const f32 getDistanceRocketPole();
	
	// setting last velocity for the rocket
	void setLastVelocity(f32 velocity);
	
	// returns last veloctiy
	const f32 getLastVelocity();
	
	// return linked pole position
	const core::vector3df getLinkedPolePosition();
	
	// return if the engine is on
	const bool isEngineOn();
	
	// change engine variable
	void setEngineOn(bool set);
	
	
private:
	
	IrrlichtDevice *device;
	scene::ISceneManager *smgr;
	video::IVideoDriver *driver;

	// rocket effects
	cEffect effect;
	scene::IParticleEmitter *effectGas;
	
	// is the engine on
	bool engine;
	
	scene::ISceneNode *rocketDummy;
	scene::IAnimatedMeshSceneNode *rocketNode;
	
	cCable cableLeft;
	cCable cableRight;
	
	bool stillShootingRight;
	bool stillShootingLeft;
	
	bool rightCableLinked;
	bool leftCableLinked;
	
	// store distance between linked rocket and pole
	f32 distanceRocketPole;
	
	f32 lastVelocity;
	
	// position of the linked pole
	core::vector3df polePosition;
	
};

#endif
