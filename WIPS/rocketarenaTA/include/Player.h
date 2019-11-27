/*

Player - header file

*/

#ifndef __Player_h
#define __Player_h

#include <irrlicht.h>

using namespace std;
using namespace irr;
using namespace scene;

#include <Init.h>
#include <EventReceiver.h>
#include <Skeleton.h>
#include <BMaths.h>
#include <Rocket.h>


class cPlayer {
public:
	
	// constructor
	cPlayer();
	
	// destructor
	~cPlayer();
	
	// Initialize
	void Initialize(cInit *setInit);
	
	// updating position/rotation of the player
	void drawPlayer(cRocket *rocket);
	
	// returns look vector
	const core::vector3df getLookAt();
	
	// store new look vector
	void setLookAt(core::vector3df set);
	
	// returns player position
//	core::vector3df cPlayer::getDummyPosition();
	
	// return skeleton motion variable
	u32 getSkeletonMotionVariable();
	
	// setting player mode, rocket/standing
	void setPlayerMode(bool newPLayerMode, cRocket *rocket);
	
	// return player mode, rocket/standing
	const bool getPlayerMode();

	// setting player mode, rocket/standing
	void setLastPlayerMode(bool newLastPlayerMode);
	
	// return player mode, rocket/standing
	const bool getLastPlayerMode();
	
	// return player velocity
	const f32 getPlayerVelocity();
	
	// change player velocity variable
	void setPlayerVelocity(f32 velocity);
	
	// return skeleton scene node
	scene::IAnimatedMeshSceneNode *getSkeletonSceneNode();
	
	// return if player is in jumping
	bool getPlayerJump();

	// change player jump variable
	void setPlayerJump(bool jump);
	
	// return player rotation speed
	const s32 getPlayerRot();
	
	// change player rotation variable
	void setPlayerRot(s32 rot);
	
	// return character position
	const core::vector3df getLookPos();
	
	// return skeleton dummy node
	scene::ISceneNode *getSkeletonDummy();
	
	// return state if player is jumping off the rocket
	const bool isPlayerJumpOffRocket();
	
	// set state if player is jumping off the rocket
	void setPlayerJumpOffRocket(bool set);
	
	// return the animation state of player
	const CSK_ANIM getPlayerAnimationType();
	
	// change player animation state
	void setPlayerAnimationType(CSK_ANIM animation);
	
	// return rocket rotation variable
	const s32 getRocketRot();
	
	// change rocket rotation variable
	void setRocketRot(s32 rotation);
	
	// return player up variable
	const f32 getPlayerUp();
	
	// change player up variable
	void setPlayerUp(f32 up);
	
	// return shadow scene node
	const scene::IShadowVolumeSceneNode *getShadowSceneNode();
	
	// animates the player skeleton
	void animatePlayer();
	
	
private:
	
	cInit *init;
	IrrlichtDevice *device;
	scene::ISceneManager *smgr;
	cEventReceiver *eventReceiver;
	
	cSkeleton skeleton;
	scene::ISceneNode *skeletonDummy;
	scene::IShadowVolumeSceneNode *shadowNode;
	
	// player velocity
	f32 playerVelo;
	s32 playerRot;
	u32 playerYaw;
	f32 playerUp;
	
	// rocket
	s32 rocketRot;
	
	// look direction
	core::vector3df lookAt;
	// look position
	core::vector3df lookPos;

	// mode rocket or map
	bool playerMode;
	bool lastPlayerMode;
	
	bool playerJump;
	
	bool jumpOffRocket;
	
};

#endif
