/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/


// NewtonCustomJoint.cpp: implementation of the NewtonCustomJoint class.
//
//////////////////////////////////////////////////////////////////////
#include "CustomJointLibraryStdAfx.h"
#include "NewtonCustomJoint.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NewtonCustomJoint::NewtonCustomJoint ()
{
	m_rtti = 0;
	m_autoDestroy = 0;
	m_joint = NULL;
	m_body0 = NULL;
	m_body1 = NULL;
	m_maxDox = NULL;
	m_world	= NULL;
	m_joint = NULL; 
	m_userData = NULL;
	m_userDestructor = NULL;
	m_userContrationCallback = NULL;
}

NewtonCustomJoint::NewtonCustomJoint (
	int maxDOF, 
	const NewtonBody* body0, 
	const NewtonBody* body1)
{
	Init (maxDOF, body0, body1);
}

NewtonCustomJoint::~NewtonCustomJoint()
{
	//_ASSERTE (m_joint);
	
	//if the joint has user data it means the application is destroy the joint
	if (NewtonJointGetUserData (m_joint)) {
		NewtonCustomJoint* joint;  

		// if there is a C destructor call it form here
		joint = (NewtonCustomJoint*) NewtonJointGetUserData (m_joint);  
		if (joint->m_userDestructor) {
			joint->m_userDestructor ((const NewtonUserJoint*) joint);
		}

		// set the joint call to NULL to prevent infinite recursion 
		NewtonJointSetUserData (m_joint, NULL);  
		NewtonJointSetDestructor (m_joint, NULL);  

		// destroy this joint
		if (!m_autoDestroy) {
			NewtonDestroyJoint(m_world, m_joint);
		}
	}
}

void *NewtonCustomJoint::operator new (size_t size) 
{ 
	return (void*) new char[size]; 
}
void NewtonCustomJoint::operator delete (void *ptr) 
{ 
	delete (char*)ptr;
}


void NewtonCustomJoint::Init (int maxDOF, const NewtonBody* body0, const NewtonBody* body1)
{
	m_rtti = 0;
	m_autoDestroy = 0;
	m_joint = NULL;
	m_body0 = body0;
	m_body1 = body1;
	m_maxDox = maxDOF;
	m_world	= NewtonBodyGetWorld (body0);
	m_joint = NewtonConstraintCreateUserJoint (m_world, maxDOF, SubmitConstraints, GetInfo, m_body0, m_body1); 

	NewtonJointSetUserData (m_joint, this);
	NewtonJointSetDestructor (m_joint, Destructor);

	m_userData = NULL;
	m_userDestructor = NULL;
	m_userContrationCallback = NULL;

}

int NewtonCustomJoint::GetJointID () const
{
	return m_rtti;

}
void NewtonCustomJoint::SetJointID (int rtti)
{
	m_rtti = rtti;
}


const NewtonBody* NewtonCustomJoint::GetBody0 () const
{
	return m_body0;
}

const NewtonBody* NewtonCustomJoint::GetBody1 () const
{
	return m_body1;
}

const NewtonJoint* NewtonCustomJoint::GetJoint () const
{
	return m_joint;
}

void NewtonCustomJoint::Destructor (const NewtonJoint* me)
{
	NewtonCustomJoint* joint;  

	// get the pointer to the joint class
	joint = (NewtonCustomJoint*) NewtonJointGetUserData (me);  

	// set the joint call to NULL to prevent infinite recursion
//	NewtonJointSetDestructor (me, NULL);  
//	NewtonJointSetUserData (me, NULL);  

	joint->m_autoDestroy = 1;

	// delete the joint class
	delete joint;
}


void  NewtonCustomJoint::SubmitConstraints (const NewtonJoint* me, dFloat timestep, int threadIndex)
{
	NewtonCustomJoint* joint;  

	// get the pointer to the joint class
	joint = (NewtonCustomJoint*) NewtonJointGetUserData (me);  

	// call the constraint call back
	joint->SubmitConstraints(timestep, threadIndex);

	// if there is a user define callback call it form here;
	if (joint->m_userContrationCallback) {
		joint->m_userContrationCallback ((const NewtonUserJoint*) joint, timestep, threadIndex);
	}

}

void NewtonCustomJoint::GetInfo (const NewtonJoint* me, NewtonJointRecord* info)
{
	NewtonCustomJoint* joint;  

	// get the pointer to the joint class
	joint = (NewtonCustomJoint*) NewtonJointGetUserData (me);  
	joint->GetInfo(info);
}


void NewtonCustomJoint::CalculateLocalMatrix (const dMatrix& pinsAndPivotFrame, dMatrix& localMatrix0, dMatrix& localMatrix1) const
{
//	_ASSERTE (0);

	dMatrix matrix0;

	// Get the global matrices of each rigid body.
	NewtonBodyGetMatrix(m_body0, &matrix0[0][0]);
	dMatrix matrix1 (GetIdentityMatrix());
	if (m_body1) {
		NewtonBodyGetMatrix(m_body1, &matrix1[0][0]);
	}


	// create a global matrix at the pivot point with front vector aligned to the pin vector
//	dMatrix pinAndPivotMatrix (dgGrammSchmidt(dir)); // 
//	pinAndPivotMatrix.m_posit = pivot;
//	pinAndPivotMatrix.m_posit.m_w = 1.0f;

	_ASSERTE (pinsAndPivotFrame.SanityCheck());

	// calculate the relative matrix of the pin and pivot on each body
 	localMatrix0 = pinsAndPivotFrame * matrix0.Inverse();
	localMatrix1 = pinsAndPivotFrame * matrix1.Inverse();

}


void NewtonCustomJoint::CalculateGlobalMatrix (const dMatrix& localMatrix0, const dMatrix& localMatrix1, dMatrix& matrix0, dMatrix& matrix1) const
{
	dMatrix body0Matrix;
	// Get the global matrices of each rigid body.
	NewtonBodyGetMatrix(m_body0, &body0Matrix[0][0]);

	dMatrix body1Matrix (GetIdentityMatrix());
	if (m_body1) {
		NewtonBodyGetMatrix(m_body1, &body1Matrix[0][0]);
	}
	matrix0 = localMatrix0 * body0Matrix;
	matrix1 = localMatrix1 * body1Matrix;
}


void NewtonCustomJoint::GetInfo (NewtonJointRecord* info) const
{
	_ASSERTE (0);
}

void NewtonCustomJoint::SetBodiesCollisionState (int state)
{
	NewtonJointSetCollisionState (m_joint, state);
}

int NewtonCustomJoint::GetBodiesCollisionState () const
{
	return NewtonJointGetCollisionState (m_joint);
}
