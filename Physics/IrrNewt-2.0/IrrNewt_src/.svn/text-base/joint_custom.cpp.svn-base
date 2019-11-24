// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../../include/hidden.hpp"
#include "../../include/world.hpp"
#include "../../include/body.hpp"

#include "../../include/joint_custom.hpp"

using namespace irr::newton;

IJointCustom::IJointCustom(
    int MaxDOF, 
	irr::newton::IBody* Body0, 
	irr::newton::IBody* Body1) {

	this->n_joint = NULL;
	this->body0 = Body0;
	this->body1 = Body1;
	this->maxDOF = MaxDOF;

	this->n_joint = NewtonConstraintCreateUserJoint (
		this->getWorld()->getNewtonWorld(), 
		maxDOF, 
		IJointCustom::NewtonSubmitConstrainst, //callback
		this->body0->getNewtonBody(), 
		this->body1->getNewtonBody()); 

	NewtonJointSetUserData (this->n_joint, this);
	NewtonJointSetDestructor (this->n_joint, JointDestructor);

}

void  IJointCustom::NewtonSubmitConstrainst (const NewtonJoint* me)
{
	IJointCustom* joint;  

	// get the pointer to the joint class
	joint = (IJointCustom*) NewtonJointGetUserData (me);  
	joint->submitConstrainst();
}

irr::newton::IWorld* IJointCustom::getWorld() {
	return (irr::newton::IWorld*)NewtonWorldGetUserData(NewtonBodyGetWorld(this->body0->getNewtonBody()));
}

void  IJointCustom::JointDestructor (const NewtonJoint* me) {

	IJointCustom* joint;  

	// get the pointer to the joint class
	joint = (IJointCustom*) NewtonJointGetUserData (me);  

	// set the joint call to NULL to prevent infinite recursion
	NewtonJointSetDestructor (me, NULL);  
	NewtonJointSetUserData (me, NULL);  

//  delete the joint class
//	delete joint;

}
