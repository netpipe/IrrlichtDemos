// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_LIBRARY_PHYSICS_BODY_CONTROLLER_HPP_CODE_SDAGASVGFASEVE562516162186
#define IRRNEWT_LIBRARY_PHYSICS_BODY_CONTROLLER_HPP_CODE_SDAGASVGFASEVE562516162186
#pragma once

//#include "hidden.hpp"
//#include "dll_exports.hpp"

#include "body.hpp"

namespace irr{
	namespace newton{

          namespace Hidden {
              void  defCharacterControllerForceAndTorqueCallback(const NewtonBody* body);
			  void  defCharacterControllerTransformCallback(const NewtonBody* body, const float* matrix);
          }

		//!Class to management character controlling of rigid bodies
		class IRRNEWT_API ICharacterController:public irr::newton::IBody{
		public:

			//!
			//!constructor. don't call this directly. use World::createCharacterController() to create a character controller
			ICharacterController() {}

			//!
			//!constructor. don't call this directly. use World::createCharacterController() to create a character controller
			ICharacterController(irr::newton::ICharacterController* body);

			//!destructor
			virtual ~ICharacterController() {}

			virtual void setVelocity(irr::core::vector3df velocity);
			virtual void addForce(irr::core::vector3df force);
			virtual void setForce(irr::core::vector3df new_force);
			virtual void restoreCallbacks();
			virtual void setScaleBody(irr::core::vector3df scale);

			//!getDirectionPositionY, getDirectionPositionXY,getDirectionAbsolutePositionY
			//!,getDirectionAbsolutePositionXY and FRI* functions documentation
			//!These are high level functions.
			//!
			//!These move the body in a direction indipendent of its rotation.
			//!This mean that the body move in the same direction whatever is its rotation.
			//!For example if the body has got a -90 rotation on Y, using one of this function properly the body move forward
			//!If the rotation of the body is -180 on y, the body move forward also
			//!
			//!As you can see every function has a Y or a XY at its end.
			//!The letter indicates the body axis\\aces rotation. Y indicates that
			//!the body rotates only on y, every rotation on X and\\or z aces will be ignored
			//!(useful for character control
			//!for example in a FPS game), XY indicates that the body rotates on X and Y aces,
			//!every rotation on Z axis will be ignored for moving calculation (useful for a
			//!FPS camera)
			//!
			//!Every function takes a vector called 'direction'. This vector indicates the direction
			//!in wich the body must move on. A +x value indicates a forward moving,
			//!a -x value indicates a backward moving, a +z value indicates a strafe left moving
			//!a - z value indicates a strafe right moving.
			//!Vector must be indicates also the velocity and not only the direction
			//!For example pass a core:.vector3df(5,0,0) to move the body on 5 untis forwards,
			//!a core::vector3df(-2,0,0) for move the body of 2 untis backward,
			//!a core::vector3df (-3,0,-1) for moving the body on 3 units backward and strafing
			//!rigth of 1 untis at the same time (as you can see the value can be combined)
			//!
			//!Every function returns the body destination.
			//!As you can see some function has got a 'absolute' string in its name
			//!(getDirectionAbsolutePositionY and getDirectionAbsolutePositionXY)
			//!and some other functions not(getDirectionPositionY,getDirectionPositionXY)
			//!The functions wich NOT CONTAIN 'absolute' in its name return the "body position
			//!destination" considerating body position at (0,0,0).
			//!
			//!For example we call getDirectionPositionY with the vector (5,0,0)
			//!(the body rotate only on y, move of 5 units forward). The returned vector is called "A"
			//! Afterwards we translate the body into a new position different than
			//!the current position and
			//!we re-call
			//!getDirectionPositionY passing the same direction vector ( (5,0,0) ).
			//!The returned vector is called "B".
			//!In this case A is always equal to B, because
			//!the functions wich NOT CONTAIN 'absolute' in its name considerating the body
			//!position at (0,0,0). Generally you pass the returned vector to setVelocity or addForce
			//!functions
			//!
			//!Some other functions has 'absolute' in their name.In This case the function considerating
			//!the position of the body like its current position. In the example above, if we use
			//!getDirectionAbsolutePositionY instead of getDirectionPositionY, A and B is always different
			//!For example with getDirectionAbsolutePositionY(core::vector3df(1,0,0)) you will get the vector
			//!in front of the body in the real 3d coordinates
			//!WARNING: you can't pass this vector to setVelocity or addForce, using 'not absolute' function for doing that
			//!for example you can use an 'absolute' function to positiong an object in front of
			//!your body.
			//!Simply pass the returned vector to setPosition()
			//!
			//!Some functions has got a sintax like FIRgetDirection(Absolute)Position.
			//!This function has equal to respective function without FRI pre fix (
			//!FRIgetDirectionPositionY is equal to getDirectionPositionY, exc..) except
			//!of 1 thing only. The 'non FRI' functions multiplied the vector by the frame rate
			//!the 'FRI functions' don't do that. This mean that the 'non FRI' function
			//!return a vector that is depends on the frame rate. This make possible to move
			//!a body at the same velocity on all kind of computers (and whatever is the frame rate)
			//!you will see the body move at the same velocity at high frame rate and at low frame rate
			//!useful for moving a character
			//!
			//!The 'FRI functions' retur a vector non multiplied by the frame rate
			//!This mean that the vector is the same at all frame rate (and don't change if the frame rate change)
			//!DON'T USE THESE FUNCTIONS FOR MOVING THE BODY! These functions are useful
			//!for position a body in a direction reletive to an other body. For example position a ball
			//!in front of the character. If you use a 'non FRI' functino for doing that, the ball is
			//!farther from the character or nearest from the character depending on the frame rate
			irr::core::vector3df getDirectionPositionY(irr::core::vector3df direction);

