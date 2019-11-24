// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_FRAMEWORK_CAMERAFPSSDFAD_HPP_ASDBASEGASFASDFAW13153132
#define IRRNEWT_FRAMEWORK_CAMERAFPSSDFAD_HPP_ASDBASEGASFASDFAW13153132
#pragma once

#include "dll_exports.hpp"

#include "irrnewt_base.hpp"
#include "body_controller.hpp"
#include "stdio.h" //NULL declaration

#pragma warning(4:4275) //non dll-interface class 'irr::IEventReceiver' used as base for dll-interface class 'irr::newton::ICameraBodyFPS'

#define MOVECALLBACK(x) void (*x) (bool up, bool down, bool left, bool right, bool is_moving)

namespace irr{
	namespace newton{

		//struct SIntersectionPoint;
		class ICharacterController;

		//!Class to check collision and raycasting
		class IRRNEWT_API ICameraBodyFPS:public irr::newton::ICharacterController,irr::IEventReceiver {
		public:
			ICameraBodyFPS(irr::newton::ICharacterController* base);

			virtual bool OnEvent(const irr::SEvent& event);

			//!set mouse sensitive
			inline void setMouseSensitive(irr::f32 value) {this->mouseSensitive=value;}

			//!set movement speed
			inline void setMoveSpeed(irr::f32 value) {this->moveSpeed=value;}

			//!set fps camera type
			inline void setType(irr::newton::E_CAMERA_FPS_TYPE type);

			//!set keys to move forward,backward,left and right
			inline void setKeyMap(SKeyMap* array,irr::u32 size);

			//!set a user defined funtion to call EVERY loop and pass to it the move state of the camera
			inline void setMoveCallback( MOVECALLBACK(userCallback) ) {this->moveCallback = userCallback; }

			//!set a pause in milliseconds. for a user defined value of milliseconds the camera won't move and won't rotate
			//!\param timeMs amount of the pause in milliseconds
			void setPause(irr::u32 timeMs);

		private:

			//functions pointer
			MOVECALLBACK(moveCallback);

			void updatePause();

			irr::core::array<SKeyMap> key_array;
			irr::f32 moveSpeed,mouseSensitive;

			bool verticalMovement;
			bool onlyY;
			bool up,down,left,right;

			bool isPaused;
			irr::u32 timePauseStop;

			void reserved_destroy() {}
			void pressedDown(EKEY_ACTION action);
			void leftUp(EKEY_ACTION action);

			friend void irr::newton::Hidden::defCharacterControllerForceAndTorqueCallback(const NewtonBody* body, float, int);

		}; //class ICameraBodyFPS

	}//raf
}//irr

//---------INLINE FUNCTIONS---------------------------

inline void irr::newton::ICameraBodyFPS::setKeyMap(SKeyMap* array,irr::u32 size) {
	for(irr::u32 i=0;i<size;i++) {
		this->key_array.push_back(array[i]);
	}
}


inline void irr::newton::ICameraBodyFPS::setType(irr::newton::E_CAMERA_FPS_TYPE type) {

	this->verticalMovement = true;
	this->onlyY            = false;

	switch(type) {

		case ECFT_NORMAL:
			this->onlyY = false;
			this->verticalMovement = true;
			break;

		case ECFT_ONLY_Y:
			this->onlyY = true;
			break;

		case ECFT_NO_VERTICAL:
			this->verticalMovement = false;
			break;

		default:
			break;

	}//switch

}//function


#endif
