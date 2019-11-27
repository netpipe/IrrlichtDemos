/*

Rocket class - cpp

*/

#include "Rocket.h"


// constructor
cRocket::cRocket() {
}

// destructor
cRocket::~cRocket() {
}

// Initialisation
void cRocket::Initialize(cInit *setInit) {
	
	init = setInit;
	device = init->getDevice();
	smgr = device->getSceneManager();
	driver = device->getVideoDriver();
	
	IAnimatedMesh *mesh = (IAnimatedMesh*) smgr->getMesh("media/rocket/rocket2.b3d");
	smgr->getMeshManipulator()->scaleMesh(mesh, core::vector3df(1.7,1.7,1.7));
	rocket = (IAnimatedMeshSceneNode*) smgr->addAnimatedMeshSceneNode(mesh);
	
	if (rocket) {
//		rocket->setPosition( rocket->getPosition() + vector3df(-0,30,-140) );
		
//		rocket->addShadowVolumeSceneNode(1, false , 1.f);
		
		rocketDummy = smgr->addEmptySceneNode();
//		rocketDummy->addChild( rocket );

//		rocketDummy->setRotation( matrixRotation(rocketDummy->getRotation(),core::vector3df(0,-140,0)) );
//		rocketDummy->setPosition( core::vector3df(-315,0,-130) );

	}
	
}

// returns dummy scene node
scene::ISceneNode *cRocket::getDummySceneNode() {
	return rocketDummy;
}

// return rocket scene node
scene::ISceneNode *cRocket::getRocketSceneNode() {
	return rocket;
}