			//!
			//!\see CharacterController::getDirectionPositionY()
			irr::core::vector3df getDirectionPositionXY(irr::core::vector3df direction);

			//!
			//!\see CharacterController::getDirectionPositionY()
			irr::core::vector3df getDirectionAbsolutePositionY(
				irr::core::vector3df direction
				);

			//!
			//!\see CharacterController::getDirectionPositionY()
			irr::core::vector3df getDirectionAbsolutePositionXY(
			    irr::core::vector3df direction);

			//frame rate indipendent

			//!
			//!\see CharacterController::getDirectionPositionY()
			irr::core::vector3df FRIgetDirectionPositionY(irr::core::vector3df direction);

			//!
			//!\see CharacterController::getDirectionPositionY()
			irr::core::vector3df FRIgetDirectionPositionXY(irr::core::vector3df direction);

			//!
			//!\see CharacterController::getDirectionPositionY()
			irr::core::vector3df FRIgetDirectionAbsolutePositionY(
				irr::core::vector3df direction
				);

			//!
			//!\see CharacterController::getDirectionPositionY()
			irr::core::vector3df FRIgetDirectionAbsolutePositionXY(
			    irr::core::vector3df direction);

			//!
			//!This function rotate a body based on mouse movements
			//!call this function inside a IEventReceiver::OnEvent function
			//!\param event the SEvent passed to the OnEvent function
			//!\param mouse_sensitive the sensitive of the mouse
			//!\return none
			void rotateFromMouse(
				irr::SEvent& event,
				float mouse_sensitive);

			//!
			//!Call this function if you rotate the body using setOmega, addTorque or setTorque
			//! (not advised for character controlling). If you control the character with
			//!(FRI)getDirection(Absolute)Position(Y\\XY) functions,
			//!please use CharacterController::rotateY() and\\or CharacterController::rotateX()
			//!to rotate the body, and not setOmega or addTorque. using rotateX you don't need to call this function
			//!\return none
			void adjustRotationForDirectionFunctions();

			//!Get the difference of the body of that body since the last update (simply do
			//!last_update_position-current_position)
			//!\return the position difference from the last update
			inline irr::core::vector3df getPositionDifference() {return position_difference;}

			//!Is the position changed from the last update? (has the body moved since the last update?)
			//!\return true if yes, false if not
			inline bool isPositionChanged() {return this->getPositionDifference()!=core::vector3df(0,0,0);}

			//!Pass yes if you want that the scene node has the same rotation of the physics rigid body
			//!false to leave their rotation separately
			//!\param value true or false
			//!\return none
			void setRotationUpdate(bool value);

			//!Return yes if the scene node has the same rotation of the physics rigid body
			//!false if IrrNewt mantein their rotation separately
			//!\return true or false
			//!\see CharacterController::setRotationUpdate()
			inline bool getRotationUpdate() {return this->update_rotation;}

			//!Call this function to avoid that a setVelocity call blocks the effect of a force
			//!The character will be able to move only after a force will stop its effects
			//!\return none
			inline void cantSetVelocityWhileMove() {this->can_set_velocity=1;}

			//!This function is inverse to CharacterController::cantSetVelocityWhileMove() function
			//!Call this function to able a body to move using setVelocity independently from the applied forces
			//!the character will be able to move immediately
			//!\return none
			inline void canSetVelocityWhileMove() {this->can_set_velocity=0;}

