/*

Cable - cpp file

*/

#include "Cable.h"


// constructor
cCable::cCable() {
	i=1;
	speed=7;
	direction=1.f;
}

// destructor
cCable::~cCable() {	
}

// Initialization
bool cCable::Initialize(cInit *init, bool rightCable) {
	
	device = init->getDevice();
	scene::ISceneManager *smgr = device->getSceneManager();
	
	for (irr::u32 n=0; n<SKT_BONE_COUNT; n++) {
		bonesOsci.push_back(core::vector3df(0,0,0));
		bonesPos.push_back(core::vector3df(0,0,0));

		switch (n) {
		case SKT_BONE_JOINT:
			
			boneNames.push_back((irr::c8*) "joint");
			break;
			
		case SKT_BONE_ROPE:
			
			boneNames.push_back((irr::c8*) "rope");
			break;	

		}
	}
	
	scene::IAnimatedMesh *mesh = smgr->getMesh("media/cable/cable.x");
	cableNode = smgr->addAnimatedMeshSceneNode(mesh);
	
	if (cableNode) {
		
		// fixing rotations for right and left cable
		if (rightCable)
			direction=1.f;
		else
			direction=-1.f;
		
		cableNode->setPosition(core::vector3df(0,0,0));
		cableNode->setFrameLoop(0,0);
		cableNode->setJointMode(scene::EJUOR_CONTROL);
		cableNode->setMaterialFlag(video::EMF_LIGHTING, false);
		cableNode->setVisible(false);
//		cableNode->addShadowVolumeSceneNode(1, true);
		
		// loading claw mesh
		if (rightCable)
			mesh = smgr->getMesh("media/cable/claw_blue.b3d");
		else
			mesh = smgr->getMesh("media/cable/claw_red.b3d");
			
		clawNode = (scene::ISceneNode*) smgr->addAnimatedMeshSceneNode(mesh);
		clawNode->setMaterialFlag(video::EMF_LIGHTING, false);
		clawNode->setPosition(core::vector3df(0,0,0));
		clawNode->setScale(core::vector3df(.3f,.3f,.3f));
		clawNode->setVisible(false);
		
		clawNodeVisible = (scene::ISceneNode*) smgr->addAnimatedMeshSceneNode(mesh);
		clawNodeVisible->setMaterialFlag(video::EMF_LIGHTING, false);
		clawNodeVisible->setScale(core::vector3df(.7f,.7f,.7f));
		clawNodeVisible->setPosition(core::vector3df(0,0,0));
		clawNodeVisible->setVisible(false);
		
		bonesRot.clear();
		bonesPos.clear();
		for (irr::u32 n=0; n<SKT_BONE_COUNT; n++) {
			bones.push_back(cableNode->getJointNode(boneNames[n]));
			bonesRot.push_back(bones[n]->getRotation());
			bonesPos.push_back(bones[n]->getPosition());
		}
		
	} else
		return false;

	return true;
}

// shoot a cable
void cCable::animCable(scene::ISceneNode *rocketNode, core::vector3df lookAt) {
	
	TA::Physics &physics = TA::Physics::GetInstance();
	physics.SetDynamicObjectIsMoving(clawController);
	
//	clawController->SetMovementDisabled(false);
	cableNode->setVisible(true);
	clawNode->setVisible(true);
	clawNodeVisible->setVisible(true);
	
	// oscillation functions
	u32 a = i%360;
	f32 rad = 3.14159/180;
	f32 const alpha = a * rad;
	f32 const sinB = -pow(alpha-3,2) + 20.f;

	lookAt.Y=0.f;
	lookAt.normalize();
	lookAt.rotateXZBy(-direction*35.f, core::vector3df(0,0,0));
	lookAt*=20.f;
	
	bonesPos[SKT_BONE_ROPE] = core::vector3df(lookAt.X*(sinB),0, lookAt.Z*(sinB) );
	core::vector3df velocity = bonesPos[SKT_BONE_ROPE];
	TA::Vec3 velocityTA = TA::Vec3(velocity.X,velocity.Y,velocity.Z);
	
	clawController->SetLinearVelocity(velocityTA);

	cableNode->setPosition(rocketNode->getAbsolutePosition()+core::vector3df(0,6,0));
	core::vector3df pos;
	pos.X = clawController->GetPosition().x;
	pos.Y = clawController->GetPosition().y;
	pos.Z = clawController->GetPosition().z;
	bones[SKT_BONE_ROPE]->setPosition(pos-rocketNode->getAbsolutePosition()-core::vector3df(0,6,0));

	clawNodeVisible->setPosition(pos);
	pos = bones[SKT_BONE_ROPE]->getPosition()-bones[SKT_BONE_JOINT]->getPosition();
	pos.Y=0.f;
	pos.normalize();
	clawNodeVisible->setRotation(core::vector3df(0,pos.getHorizontalAngle().Y-0.f,0));

	i+=speed;
}

// get motion variable
irr::u32 cCable::getMotionVariable() {
	return i;
}

// setting cable back to root position
void cCable::stopCable() {
	cableNode->setVisible(false);
	clawNode->setVisible(false);
	clawNodeVisible->setVisible(false);
	
	clawController->SetMovementDisabled(true);
	
	i=1;
}

// return cable scene node
const scene::ISceneNode *cCable::getCableSceneNode() {
	return cableNode;
}

// return claw scene node
const scene::ISceneNode *cCable::getClawSceneNode() {
	return clawNode;
}

// set claw TA dynamic object
void cCable::setClawTAController(TA::DynamicObject *controller) {
	clawController = controller;
}

// return Claw TA dynamic object
TA::DynamicObject *cCable::getClawTAController() {
	return clawController;
}

// get the position of claw
core::vector3df cCable::getClawPosition() {
	TA::Vec3 position = clawController->GetPosition();
	return core::vector3df(position.x,position.y,position.z);
}

// draw cable static from one position to another
void cCable::drawCable(scene::ISceneNode *rocketNode, core::vector3df end) {
	cableNode->setVisible(true);
	cableNode->setPosition(rocketNode->getAbsolutePosition()+core::vector3df(0,6,0));
	bones[SKT_BONE_ROPE]->setPosition(end-cableNode->getPosition()-core::vector3df(0,6,0));
}

// return visible claw scene node
const scene::ISceneNode *cCable::getClawVisibleSceneNode() {
	return clawNodeVisible;
}
