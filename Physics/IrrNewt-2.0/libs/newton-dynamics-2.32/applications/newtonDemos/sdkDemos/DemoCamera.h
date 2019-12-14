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


// RenderPrimitive.h: interface for the RenderPrimitive class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DEMO_CAMERA_H__
#define __DEMO_CAMERA_H__

#include <toolbox_stdafx.h>
#include "DemoEntity.h"


class DemoCamera: public DemoEntity
{
	public:
	DemoCamera();
	~DemoCamera();

	void SetProjectionMode(int width, int height);

	void Update();
	virtual void Render(dFloat timeStep) const;


	dFloat m_fov;
	dFloat m_frontPlane;
	dFloat m_backPlane;

	dVector m_origin;
	dVector m_upVector;
	dVector m_pointOfInterest;
	


};

#endif 

