/*

Camera class - cpp

*/

#include "Polaroid.h"


// constructor
cPolaroid::cPolaroid() {
}

// destructor
cPolaroid::~cPolaroid() {
}

// Initialization
void cPolaroid::Initialize(cInit *setInit) {
	
	init = setInit;
	
	camera = init->getCamera();
	
	eventReceiver = init->getEventReceiver();
	
}

// draw Polaroid
void cPolaroid::drawPolaroid(cPlayer *player) {
	
	vector3df playerPos = player->getLookPos();
	vector3df playerLook = player->getLookAt();
	vector3df distancePlayerCam = playerPos - camera->getPosition();
	
	f32 distanceMult=30.f;
	f32 distanceTopMult=15.f;
	f32 playerVelo = player->getPlayerVelocity();

	if (playerVelo > 1.f) {
		distanceMult /= 1.f*playerVelo;
		distanceTopMult /= 1.f*playerVelo;
	}

	playerLook.normalize();
	playerLook *= distanceMult;
	
	vector3df camPosZero = playerPos - 1.5f * playerLook;
	camPosZero.Y += distanceTopMult;

	vector3df camPos = camera->getPosition();
	
	// cam - drift
	if (camPos != camPosZero) {
		vector3df delta = camPos - camPosZero;
		
		camPos = camPos - (.065f * delta);
		
		camera->setPosition(camPos);
	}
	

	camera->setTarget(playerPos);
	
}

// returns camera scene node
ICameraSceneNode *cPolaroid::getCameraSceneNode() {
	return camera;
}

