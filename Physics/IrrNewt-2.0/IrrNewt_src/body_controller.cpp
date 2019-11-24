// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/body_controller.hpp"
#include "../include/world.hpp"
#include "../include/utils.hpp"
#include "../include/hidden.hpp"

#include <string>
using namespace std;

irr::newton::ICharacterController::ICharacterController(irr::newton::ICharacterController* body):IBody() {

	this->force = body->force;
	this->box_size = body->box_size;

	this->collision = body->collision;

	this->force_continuos = body->force_continuos;
	this->material = body->material;
	this->node = body->node;

	this->torque = body->torque;
	this->body = body->body;
	this->applied_offset = body->applied_offset;
	this->camera = body->camera;


	//standard character control attribute
	this->class_type.Type = ECT_BODY_CONTROLLER;
	this->center_mouse = false;
	this->apply_force_if_not_air_borne = true;
	this->is_crouching = false;
	this->last_direction_parameter = core::vector3df();
	this->last_time_elapsed = 0;
	this->can_set_velocity = 0;//can set velocity
	this->can_set_force = 0;//can set force
    this->last_body_pos.X = UNDEFINED_PARAMETER;
    this->last_body_pos.Y = UNDEFINED_PARAMETER;
    this->last_body_pos.Z = UNDEFINED_PARAMETER;


	//set new force and torque callback
	this->setCorrectCallback();

	//set new user data
	NewtonBodySetUserData(this->body,this);
}

void irr::newton::ICharacterController::setCorrectCallback() {
	if(this->node->getType()==scene::ESNT_CAMERA||
		this->node->getType()==scene::ESNT_CAMERA_MAYA||
		this->node->getType()==scene::ESNT_CAMERA_FPS)

		NewtonBodySetTransformCallback(this->body,
			irr::newton::Hidden::defCameraCharacterControllerTransformCallback);

	else

		NewtonBodySetTransformCallback(this->body,
			irr::newton::Hidden::defCharacterControllerTransformCallback);

	NewtonBodySetForceAndTorqueCallback(this->body,
		irr::newton::Hidden::defCharacterControllerForceAndTorqueCallback);
}

//to set default callbacks
void irr::newton::ICharacterController::restoreCallbacks() {

	NewtonBodySetTransformCallback(this->body,
			irr::newton::Hidden::defCharacterControllerTransformCallback);

	NewtonBodySetForceAndTorqueCallback(this->body,
		irr::newton::Hidden::defCharacterControllerForceAndTorqueCallback);

}

void irr::newton::ICharacterController::rotateFromMouse(
												 irr::SEvent& event,
												 float mouse_sensitive) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT&&
		event.MouseInput.Event==EMIE_MOUSE_MOVED)  {

			irr::IrrlichtDevice* device = this->getWorld()->getIrrlichtDevice();
			irr::video::IVideoDriver* driver=device->getVideoDriver();

		core::position2d<f32> point=device->getCursorControl()->getRelativePosition();

		//center the mouse
		this->center_mouse=true;

		this->node->updateAbsolutePosition();
		core::vector3df last_rotation=this->node->getRotation();

		if(this->getWorld()->getTimeElapsed()!=0.0f) {

		last_rotation.X+=(((point.Y-0.5f)*mouse_sensitive) * this->getWorld()->getTimeElapsed());
		last_rotation.Y+=(((point.X-0.5f)*mouse_sensitive) * this->getWorld()->getTimeElapsed());

		if(last_rotation.Y>360)last_rotation.Y-=360;
        if(last_rotation.Y<0)last_rotation.Y+=360;
		if(last_rotation.X>360)last_rotation.X-=360;
        if(last_rotation.X<0)last_rotation.X+=360;

		f32 rotx=this->node->getRotation().X;

		if(rotx<185&&rotx>89)
			last_rotation.X=/*89*/80;

		if(rotx>186&&rotx<271)
			last_rotation.X=/*271*/260;

		this->setRotation(last_rotation);

		}//if(driver!=NULL&&driiver->getFPS()!=0) {

		return;

	}//if (event.MouseInput.Event==EMIE_MOUSE_MOVED)
}

