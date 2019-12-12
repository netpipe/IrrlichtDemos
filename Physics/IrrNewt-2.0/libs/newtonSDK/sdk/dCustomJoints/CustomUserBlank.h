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



// CustomUserBlankJoint.h: interface for the CustomUserBlank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOM_USER_BLANK_INCLUDED_)
#define AFX_CUSTOM_USER_BLANK_INCLUDED_

#include "NewtonCustomJoint.h"

class JOINTLIBRARY_API CustomUserBlank: public NewtonCustomJoint  
{
	public:
	CustomUserBlank(unsigned int maxDOF, const NewtonBody* child, const NewtonBody* parent = NULL);
	virtual ~CustomUserBlank();

	protected:
	virtual void SubmitConstraints (dFloat timestep, int threadIndex);
	virtual void GetInfo (NewtonJointRecord* info) const;
};

#endif // !defined(AFX_CUSTOM_USER_BLANK_INCLUDED_)

