/*

Cable - header file

*/

#ifndef __Cable_h
#define __Cable_h

#include <irrlicht.h>

using namespace irr;

#include "Init.h"
#include "BMaths.h"
#include <SceneNodeAnimatorTA.h>
#include "CollisionObjectAABBMesh.h"


class cCable {
public:

	// constructor
	cCable();

	// destructor
	~cCable();

	// Initialization / true=right, false=left
	bool Initialize(cInit *init, bool rightCable);

	// shoot a cable
	void animCable(scene::ISceneNode *rocketNode, core::vector3df lookAt);

	// get motion variable
	irr::u32 getMotionVariable();

	// setting cable back to root position
	void stopCable();

	// return cable scene node
	const scene::ISceneNode *getCableSceneNode();

	// return claw scene node
	const scene::ISceneNode *getClawSceneNode();

	// return visible claw scene node
	const scene::ISceneNode *getClawVisibleSceneNode();

	// set claw TA dynamic object
	void setClawTAController(TA::DynamicObject *controller);

	// return Claw TA dynamic object
	TA::DynamicObject *getClawTAController();

	// get the position of claw
	core::vector3df getClawPosition();

	// draw cable static from one position to another
	void drawCable(scene::ISceneNode *rocketNode, core::vector3df end);


private:

	IrrlichtDevice *device;

	scene::IAnimatedMeshSceneNode *cableNode;
	scene::ISceneNode *clawNode;
	scene::ISceneNode *clawNodeVisible;
	TA::DynamicObject *clawController;

	// storing bone scene nodes
	core::array<scene::IBoneSceneNode*> bones;
	// storing bone oscillations
	core::array<core::vector3df> bonesOsci;
	// storing bone positions
	core::array<core::vector3df> bonesPos;
	// storing bone names
	core::array<irr::c8*> boneNames;
	// storing bone rotations direct after mesh load
	core::array<core::vector3df> bonesRot;

	// motion variable
	irr::u32 i;
	irr::u32 speed;
	irr::f32 direction;

	// enumeration for bones
	enum SKT_BONE {
		SKT_BONE_JOINT,
		SKT_BONE_ROPE,

		// count of bones
		SKT_BONE_COUNT
	};

};

#endif
