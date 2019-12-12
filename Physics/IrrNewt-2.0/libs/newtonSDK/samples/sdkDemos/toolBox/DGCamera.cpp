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

// DGCamera.cpp: implementation of the DGCamera class.
// This DGCamera is currently a work in progress by Dave Gravel - 2009.
// DGCamera Version 3.0b
//////////////////////////////////////////////////////////////////////
#include "toolbox_stdafx.h"
#include "DGCamera.h"

/*
DGDummyCamera::DGDummyCamera ()
{
m_Matrix = GetIdentityMatrix();
}

DGDummyCamera::~DGDummyCamera()
{
}
*/

DGCamera::DGCamera ()
{
	m_Matrix = GetIdentityMatrix();
	m_AbsoluteMatrix = GetIdentityMatrix();

	Direction(dVector(1.0f,0.0f,0.0f,0.0f));

	m_TargetPosition = dVector(0.0f,0.0f,0.0f,1.0f);
	m_LastDirection = dVector(0.0f,0.0f,0.0f,0.0f);

	m_NearPlane = 0.1f;
	m_FocalLength = 60.0f;
	m_DepthOfView = 1000.0f;
//	m_SceneScale = 1.0f;
//	m_NearPlaneBias = 1.0f;
//	m_Dpi = 96.0f;
	m_Target = false;
	//  m_DummyParent = new DGDummyCamera();
}

/*
DGCamera::DGCamera (dFloat NearPlane, dFloat FocalLength, dFloat DepthOfView, dFloat SceneScale, dFloat NearPlaneBias)
{
m_Matrix = GetIdentityMatrix();
m_AbsoluteMatrix = GetIdentityMatrix();
Direction(dVector(0.0f,0.0f,-1.0f,0.0f));
m_TargetPosition = dVector(0.0f,0.0f,0.0f,1.0f);
m_LastDirection = dVector(0.0f,0.0f,0.0f,0.0f);
m_NearPlane = NearPlane;
m_FocalLength = FocalLength;
m_DepthOfView = DepthOfView;
m_SceneScale = SceneScale;
m_NearPlaneBias = NearPlaneBias;
m_Dpi = 96;
m_Target = false;
m_DummyParent = new DGDummyCamera();
}
*/

DGCamera::~DGCamera()
{
//	if (m_DummyParent) {
//		delete m_DummyParent;
//	}
}

void DGCamera::ResetDefaultCamera ()
{
//	m_FocalLength = 60;
	Position(-40.0f, 10.0f, 0.0f);
	Direction(1.0f,0.0f,0.0f);
	Up(0.0f,1.0f,0.0f);
}

/*
void DGCamera::ApplyPerspective(int pwidth, int pheight, GLint viewport[4])
{ 
dFloat LLeft;
dFloat LRight;
dFloat LTop;
dFloat LBottom;
dFloat zFar;
dFloat MaxDim;
dFloat Ratio;
dFloat f;
if (( pwidth <= 0 ) || ( pheight <= 0 )) { 
return;
}
MaxDim = dFloat( pwidth );
if ( pheight > dFloat( MaxDim ) ) {
MaxDim = dFloat( pheight );
}
f = m_NearPlaneBias / ( pwidth * m_SceneScale );
Ratio = ( 2 * viewport[2] + 2 * viewport[0] - pwidth ) * f;
LRight = Ratio * pwidth / ( 2 * MaxDim );
Ratio = ( pwidth - 2 * viewport[0] ) * f;
LLeft = -Ratio * pwidth / ( 2 * MaxDim );
f = m_NearPlaneBias / ( pheight * m_SceneScale );
Ratio = ( 2 * viewport[3] + 2 * viewport[0] - pheight ) * f;
LTop = Ratio * pheight / ( 2 * MaxDim );
Ratio = ( pheight - 2 * viewport[0] ) * f;
LBottom = -Ratio * pheight / ( 2 * MaxDim );
m_NearPlane = m_FocalLength * 2 * m_Dpi / ( 25.4f * MaxDim ) * m_NearPlaneBias;
zFar = m_NearPlane + m_DepthOfView;
glFrustum( LLeft, LRight, LBottom, LTop, m_NearPlane, zFar );
}
*/