void irr::newton::ICharacterController::adjustRotationForDirectionFunctions() {
	if(this->getRotationBody().X>+170.0f&&
		this->getRotationBody().X<+182.0f&&
		this->getRotationBody().Z>+170.0f&&
		this->getRotationBody().Z<+182.0f) {

		core::vector3df p=this->getRotationBody();
		p=core::vector3df(180,180,180)-p;
		this->setRotationBody(p);
		this->node->updateAbsolutePosition();
	}
}

/*
void irr::newton::ICharacterController::setRotation(irr::core::vector3df rot) {
}
*/

void irr::newton::ICharacterController::setVelocity(irr::core::vector3df velocity) {

	if(this->can_set_velocity==0) {

		float velocity_array[3];
		irr::newton::Hidden::Vector3dfFillArray(velocity,velocity_array);
		NewtonBodySetVelocity(this->body,velocity_array);

	}
}

void irr::newton::ICharacterController::addForce(irr::core::vector3df force) {
	if(this->can_set_force==0) {

		this->force+=force;

	}
}

void irr::newton::ICharacterController::setForce(irr::core::vector3df new_force) {
	if(this->can_set_force==0) {

		this->force=force;
	}

}

void irr::newton::ICharacterController::setScaleBody(irr::core::vector3df scale) {

	//backup data
	//bool update_rotation=this->getRotationUpdate();
	//bool is_crouching=this->isCrouching();

	//call IBody set scale
	irr::newton::IBody* body = (static_cast<irr::newton::IBody*>(this));
	IBody::setScaleBody(scale);

	//restore data
	//this->setRotationUpdate(update_rotation);
	//this->is_crouching=is_crouching;

	//restore correct callback. very important!
	this->setCorrectCallback();
}

//----------PRIVATE-------------------------------------------------
/*
template<class Num> std::string NumString(Num num) {
		std::ostringstream stringa;
		stringa<<num;
		return stringa.str();
}
*/
//------------------------------------------------------------------------------
//CHARACTER CONTROLLER

irr::core::vector3df irr::newton::ICharacterController::getDirectionPositionY(
										   irr::core::vector3df direction) {
	this->last_time_elapsed=this->getWorld()->getTimeElapsed();

	irr::core::matrix4 matrix;
	matrix.setRotationDegrees(irr::core::vector3df(
		 0,
		 this->node->getRotation().Y,
		 0));
	 matrix.rotateVect(direction);

	 direction*=this->getWorld()->getTimeElapsed();

	 this->last_direction_parameter=direction;
	 return direction;
}

irr::core::vector3df irr::newton::ICharacterController::getDirectionPositionXY(irr::core::vector3df direction)  {

	this->last_time_elapsed=this->getWorld()->getTimeElapsed();

	 core::matrix4 matrix;
	 matrix.setRotationDegrees(core::vector3df(
		 this->node->getRotation().X,
		 this->node->getRotation().Y,
		 0));
	 matrix.rotateVect(direction);

	 direction*=this->getWorld()->getTimeElapsed();

	 this->last_direction_parameter=direction;

	 return direction;
}

irr::core::vector3df irr::newton::ICharacterController::getDirectionAbsolutePositionY(
	irr::core::vector3df direction
	) {

	this->last_time_elapsed=this->getWorld()->getTimeElapsed();

	this->last_direction_parameter=this->getDirectionPositionY(direction)+this->node->getPosition();;
	return this->last_direction_parameter;
}

irr::core::vector3df irr::newton::ICharacterController::getDirectionAbsolutePositionXY(
												   irr::core::vector3df direction) {
	this->last_direction_parameter=direction;
	this->last_time_elapsed=this->getWorld()->getTimeElapsed();

	this->last_direction_parameter=this->getDirectionPositionXY(direction)+this->node->getPosition();
	return this->last_direction_parameter;
}

void irr::newton::ICharacterController::setRotationUpdate(bool value) {
	this->update_rotation=value;}

//frame rate indipendent

