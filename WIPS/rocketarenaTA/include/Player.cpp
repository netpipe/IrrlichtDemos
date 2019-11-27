/*

Player - cpp file

*/

#include "Player.h"


// constructor
cPlayer::cPlayer() {
	playerVelo=0;
	playerRot=0;
	playerYaw=0;
	playerUp=0;
	playerMode=true;
	lastPlayerMode=true;
	lookAt = core::vector3df(0,0,1);
	lookPos = core::vector3df(0,0,0);
	playerJump=false;
	jumpOffRocket=false;

	rocketRot=0;
}

// destructor
cPlayer::~cPlayer() {
}

// Initialize
void cPlayer::Initialize(cInit *setInit) {
	
	init = setInit;
	device = init->getDevice();
	eventReceiver = init->getEventReceiver();

	smgr = device->getSceneManager();

	scene::IAnimatedMesh *mesh = smgr->getMesh("media/player/player.x");
	scene::IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(mesh);
	node->setScale(core::vector3df(3.6f,3.6f,3.6f));
	shadowNode = node->addShadowVolumeSceneNode(1, true);
	
	skeleton.Initialize(node, 7);

	skeletonDummy = smgr->addEmptySceneNode();
	skeletonDummy->addChild( skeleton.getSkeletonSceneNode() );
	
	skeletonDummy->setPosition(skeletonDummy->getPosition()+core::vector3df(-710,-15,309));
	
}

void cPlayer::drawPlayer(cRocket *rocket) {
	
	lookAt.normalize();
	scene::ISceneNode *playerNode = skeleton.getSkeletonSceneNode();
	scene::ISceneNode *rocketNode = rocket->getRocketSceneNode();
	
	// standing on the map
	if (playerMode) {

		if (playerRot!=0.f) {
			playerNode->setRotation(matrixRotation(playerNode->getRotation(), vector3df(0,playerRot/8.5f,0)));
			lookAt.rotateXZBy( -playerRot/8.5f, core::vector3df(0,0,0) );
		}

	// sitting on the rocket	
	} else {

		lookAt = core::vector3df(0,0,1);
		matrix4 matA,matB;
		matA = skeletonDummy->getAbsoluteTransformation();
		matB.setRotationDegrees(core::vector3df(0,180,0));
		matA *= matB;
		matA.rotateVect(lookAt);
		lookAt.normalize();

		// rocket rotation around z
		core::vector3df rot = rocketNode->getRotation();

		rot.X = 5.f*sin(1.5f*playerUp);

		if (rocketRot==0)
			rocketNode->setRotation( core::vector3df(rot.X,rot.Y,0) );
		else
			rocketNode->setRotation( core::vector3df(rot.X,rot.Y,10.f*sin(rocketRot/7.5f)) );
	}

	lookPos = playerNode->getAbsolutePosition();	

}

// returns look vector
const core::vector3df cPlayer::getLookAt() {
	return lookAt;
}

// store new look vector
void cPlayer::setLookAt(core::vector3df set) {
	lookAt = set;
}

// return skeleton motion variable
u32 cPlayer::getSkeletonMotionVariable() {
	return skeleton.getMotionVariable();
}

// setting player mode, rocket/standing
void cPlayer::setPlayerMode(bool newPlayerMode, cRocket *rocket) {
	
	if (newPlayerMode) {
		
		if (playerMode)
			return;
			
		jumpOffRocket=true;

		scene::ISceneNode *rocketNode = rocket->getRocketSceneNode();
		scene::ISceneNode *skeletonNode = skeleton.getSkeletonSceneNode();			

		skeletonDummy->setParent(smgr->getRootSceneNode());

		skeletonNode->setRotation(core::vector3df(0,lookAt.getHorizontalAngle().Y,0));

	} else {
		
		if (!playerMode)
			return;
		
		scene::ISceneNode *skeletonNode = skeleton.getSkeletonSceneNode();
		scene::ISceneNode *rocketNode = rocket->getRocketSceneNode();
		scene::ISceneNode *rocketDummy = rocket->getDummySceneNode();
		
		rocketNode->setRotation(core::vector3df(0,0,0));

		rocketNode->addChild(skeletonDummy);

		skeletonNode->setRotation(core::vector3df(0,rocketNode->getRotation().Y+180.f,0));

		skeleton.setAnimType( CSK_ANIM_ROCKET );

	}

	playerMode=newPlayerMode;
	
}

// return player mode, rocket/standing
const bool cPlayer::getPlayerMode() {
	return playerMode;
}

// setting player mode, rocket/standing
void cPlayer::setLastPlayerMode(bool newLastPlayerMode) {
	lastPlayerMode=newLastPlayerMode;
}
	
// return player mode, rocket/standing
const bool cPlayer::getLastPlayerMode() {
	return lastPlayerMode;
}

// return player velocity
const irr::f32 cPlayer::getPlayerVelocity() {
	return playerVelo;
}

// change player velocity variable
void cPlayer::setPlayerVelocity(f32 velocity) {
	playerVelo=velocity;
}

// return skeleton scene node
scene::IAnimatedMeshSceneNode *cPlayer::getSkeletonSceneNode() {
	return skeleton.getSkeletonSceneNode();
}

// return if player is in jumping
bool cPlayer::getPlayerJump() {
	return playerJump;
}

// change player jump variable
void cPlayer::setPlayerJump(bool jump) {
	playerJump=jump;
}

// return player rotation speed
const s32 cPlayer::getPlayerRot() {
	return playerRot;
}

// change player rotation variable
void cPlayer::setPlayerRot(s32 rot) {
	playerRot=rot;
}

// return character position
const core::vector3df cPlayer::getLookPos() {
	return lookPos;
}

// return skeleton dummy node
scene::ISceneNode *cPlayer::getSkeletonDummy() {
	return skeletonDummy;
}

// return state if player is jumping off the rocket
const bool cPlayer::isPlayerJumpOffRocket() {
	return jumpOffRocket;
}

// set state if player is jumping off the rocket
void cPlayer::setPlayerJumpOffRocket(bool set) {
	jumpOffRocket=set;
}

// return the animation state of player
const CSK_ANIM cPlayer::getPlayerAnimationType() {
	return skeleton.getAnimType();
}
	
// change player animation state
void cPlayer::setPlayerAnimationType(CSK_ANIM animation) {
	skeleton.setAnimType(animation);
}

// return rocket rotation variable
const s32 cPlayer::getRocketRot() {
	return rocketRot;
}
	
// change rocket rotation variable
void cPlayer::setRocketRot(s32 rotation) {
	rocketRot=rotation;
}

// return player up variable
const f32 cPlayer::getPlayerUp() {
	return playerUp;
}
	
// change player up variable
void cPlayer::setPlayerUp(f32 up) {
	playerUp=up;
}

// return shadow scene node
const scene::IShadowVolumeSceneNode *cPlayer::getShadowSceneNode() {
	return shadowNode;
}

// animates the player skeleton
void cPlayer::animatePlayer() {
	skeleton.animSkeleton();
}

