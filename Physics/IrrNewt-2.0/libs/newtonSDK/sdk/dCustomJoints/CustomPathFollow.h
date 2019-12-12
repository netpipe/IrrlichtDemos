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



// CustomPathFollow.h: interface for the CustomPathFollow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOM_PATH_FOLLOW_H__INCLUDED_)
#define AFX_CUSTOM_PATH_FOLLOW_H__INCLUDED_

#include "NewtonCustomJoint.h"

class JOINTLIBRARY_API CustomPathFollow: public NewtonCustomJoint  
{
	public:
	CustomPathFollow (const dMatrix& pinsAndPivoFrame, NewtonBody* body);
	virtual ~CustomPathFollow();

	virtual dMatrix EvalueCurve (const dVector& posit);

	protected:
	virtual void SubmitConstraints (dFloat timestep, int threadIndex);
	virtual void GetInfo (NewtonJointRecord* info) const;

	dMatrix m_localMatrix0;
};

#endif // !defined(AFX_CUSTOM_PATH_FOLLOW_H__INCLUDED_)

