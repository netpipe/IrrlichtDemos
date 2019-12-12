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

#if !defined(AFX_RENDERPRIMITIVE_H__0C9BDF8F_FF8D_4F5A_9692_4E76E3C8878A__INCLUDED_)
#define AFX_RENDERPRIMITIVE_H__0C9BDF8F_FF8D_4F5A_9692_4E76E3C8878A__INCLUDED_

#include <toolbox_stdafx.h>
#include "../OGLModel.h"


class OGLMesh;
#define CAMERA_NAME	"cameraNode"
struct NewtonWorld;


class RenderPrimitive: public OGLModel
{
	public:
	RenderPrimitive ();
	RenderPrimitive(const dMatrix& matrix, OGLMesh* geometry);
	virtual ~RenderPrimitive();
	OGLMesh *GetMesh() const;
	void SetMesh(OGLMesh *mesh);

	
	void CreateVisualEffect (const NewtonWorld* world);
	public:

	int m_effectTexture;
	NewtonMesh* m_specialEffect;

	dFloat m_density;
//	dFloat m_controlVolume;
	dFloat m_modifierScaleAngleX;
	dFloat m_modifierScaleAngleY;
	dFloat m_modifierScaleAngleZ;
	dFloat m_modifierSkewAngleY;
};

/*
class CameraNode: public dModel
{
	public:
	CameraNode (dModel* parent);

	void Render() const;
	dFloat m_fov;
	dFloat m_aspect;
	dFloat m_farPlane;
	dFloat m_nearPlane;
};
*/

#endif 

