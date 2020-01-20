// NewtonLeader.h: interface for the NewtonLeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWTONLEADER_H__97F454A0_33A3_4074_9B3B_E7E7EB7E155C__INCLUDED_)
#define AFX_NEWTONLEADER_H__97F454A0_33A3_4074_9B3B_E7E7EB7E155C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NewtonBoid.h"



class NewtonLeader : public NewtonBoid 
{
public:
	NewtonLeader();
	virtual ~NewtonLeader();

void Init(ISceneManager* smgr,
		NewtonWorld* world, 
		vector3df* position, vector3df rotationDegrees,
		char* filename,
		float scale,
		float mass);
void setMaterialData(CMaterialDataBaseClass& m);
 
 

protected:
	void  NewtonLeader::ApplyForceEventHandler (const NewtonBody* body);

};

#endif // !defined(AFX_NEWTONLEADER_H__97F454A0_33A3_4074_9B3B_E7E7EB7E155C__INCLUDED_)
