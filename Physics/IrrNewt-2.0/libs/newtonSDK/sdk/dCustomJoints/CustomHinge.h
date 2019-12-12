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

// CustomHinge.h: interface for the CustomHinge class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_CUSTOMHINGE_H__B631F556_468B_4331_B7D7_F85ECF3E9ADE__INCLUDED_)
#define AFX_CUSTOMHINGE_H__B631F556_468B_4331_B7D7_F85ECF3E9ADE__INCLUDED_

#include "NewtonCustomJoint.h"

class JOINTLIBRARY_API CustomHinge: public NewtonCustomJoint  
{
	public:
	CustomHinge (const dMatrix& pinsAndPivoFrame, const NewtonBody* child, const NewtonBody* parent = NULL);
	virtual ~CustomHinge();

	void EnableLimits(bool state);
	void SetLimis(dFloat minAngle, dFloat maxAngle);
	dFloat GetJointAngle () const;
	dVector GetPinAxis () const;
	dFloat GetJointOmega () const;

	protected:
	virtual void GetInfo (NewtonJointRecord* info) const;
	virtual void SubmitConstraints (dFloat timestep, int threadIndex);

	dMatrix m_localMatrix0;
	dMatrix m_localMatrix1;

	bool m_limitsOn;
	dFloat m_minAngle;
	dFloat m_maxAngle;
	dFloat m_jointOmega;
	AngularIntegration m_curJointAngle;
};

#endif // !defined(AFX_CUSTOMHINGE_H__B631F556_468B_4331_B7D7_F85ECF3E9ADE__INCLUDED_)

