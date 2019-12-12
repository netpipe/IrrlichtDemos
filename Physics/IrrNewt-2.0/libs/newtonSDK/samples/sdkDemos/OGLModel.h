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

#ifndef __OGL_MODEL_H__
#define __OGL_MODEL_H__

#include <dModel.h>
#include <dBone.h>
#include <dMesh.h>

#define MAX_PALETTE_MATRIX  512


class OGLLoaderContext: public dLoaderContext
{
	public:
	OGLLoaderContext();
	~OGLLoaderContext();

	virtual dModel* CreateModel ();
	virtual dMesh* CreateMesh (const char* name);
	virtual dBone* CreateBone (dBone* parent);

	virtual void LoaderFixup (dModel* model);

	virtual void SetUserData (const NewtonBody* body, dModel* model, const char* bindkeyword);
	virtual void SetTransformCallback (const NewtonBody* body, const char* bindkeyword);
	virtual void SetForceAndTorqueCallback (const NewtonBody* body, const char* bindkeyword);
	virtual void SetDestructorCallback (const NewtonBody* body, const char* bindkeyword);

};


class OGLModel: public dModel
{
public:
	OGLModel(void);
	virtual ~OGLModel(void);

	dMatrix GetMatrix () const;
	void SetMatrix (const dMatrix& matrix);
	
	virtual void InitFromModel (const dModel& source);

	virtual void Render();
	void* m_node;

	static dMatrix m_matrixPalete[MAX_PALETTE_MATRIX];
};

#endif