			//!Call this function to avoid that a setForce\\addForce call blocks the effect of another force
			//!While the body is moving a force can't affect it.
			//!\return none
			inline void cantForceWhileMove() {this->can_set_force=1;}

			//!inverse function of cantForceWhileMove(). Cancel cantForceWhileMove() effects
			inline void canForceWhileMove() {this->can_set_force=0;}

			//!get the returned value of the last (FRI)getDirection(Absolute)Position(X\\XY) call
			inline irr::core::vector3df getLastDirectionParameter() {return this->last_direction_parameter;}

			//!get the time elapsed since last IWorld::update() call when the last (FRI)getDirection(Absolute)Position(X\\XY) were called
			//!to get the current elapsed time use IWorld::getElapsedTime()
			inline irr::f32 getLastTimeElapsed() {return this->last_time_elapsed;}

			inline void setClimbStairForce(core::vector3df g) {this->gravity=g;}
			inline core::vector3df getClimbStairForce() {return this->gravity;}
			void setStairMaxHeight(irr::f32 s) {this->stair_max_height=s;}

			//!this is a high level function to jump
			//!the function to some test before jumping, like raycasting with floor, net applied force
			//!to be sure that the body is not currently jumping. So if you call this function on a jumping body not may be happen
			//!\param force y velocity to apply to make the body jumping. Proportional to jump height
			//!\param distanceFromFloor distance of body bounding box mion edge (NOT node bounding box) from floor to raycasting to check if the body is jumping. Default 27 would be a good value
			void jump(irr::f32 force, irr::f32 distanceFromFloor=27);

			//!crouch function
			//!\param scale the new y scale of the body (generally less than the initial scale for crouch)
			//!\return true if the body is already crouched, false if the body wasn't crouched and now it is crouching
			bool crouch(irr::f32 scale);

			//!stand. Recover from a crouch
			//!\param speed the velocity to apply to te body to make it go up from floor
			//!\param distanceFromFloor if the distance from floor is mayor than this no velocity will be apllied to make the body stand up from floor. Set this to 0 to never apply velocity, set this to a big value to alwas apply velocity. The default values is a good middle value. This is used to avoid that a body in air receive a y-up velocity to stand up
			bool stand(irr::f32 speed, irr::f32 distanceFromFloor=27);

			//!is the body crouching?
			//!\return true if the body is crouching
			bool isCrouching() {return this->is_crouching;}

			//!add a value at body current y rotation
			//!\param value value to add at the rotation
			//!\return none
			inline void rotateY(irr::f32 value) {
				core::vector3df rot=this->getRotationBody();
				rot.Y+=value;
				this->setRotation(rot);
			}

			//!add a value at body current x rotation
			//!\param value value to add at the rotation
			//!\return none
			inline void rotateX(irr::f32 value) {
				core::vector3df rot=this->getRotationBody();
				rot.X+=value;
				this->setRotation(rot);
			}

			//!add a value at body current z rotation
			//!\param value value to add at the rotation
			//!\return none
			inline void rotateZ(irr::f32 value) {
				core::vector3df rot=this->getRotationBody();
				rot.Z+=value;
				this->setRotation(rot);
			}

			//!copy a value to body current x rotation
			//!\param value new rotation on the axis
			//!\return none
			inline void newRotationX(float value) {
				core::vector3df rot=this->getRotationBody();
				rot.X=value;
				this->setRotation(rot);
			}

			/*
			inline bool getApplyForceIfNotAirBorne(){return this->apply_force_if_not_air_borne;}
			inline void setApplyForceIfNotAirBorne(bool value){this->apply_force_if_not_air_borne=value;}
			*/

		protected:

			//character control callbacks
			friend void  irr::newton::Hidden::defCharacterControllerForceAndTorqueCallback(const NewtonBody* body, float, int);
			friend void  irr::newton::Hidden::defCharacterControllerTransformCallback(const NewtonBody* body, const float* matrix, int);

			void setCorrectCallback();

			virtual void reserved_destroy() {this->~ICharacterController();}

			bool update_rotation, center_mouse, apply_force_if_not_air_borne, is_crouching;
			irr::u32 can_set_velocity,can_set_force;
			irr::f32 last_time_elapsed;
			irr::core::vector3df position_difference;
			irr::core::vector3df last_direction_parameter;
			irr::core::vector3df last_body_pos;

			//for gravity
			irr::core::vector3df gravity;
			irr::f32 stair_max_height;
		};
	}
}

#endif
