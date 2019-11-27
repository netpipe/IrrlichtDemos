/*

Rocket class - cpp

*/

#include "Rocket.h"


// constructor
cRocket::cRocket() {
	stillShootingRight = false;
	stillShootingLeft = false;
	rightCableLinked = false;
	leftCableLinked = false;
	distanceRocketPole = 0.f;
	engine=false;
}

// destructor
cRocket::~cRocket() {
}

// Initialisation
bool cRocket::Initialize(cInit *init) {

	device = init->getDevice();
	smgr = device->getSceneManager();
	driver = device->getVideoDriver();

	IAnimatedMesh *mesh = (IAnimatedMesh*) smgr->getMesh("media/rocket/rocket.b3d");
	smgr->getMeshManipulator()->scaleMesh(mesh, core::vector3df(1.7,1.7,1.7));
	rocketNode = (IAnimatedMeshSceneNode*) smgr->addAnimatedMeshSceneNode(mesh);

	rocketDummy = smgr->addEmptySceneNode();

	if (rocketNode) {

//		rocketNode->setDebugDataVisible(scene::EDS_BBOX);

		cableRight.Initialize(init, true);
		cableLeft.Initialize(init, false);

//		rocketNode->addShadowVolumeSceneNode(1, true);
//		rocketNode->setVisible(false);

		effect.createRocketWasteGas(device);
		scene::IParticleSystemSceneNode *ps = (scene::IParticleSystemSceneNode*) effect.getPS();
		ps->setVisible(false);
		
//		effect.createFireEffect(device);

		rocketDummy->addChild( rocketNode );

	} else
		return false;

	return true;
}

// returns dummy scene node
scene::ISceneNode *cRocket::getDummySceneNode() {
	return rocketDummy;
}

// return rocket scene node
scene::ISceneNode *cRocket::getRocketSceneNode() {
	return rocketNode;
}

// draw rocket
void cRocket::drawRocket(core::vector3df lookAt) {

	if (!rightCableLinked) {

		if (stillShootingRight) {
			cableRight.animCable(rocketNode, lookAt);

			if (cableRight.getMotionVariable() % 360 > 350) {
				stillShootingRight=false;
				cableRight.stopCable();
			}

		}
	} else {
		cableRight.drawCable(rocketNode, core::vector3df(polePosition.X,rocketNode->getAbsolutePosition().Y+12.f,polePosition.Z));
	}

	if (!leftCableLinked) {

		if (stillShootingLeft) {
			cableLeft.animCable(rocketNode, lookAt);

			if (cableLeft.getMotionVariable() % 360 > 350) {
				stillShootingLeft=false;
				cableLeft.stopCable();
			}

		}
	} else {
		cableLeft.drawCable(rocketNode, core::vector3df(polePosition.X,rocketNode->getAbsolutePosition().Y+12.f,polePosition.Z));
	}
	
	// handling rocket effects/gas/smoke
	scene::IParticleSystemSceneNode *ps = (scene::IParticleSystemSceneNode*) effect.getPS();	
	if (engine) {
		f32 rocketRot = rocketNode->getRotation().Z;
		f32 mult;
		
		if (rocketRot < 0.f) {
			mult=-1.f;
			rocketRot*=-1.f;
		} else if (rocketRot > 0.f)
			mult=1.f;
		else
			mult=0.f;
			
		core::vector3df lookAtBackup = lookAt;
		lookAtBackup.rotateXZBy(mult*90.f, core::vector3df(0,0,0));
		lookAtBackup.normalize();

		ps->setPosition(rocketNode->getAbsolutePosition()
						+core::vector3df(0,9.f,0)
						-.08f*rocketRot*lookAtBackup);

		lookAt.rotateXZBy(180,core::vector3df(0,0,0));
		lookAt.normalize();
	
		scene::IParticleEmitter *em = (scene::IParticleEmitter*) effect.getEM();
		em->setDirection(lookAt*.4f+core::vector3df(0,-1.f*lookAt.Y,0));
		em->setDirection(.5f*em->getDirection());
		ps->setVisible(true);
	} else
		ps->setVisible(false);
		
}

// shoot right cable out
void cRocket::shootRightCable() {
	if (rightCableLinked) {
		rightCableLinked=false;
		stillShootingRight=false;
		cableRight.stopCable();
	} else {
		if (stillShootingRight==true)
			return;
		
		stillShootingRight=true;
		
		core::vector3df pos = rocketNode->getAbsolutePosition();
		TA::DynamicObject *controller = cableRight.getClawTAController();
		controller->Clear();
		controller->SetMovementDisabled(false);
		controller->SetPosition(TA::Vec3(pos.X,pos.Y+6.f,pos.Z));
	}
}
	
