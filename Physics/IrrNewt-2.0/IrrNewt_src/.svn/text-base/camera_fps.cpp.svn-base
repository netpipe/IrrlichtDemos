// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/world.hpp"
#include "../include/utils.hpp"
#include "../include/camera_fps.hpp"
#include "../include/hidden.hpp"
using namespace irr;

irr::newton::ICameraBodyFPS::ICameraBodyFPS(irr::newton::ICharacterController* c_base):ICharacterController(c_base) {
	this->class_type.Type=ECT_CAMERA_FPS_BODY;

	this->up=false;
	this->down=false;
	this->left=false;
	this->right=false;
	this->verticalMovement=true;
	this->onlyY=false;
	this->isPaused=false;
	this->timePauseStop=0;

	this->moveCallback = NULL;

	this->mouseSensitive = 50.0f;
	this->moveSpeed = 6.0f;

	//very important for a correct mouse calculation
	this->center_mouse = false;

	//no rotation update
	this->setRotationUpdate(false);

	//restore default callbacks
	//this->restoreCallbacks();

	//rotation constraint on all aces
	this->getWorld()->getUtils()->avoidRotationOnAllAxes(this);

	//build default key map
	SKeyMap keyMap[4];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;

    keyMap[1].Action = EKA_MOVE_BACKWARD;
    keyMap[1].KeyCode = KEY_DOWN;

    keyMap[2].Action = EKA_STRAFE_LEFT;
    keyMap[2].KeyCode = KEY_LEFT;

    keyMap[3].Action = EKA_STRAFE_RIGHT;
    keyMap[3].KeyCode = KEY_RIGHT;

	this->setKeyMap(keyMap,4);
}

//event receiver

bool irr::newton::ICameraBodyFPS::OnEvent(const irr::SEvent& event) {


	if(event.EventType==EET_KEY_INPUT_EVENT ) {

		//PRESSED DOWN
		if(event.KeyInput.PressedDown==true) {

			for(irr::u32 i=0;i<this->key_array.size();i++) {
				if(event.KeyInput.Key==this->key_array[i].KeyCode) this->pressedDown(this->key_array[i].Action);
			}//for

		}//pressed down

		//LEFT UP

		else if(event.KeyInput.PressedDown==false) {

			for(int i=0;i<this->key_array.size();i++) {
				if(event.KeyInput.Key==this->key_array[i].KeyCode) this->leftUp(this->key_array[i].Action);
			}//for

		}//pressed down

	}//event type==key input
	//-----end of event caught

	if(this->moveCallback)
		this->moveCallback(up, down, left, right,
		up || down || left || right  //is moving
		);

	/*
	this->getWorld()->getUtils()->updateCameraFPS(
		this,
		this->mouseSensitive,
		this->moveSpeed,
		irr::newton::SDirection(this->up,this->down,this->left,this->right),
		this->vertical_movement,
		this->only_y
		);
		*/

	return false;
}

void irr::newton::ICameraBodyFPS::pressedDown(EKEY_ACTION action) {

	switch(action) {

	case EKA_MOVE_FORWARD:
		this->up=true;
		break;

	case EKA_MOVE_BACKWARD:
		this->down=true;
		break;

	case EKA_STRAFE_LEFT:
		this->left=true;
		break;

	case EKA_STRAFE_RIGHT:
		this->right=true;
		break;
	default: break;
 	}

}

void irr::newton::ICameraBodyFPS::leftUp(EKEY_ACTION action) {

	switch(action) {

	case EKA_MOVE_FORWARD:
		this->up=false;
		break;

	case EKA_MOVE_BACKWARD:
		this->down=false;
		break;

	case EKA_STRAFE_LEFT:
		this->left=false;
		break;

	case EKA_STRAFE_RIGHT:
		this->right=false;
		break;
	default: break;
 	}
}

void irr::newton::ICameraBodyFPS::updatePause() {

	if(this->isPaused) {  //is in paused
		irr::u32 time = irr::newton::Hidden::GetDevice()->getTimer()->getRealTime();
		if(time>=this->timePauseStop) this->isPaused=false;
	}

}

void irr::newton::ICameraBodyFPS::setPause(irr::u32 timeMs) {

       if(timeMs!=0) {
                     this->timePauseStop =
                     irr::newton::Hidden::GetDevice()->getTimer()->getRealTime()+timeMs;

                     this->isPaused=true;
                     }

       else this->isPaused=false;

}