irr::core::vector3df irr::newton::ICharacterController::FRIgetDirectionPositionY(irr::core::vector3df direction) {

	this->last_time_elapsed=this->getWorld()->getTimeElapsed();

	irr::core::matrix4 matrix;
	matrix.setRotationDegrees(irr::core::vector3df(
		 0,
		 this->node->getRotation().Y,
		 0));
	 matrix.rotateVect(direction);

	 this->last_direction_parameter=direction;
	 return direction;
}

irr::core::vector3df irr::newton::ICharacterController::FRIgetDirectionPositionXY(irr::core::vector3df direction) {
	this->last_direction_parameter=direction;
	this->last_time_elapsed=this->getWorld()->getTimeElapsed();

	irr::core::matrix4 matrix;
	matrix.setRotationDegrees(irr::core::vector3df(
		 this->node->getRotation().X,
		 this->node->getRotation().Y,
		 0));
	 matrix.rotateVect(direction);

	 this->last_direction_parameter=direction;
	 return direction;
}

//absolute position

irr::core::vector3df irr::newton::ICharacterController::FRIgetDirectionAbsolutePositionY(
	irr::core::vector3df direction
	) {

	this->last_time_elapsed=this->getWorld()->getTimeElapsed();


	this->last_direction_parameter=this->FRIgetDirectionPositionY(direction)+this->getPosition();
	return (this->last_direction_parameter);

}

irr::core::vector3df irr::newton::ICharacterController::FRIgetDirectionAbsolutePositionXY(
					 irr::core::vector3df direction)
{
	this->last_direction_parameter=direction;
	this->last_time_elapsed=this->getWorld()->getTimeElapsed();

	this->last_direction_parameter=this->FRIgetDirectionPositionXY(direction)+this->getPosition();
	return (this->last_direction_parameter);
}


//------------------jumping----------------------------------------------------------------

void irr::newton::ICharacterController::jump(
											 irr::f32 force,
											 irr::f32 distanceFromFloor) {

	core::aabbox3d<f32> box = this->calculateAABB();

	core::line3d<f32> line(box.MinEdge,core::vector3df());
	line.end = line.start;
	line.end.Y -= 999999.9f;

	irr::newton::SIntersectionPoint i_point=
		this->getWorld()->getCollisionManager()->getCollisionFirstPointEx(line);

	if(i_point.body!= NULL) {

		irr::f32 distance = i_point.point.getDistanceFrom(line.start);

		if(distance <= distanceFromFloor) {

			core::vector3df net_applied_force = this->getNetAppliedForce();

			/*
			//DEBUG
			core::stringc m;
			m+=(double)net_applied_force.Y;
			this->getWorld()->getIrrlichtDevice()->getLogger()->log(m.c_str());
			*/

			this->getWorld()->getUtils()->round(net_applied_force,1);

			/*
			//DEBUG
			m="";
			m+=(double)net_applied_force.Y;
			this->getWorld()->getIrrlichtDevice()->getLogger()->log(m.c_str());
			*/

			if(net_applied_force.Y == 0.0f) {

				//can jump!!
				core::vector3df vel = this->getVelocity();
				vel.Y = force;
				this->setVelocity(vel);

			}

		}//distance
	}//body!=NULL
}

//---------------------------CROUCHING-----------------------------------

static irr::f32 last_crouch_scale=0;

bool irr::newton::ICharacterController::crouch(irr::f32 scale) {

	if(!this->is_crouching) {
		//crouch
		this->is_crouching = true;
		last_crouch_scale = scale;
		this->setScaleBody(core::vector3df(1,scale,1));
		return true;
	}

	return false;
}

bool irr::newton::ICharacterController::stand(irr::f32 speed, irr::f32 distanceFromFloor) {

	if(this->is_crouching) {

		this->is_crouching=false;
		this->setScaleBody(core::vector3df(1,1,1));

		//velocity to make the body standing
		newton::SIntersectionPoint point;
		f32 dist = this->getWorld()->getUtils()->getDistanceFromFloor(this, point);

		if(dist <= distanceFromFloor) {

			core::vector3df vel = this->getVelocity();
			vel.Y += speed;
			this->setVelocity(vel);

		}

		return true;
	}

	return false;
}

//---------------------------END OF CROUCHING-----------------------------------
