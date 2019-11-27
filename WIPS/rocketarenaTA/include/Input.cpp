/*

Input - cpp file

*/

#include "Input.h"


// constructor
cInput::cInput() {
}
	
// destructor
cInput::~cInput() {
}
	
// Initialisation
void cInput::Initialize(cInit *init) {
	eventReceiver=init->getEventReceiver();
}

// check for any keyboard/mouse input
void cInput::checkForInput(cPlayer *player, cRocket *rocket, cHud *hud) {

	bool playerMode = player->getPlayerMode();
	bool playerJump = player->getPlayerJump();
	
	f32 playerVelo = player->getPlayerVelocity();
	s32 playerRot = player->getPlayerRot();
	f32 playerUp = player->getPlayerUp();
	CSK_ANIM animation = player->getPlayerAnimationType();
	
	s32 rocketRot = player->getRocketRot();
	u32 radarMode = hud->getRadarMode();
	
	scene::ISceneNode *rocketNode = rocket->getRocketSceneNode();
	core::vector3df rocketPos = rocketNode->getAbsolutePosition();

	// standing on the map
	if (playerMode) {

		if (eventReceiver->Keys[KEY_KEY_W]) {
		
			if (eventReceiver->Keys[KEY_SHIFT]) {
			
				if (playerVelo < .6f)
					playerVelo += .1f;
				else
					playerVelo -= .1f;
			
				animation=CSK_ANIM_WALK;
			} else {
				
				if (playerVelo < 1.2f)
					playerVelo += .1f;
				
				animation=CSK_ANIM_RUN;
			}

		} else {

			if (playerVelo > 0)
				playerVelo -= .1;
			else
				playerVelo = 0;

			animation=CSK_ANIM_STAND;

		}
	
		if (eventReceiver->checkKey(KEY_SPACE) && playerJump==false) {
			animation=CSK_ANIM_JUMP;
			playerJump = true;
		} else
			playerJump = false;
	
		if (eventReceiver->Keys[KEY_RIGHT] || eventReceiver->Keys[KEY_LEFT]) {
			(eventReceiver->Keys[KEY_RIGHT]) ? (playerRot=12) : (playerRot=-12);
		} else
			playerRot = 0;

		
	// sitting on the rocket
	} else {
		
		
		if (eventReceiver->Keys[KEY_RIGHT] || eventReceiver->Keys[KEY_LEFT]) {
			
			if (eventReceiver->Keys[KEY_RIGHT]) {
				playerRot=12;
				
				if (rocketRot < 10)
					rocketRot += 1;
			} else {
				playerRot=-12;
				
				if (rocketRot > -10)
					rocketRot -= 1;
			}
			
		} else {
			playerRot = 0;

			if (rocketRot > 0)
				rocketRot -= 1;
			
			if (rocketRot < 0)
				rocketRot += 1;
		}
			
		if (eventReceiver->Keys[KEY_KEY_W] || rocket->isEngineOn()) {
		
			if (playerVelo < 1.f)
				playerVelo += .1f;

			if (eventReceiver->Keys[KEY_KEY_W]) {
				if (playerVelo < 1.5f)
					playerVelo += .1f;
			} else {
				if (playerVelo > 1.f)
					playerVelo -= .1f;
			}
				
			rocket->setEngineOn(true);

		} else {

			if (playerVelo > 0)
				playerVelo -= .1f;
			else
				playerVelo = 0;

		}
		
		
		if (eventReceiver->Keys[KEY_DOWN] && rocket->isAnyCableLinked()==false) {
			
			if (playerUp < 1.f && rocket->isEngineOn())
				playerUp += .1f;
			
		} else if (eventReceiver->Keys[KEY_UP] && rocket->isAnyCableLinked()==false) {
			
			if (playerUp > -1.f && rocket->isEngineOn())
				playerUp -= .1f;
				
		} else {
			
			if (playerUp > 0)
				playerUp -= .1f;

			if (playerUp < 0)
				playerUp += .1f;
				
		}
		
		if (eventReceiver->checkKey(KEY_KEY_A))
			rocket->shootLeftCable();
			
		if (eventReceiver->checkKey(KEY_KEY_D))
			rocket->shootRightCable();

		animation=CSK_ANIM_ROCKET;

		if (eventReceiver->checkKey(KEY_KEY_Q)) {
			player->setPlayerMode(true, rocket);
			player->setPlayerAnimationType(CSK_ANIM_JUMP_OFF_ROCKET);
			rocket->setEngineOn(false);
			return;
		}
	}

	if (eventReceiver->checkKey(KEY_F2))
		(radarMode==2) ? (radarMode=0) : (radarMode++);
	hud->setRadarMode(radarMode);
	
	player->setPlayerMode(playerMode, rocket);
	player->setPlayerJump(playerJump);
	
	player->setPlayerVelocity(playerVelo);
	player->setPlayerRot(playerRot);
	player->setPlayerUp(playerUp);
	
	if (player->getPlayerAnimationType()!=CSK_ANIM_JUMP_OFF_ROCKET)
		player->setPlayerAnimationType(animation);
		
	if (animation==CSK_ANIM_ROCKET)
		hud->setHudPlayerMode(HUD_PLAYER_ROCKET);
	else if (animation==CSK_ANIM_STAND)
		hud->setHudPlayerMode(HUD_PLAYER_STAND);
	else
		hud->setHudPlayerMode(HUD_PLAYER_WALK);

	player->setRocketRot(rocketRot);
	
}


