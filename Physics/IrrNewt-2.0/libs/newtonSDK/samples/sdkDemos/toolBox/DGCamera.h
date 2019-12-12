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

// DGCamera.h: interface for the DGCamera class.
// DGCamera by Dave Gravel - 2009.
// DGCamera Version 3.0b
// unfinished coming soon.

#ifndef __DGCamera_H__ 
#define __DGCamera_H__ 

class DGDummyCamera 
{
  public:
  DGDummyCamera();
  dMatrix m_Matrix;
  virtual ~DGDummyCamera();
};

class DGCamera  
{
	public:
	DGCamera();
//	DGCamera(dFloat NearPlane, dFloat FocalLength, dFloat DepthOfView, dFloat SceneScale, dFloat NearPlaneBias);
	virtual ~DGCamera();
	void Apply();

//	void ApplyPerspective(int pwidth, int pheight, GLint viewport[4]);
	void PointToMatrix(const dMatrix pMatrix, const dVector UpVector);
	void TargetPosition(dFloat tpX, dFloat tpY, dFloat tpZ);
	void TargetPosition(dVector tpXYZ);
	void Position(dFloat pX, dFloat pY, dFloat pZ);
	void Position(dVector pXYZ);
	void Direction(dFloat dX, dFloat dY, dFloat dZ);
	void Direction(dVector dXYZ);
	void Up(dFloat uX, dFloat uY, dFloat uZ);
	void Up(dVector uXYZ);
	void Front(dFloat fX, dFloat fY, dFloat fZ);
	void Front(dVector fXYZ);
	void ResetDefaultCamera ();


	dMatrix m_Matrix;
	dMatrix m_AbsoluteMatrix;
	dFloat m_NearPlane;
	dFloat m_FocalLength;
	dFloat m_DepthOfView;
	dVector m_TargetPosition;
	dVector m_LastDirection;
	bool m_Target;

//	dFloat m_Dpi;
//	dFloat m_SceneScale;
//	dFloat m_NearPlaneBias;
//	DGDummyCamera* m_DummyParent; 

};

#endif