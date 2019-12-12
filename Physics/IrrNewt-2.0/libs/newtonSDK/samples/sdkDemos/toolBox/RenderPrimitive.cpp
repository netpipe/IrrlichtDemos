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


// RenderPrimitive.cpp: implementation of the RenderPrimitive class.
//
//////////////////////////////////////////////////////////////////////
#include <toolbox_stdafx.h>
#include "OpenGlUtil.h"
#include "dTree.h"
#include "dCRC.h"
#include "../OGLMesh.h"
#include "TargaToOpenGl.h"
#include "RenderPrimitive.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RenderPrimitive::RenderPrimitive ()
	:OGLModel()
{
	dBone* rootBone = new dBone(NULL);
	AddSkeleton(rootBone);
	rootBone->m_boneID = 0;
	rootBone->SetNameID("root");
	rootBone->Release();

	m_effectTexture = 0;
	m_specialEffect = NULL;
//	m_controlVolume = 1.0e10f;
	m_modifierSkewAngleY = 0.0f;
	m_modifierScaleAngleX = 0.0f;
	m_modifierScaleAngleY = 0.0f;
	m_modifierScaleAngleZ = 0.0f;

}

RenderPrimitive::RenderPrimitive(const dMatrix& matrix, OGLMesh* geometry)
	:OGLModel()
{
	SetMesh(geometry);

	dBone* rootBone = new dBone(NULL);
	AddSkeleton(rootBone);
	rootBone->m_boneID = 0;
	rootBone->SetNameID("root");
	rootBone->Release();

	SetMatrix (matrix);

	m_effectTexture = 0;
	m_specialEffect = NULL;
//	m_destrctionLevel = 3;
//	m_controlVolume = 1.0e10f;
	m_modifierSkewAngleY = dFloat (dRand () % 360) / 6.2832f;
	m_modifierScaleAngleX = dFloat (dRand () % 360) / 6.2832f;
	m_modifierScaleAngleY = dFloat (dRand () % 360) / 6.2832f;
	m_modifierScaleAngleZ = dFloat (dRand () % 360) / 6.2832f;
}

RenderPrimitive::~RenderPrimitive()
{
	if (m_specialEffect) {
		NewtonMeshDestroy(m_specialEffect);
	}
}

OGLMesh *RenderPrimitive::GetMesh() const
{
	if (m_meshList.GetFirst()) {
		return (OGLMesh *)m_meshList.GetFirst()->GetInfo().m_mesh;
	} else {
		return NULL;
	}
}

void RenderPrimitive::SetMesh(OGLMesh *mesh)
{
	if (GetMesh()) {
		GetMesh()->Release();
		m_meshList.Remove(m_meshList.GetFirst());
	}
	if (mesh) {
		AddMesh(mesh);
	}
}


void RenderPrimitive::CreateVisualEffect (const NewtonWorld* world)
{
	// save the geometry in for of a NewtonMesh effect for fast preprocessing
	m_specialEffect = GetMesh()->BuildMesh(world);

	// set a detail texture for the cut part
	m_effectTexture = LoadTexture("dirt.tga");
}


/*
CameraNode::CameraNode (dModel* parent)
	:dModel()
{
	SetNameID (CAMERA_NAME);
	m_fov = 60.0f;
	m_aspect = 1.0f;
	m_farPlane = 0.1f;
	m_nearPlane = 1000.0f;
}

void CameraNode::Render() const
{
	_ASSERTE (0);
//	dModel::Render();
}
*/

