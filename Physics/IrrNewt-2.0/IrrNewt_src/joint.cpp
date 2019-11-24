// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/hidden.hpp"

//#include "../include/joint.hpp"

#include "../include/joint_universal.hpp"
#include "../include/joint_upvector.hpp"
#include "../include/joint_hinge.hpp"
#include "../include/joint_corkscrew.hpp"
#include "../include/joint_slider.hpp"
#include "../include/joint_ball.hpp"

#include "../include/world.hpp"
#include "../include/body.hpp"

//joint
irr::newton::IJoint::IJoint() {
	this->class_type.Type=ECT_JOINT;
}

void irr::newton::IJoint::setCollisionState(bool value) {
	NewtonJointSetCollisionState(this->n_joint,value);
}

bool irr::newton::IJoint::getCollisionState() {
	bool value_bool;
	irr::s32 value_int=NewtonJointGetCollisionState(this->n_joint);

	//to avoid a warning
	switch(value_int) {
		case 1: value_bool=true;
		default: value_bool=false;
		}

	return value_bool;
}

//----------------------------------------------------
//up vector joint
//----------------------------------------------------------

irr::newton::IJointUpVector::IJointUpVector(
	irr::newton::IWorld* world,
	irr::newton::SJointUpVector joint_data):IJoint() {

	this->n_world=const_cast<NewtonWorld*>(world->getNewtonWorld());

	irr::f32 pin_dir_array[3];
	irr::newton::Hidden::Vector3dfFillArray(joint_data.PinDir,pin_dir_array);

	//create the joint
	this->n_joint=NewtonConstraintCreateUpVector(
		this->n_world,
		pin_dir_array,
		joint_data.ParentBody->getNewtonBody());
}

irr::core::vector3df irr::newton::IJointUpVector::getPin() {
	irr::f32 pin_array[3];
	NewtonUpVectorGetPin(this->n_joint,pin_array);
	return irr::newton::Hidden::Vector3dfFromArray(pin_array);
}

void irr::newton::IJointUpVector::setPin(irr::core::vector3df new_pin) {
	irr::f32 pin_array[3];
	irr::newton::Hidden::Vector3dfFillArray(new_pin,pin_array);
	NewtonUpVectorSetPin(this->n_joint,pin_array);
}

void irr::newton::IJoint::setStiffness(irr::f32 value) {
	NewtonJointSetStiffness(this->n_joint,value);
}

irr::f32 irr::newton::IJoint::getStiffness() {
	return NewtonJointGetStiffness(this->n_joint);
}


//----------------------------------------------------
//hinge joint
//----------------------------------------------------------

irr::newton::IJointHinge::IJointHinge(
	irr::newton::IWorld* world,
	irr::newton::SJointHinge jointData) {

	this->n_world=const_cast<NewtonWorld*>(world->getNewtonWorld());

	jointData.Pivot*=IrrToNewton;

	irr::f32 pin_dir_array[3];
	irr::newton::Hidden::Vector3dfFillArray(jointData.PinDir,pin_dir_array);

	irr::f32 pivot_array[3];
	irr::newton::Hidden::Vector3dfFillArray(jointData.Pivot,pivot_array);

	//create the joint
	this->n_joint=NewtonConstraintCreateHinge(
		this->n_world,
		pivot_array,
		pin_dir_array,
		jointData.ParentBody->getNewtonBody(),
		jointData.ChildBody->getNewtonBody());

	NewtonJointSetUserData(this->n_joint,this);

	//set callback
	NewtonHingeSetUserCallback(this->n_joint,irr::newton::Hidden::defJointHingeCallback);
}

//----------------------------------------------------
//universal joint
//----------------------------------------------------------