void DGCamera::Apply()
{
	dMatrix mat;
	dVector absPos;
	dVector v;
	dVector d;
	dFloat n;

	if (m_Target){
		v = m_TargetPosition;
		absPos = m_Matrix.m_posit;
		d = v - absPos;
		n = dSqrt(d % d);
		d = d.Scale(n);
		m_LastDirection = d;
		m_LastDirection.m_w = 0.0f;
		gluLookAt(absPos.m_x, absPos.m_y, absPos.m_z, v.m_x, v.m_y, v.m_z, m_Matrix.m_up.m_x, m_Matrix.m_up.m_y, m_Matrix.m_up.m_z);	
	} else {
		_ASSERTE (0);
/*
		if (m_DummyParent) {
			m_AbsoluteMatrix = m_Matrix * m_DummyParent->m_Matrix;
			mat = m_DummyParent->m_Matrix;
			v = mat.TransformVector(m_Matrix.m_right);
			d = mat.TransformVector(m_Matrix.m_up);
		} else {
			m_AbsoluteMatrix = m_Matrix;	 
			v = m_Matrix.m_right;
			d = m_Matrix.m_up;
		}
		absPos = m_AbsoluteMatrix.m_posit;
		m_LastDirection = v;
		m_LastDirection.m_w = 0.0f;
		gluLookAt(absPos.m_x, absPos.m_y, absPos.m_z, 
			absPos.m_x+v.m_x, absPos.m_y+v.m_y, absPos.m_z+v.m_z, 
			d.m_x, d.m_y, d.m_z);
*/
	}
}

void DGCamera::PointToMatrix(const dMatrix pMatrix, const dVector UpVector)
{
	m_Target = false;
	dVector absDir; 
	dVector absRight; 
	dVector absUp;
	dFloat n;
	absDir = pMatrix.m_posit - m_Matrix.m_posit;
	n = dSqrt(absDir % absDir);
	//n = 1.0f / dSqrt(absDir % absDir);
	absDir = absDir.Scale(n);
	absDir.m_w = 0.0f;
	absRight = absDir * UpVector;
	n = dSqrt(absRight % absRight);
	//n = 1.0f / dSqrt(absRight % absRight);
	absRight = absRight.Scale(n);
	absRight.m_w = 0.0f;
	absUp = absRight * absDir;
	absUp.m_w = 0.0f;
	m_Matrix.m_right = absDir;
	m_Matrix.m_up = absUp;
}

void DGCamera::TargetPosition(dFloat tpX, dFloat tpY, dFloat tpZ)
{
	m_TargetPosition = dVector(tpX,tpY,tpZ,1.0f);
	m_Target = true;
}

void DGCamera::TargetPosition(dVector tpXYZ)
{
	m_TargetPosition = tpXYZ;
	m_Target = true;
}

void DGCamera::Position(dFloat pX, dFloat pY, dFloat pZ)
{
	m_Matrix.m_posit = dVector(pX,pY,pZ,1.0f);
}

void DGCamera::Position(dVector pXYZ)
{
	m_Matrix.m_posit = pXYZ;
}

void DGCamera::Direction(dFloat dX, dFloat dY, dFloat dZ)
{
	m_Matrix.m_right = dVector(dX,dY,dZ,0.0f);
}

void DGCamera::Direction(dVector dXYZ)
{
	m_Matrix.m_right = dXYZ;
}

void DGCamera::Up(dFloat uX, dFloat uY, dFloat uZ)
{
	m_Matrix.m_up = dVector(uX,uY,uZ,0.0f);
}

void DGCamera::Up(dVector uXYZ)
{
	m_Matrix.m_up = uXYZ;
}

void DGCamera::Front(dFloat fX, dFloat fY, dFloat fZ)
{
	m_Matrix.m_front = dVector(fX,fY,fZ,0.0f);
}

void DGCamera::Front(dVector fXYZ)
{
	m_Matrix.m_front = fXYZ;
}