// shoot left cable out
void cRocket::shootLeftCable() {
	if (leftCableLinked) {
		leftCableLinked=false;
		stillShootingLeft=false;
		cableLeft.stopCable();
		
	} else {
		if (stillShootingLeft==true)
			return;
		
		stillShootingLeft=true;
		
		core::vector3df pos = rocketNode->getAbsolutePosition();
		TA::DynamicObject *controller = cableLeft.getClawTAController();
		controller->Clear();
		controller->SetMovementDisabled(false);
		controller->SetPosition(TA::Vec3(pos.X,pos.Y+6.f,pos.Z));
	}
}

// return right claw scene node
const scene::ISceneNode *cRocket::getRightClawSceneNode() {
	return cableRight.getClawSceneNode();
}
	
// return left claw scene node
const scene::ISceneNode *cRocket::getLeftClawSceneNode() {
	return cableLeft.getClawSceneNode();
}

// setting claw TA controller
void cRocket::setClawTAController(bool right, TA::DynamicObject *clawController) {

	if (right)
		cableRight.setClawTAController(clawController);
	else
		cableLeft.setClawTAController(clawController);	
	
}

// check if cable is out
bool cRocket::isCableOut() {
	if (stillShootingRight)
		return true;

	if (stillShootingLeft)
		return true;
		
	return false;
}

// check if right cable is out
bool cRocket::isRightCableOut() {
	return stillShootingRight;
}
	
// check if left cable is out
bool cRocket::isLeftCableOut() {
	return stillShootingLeft;
}

// get right cable as line3d
core::line3d<f32> cRocket::getRightCable() {
	core::line3d<f32> line;
	line.setLine(rocketNode->getAbsolutePosition(), cableRight.getClawPosition());
	return line;
}
	
// get left cable as line3d
core::line3d<f32> cRocket::getLeftCable() {
	core::line3d<f32> line;
	line.setLine(rocketNode->getAbsolutePosition(), cableLeft.getClawPosition());
	return line;
}

// set, if cable is linked to pole
void cRocket::setCableLinked(bool right, bool set, core::vector3df position) {
	scene::ISceneNode *node;
	if (right) {
		rightCableLinked=set;
		node = (scene::ISceneNode*) cableRight.getClawSceneNode();
		node->setVisible(false);
		node = (scene::ISceneNode*) cableRight.getClawVisibleSceneNode();
		node->setVisible(false);
	} else {
		leftCableLinked=set;
		node = (scene::ISceneNode*) cableLeft.getClawSceneNode();
		node->setVisible(false);
		node = (scene::ISceneNode*) cableLeft.getClawVisibleSceneNode();
		node->setVisible(false);
	}
	
	polePosition=position;
}

// return if right cable is linked
bool cRocket::isRightCableLinked() {
	return rightCableLinked;
}
	
// return if left cable is linked
bool cRocket::isLeftCableLinked() {
	return leftCableLinked;
}

// return true if left or right cable is linked
bool cRocket::isAnyCableLinked() {
	if (leftCableLinked)
		return true;
	if (rightCableLinked)
		return true;
		
	return false;
}

// set the distance (radius) between linked rocket and pole
void cRocket::setDistanceRocketPole(f32 dist) {
	distanceRocketPole=dist;
}

// get distance (radius) between linked rocket and pole
const f32 cRocket::getDistanceRocketPole() {
	return distanceRocketPole;
}

// setting last velocity for the rocket
void cRocket::setLastVelocity(f32 velocity) {
	lastVelocity = velocity;
}
	
// returns last veloctiy
const f32 cRocket::getLastVelocity() {
	return lastVelocity;
}

// return linked pole position
const core::vector3df cRocket::getLinkedPolePosition() {
	return polePosition;
}

// return if the engine is on
const bool cRocket::isEngineOn() {
	return engine;
}
	
// change engine variable
void cRocket::setEngineOn(bool set) {
	engine=set;
	scene::IParticleSystemSceneNode *ps = (scene::IParticleSystemSceneNode*) effect.getPS();
	
	if (!engine) {
		rightCableLinked=false;
		stillShootingRight=false;
		cableRight.stopCable();
	
		leftCableLinked=false;
		stillShootingLeft=false;
		cableLeft.stopCable();
		
		ps->setVisible(false);
	} else
		ps->setVisible(true);
}