irr::newton::IJointUniversal::IJointUniversal(
	irr::newton::IWorld* world,
	irr::newton::SJointUniversal jointData) {

	this->n_world=const_cast<NewtonWorld*>(world->getNewtonWorld());

	jointData.Pivot*=IrrToNewton;

	irr::f32 pin0_dir_array[3];
	irr::newton::Hidden::Vector3dfFillArray(jointData.Pin0,pin0_dir_array);

	irr::f32 pin1_dir_array[3];
	irr::newton::Hidden::Vector3dfFillArray(jointData.Pin1,pin1_dir_array);

	irr::f32 pivot_array[3];
	irr::newton::Hidden::Vector3dfFillArray(jointData.Pivot,pivot_array);

	//create the joint
	this->n_joint=NewtonConstraintCreateUniversal(
		this->n_world,
		pivot_array,
		pin0_dir_array,
		pin1_dir_array,
		jointData.ParentBody->getNewtonBody(),
		jointData.ChildBody->getNewtonBody());

	NewtonJointSetUserData(this->n_joint,this);

}


//----------------------------------------------------
//Corkscrew joint
//----------------------------------------------------------


irr::newton::IJointCorkscrew::IJointCorkscrew(
	irr::newton::IWorld* world,
	irr::newton::SJointCorkscrew jointData) {

	this->n_world=const_cast<NewtonWorld*>(world->getNewtonWorld());

	jointData.Pivot*=IrrToNewton;

	irr::f32 pin0_dir_array[3];
	irr::newton::Hidden::Vector3dfFillArray(jointData.PinDir,pin0_dir_array);

	irr::f32 pivot_array[3];
	irr::newton::Hidden::Vector3dfFillArray(jointData.Pivot,pivot_array);

	//create the joint
	this->n_joint=NewtonConstraintCreateCorkscrew(
		this->n_world,
		pivot_array,
		pin0_dir_array,
		jointData.ParentBody->getNewtonBody(),
		jointData.ChildBody->getNewtonBody());

	NewtonJointSetUserData(this->n_joint,this);
}


//----------------------------------------------------
//Slider joint
//----------------------------------------------------------


irr::newton::IJointSlider::IJointSlider(
	irr::newton::IWorld* world,
	irr::newton::SJointSlider jointData) {

	this->n_world=const_cast<NewtonWorld*>(world->getNewtonWorld());

	jointData.Pivot*=IrrToNewton;

	irr::f32 pin_dir_array[3];
	irr::newton::Hidden::Vector3dfFillArray(jointData.PinDir,pin_dir_array);

	irr::f32 pivot_array[3];
	irr::newton::Hidden::Vector3dfFillArray(jointData.Pivot,pivot_array);

	//create the joint
	this->n_joint=NewtonConstraintCreateSlider(
		this->n_world,
		pivot_array,
		pin_dir_array,
		jointData.ParentBody->getNewtonBody(),
		jointData.ChildBody->getNewtonBody());

	NewtonJointSetUserData(this->n_joint,this);
}


//----------------------------------------------------
//ball joint
//----------------------------------------------------------
void irr::newton::IJointBall::setConeLimits(
	core::vector3df pin,
	irr::f32 maxConeAngle,
	irr::f32 maxTwistAngle
	) {

	irr::f32 pin_array[3];
	irr::newton::Hidden::Vector3dfFillArray(pin,pin_array);
	NewtonBallSetConeLimits(this->n_joint,pin_array,maxConeAngle,maxTwistAngle);
}

irr::newton::IJointBall::IJointBall(
	irr::newton::IWorld* world,
	irr::newton::SJointBall jointData) {

	this->n_world=const_cast<NewtonWorld*>(world->getNewtonWorld());

	jointData.Pivot*=IrrToNewton;

	irr::f32 pivot_dir_array[3];
	irr::newton::Hidden::Vector3dfFillArray(jointData.Pivot,pivot_dir_array);

	//create the joint
	this->n_joint=NewtonConstraintCreateBall(
		this->n_world,
		pivot_dir_array,
		jointData.ParentBody->getNewtonBody(),
		jointData.ChildBody->getNewtonBody());

	NewtonJointSetUserData(this->n_joint,this);
}
