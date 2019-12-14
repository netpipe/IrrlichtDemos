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

// HeightFieldPrimitive.h: interface for the HeightFieldPrimitive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HeightFieldPrimitive_H____INCLUDED_)
#define AFX_HeightFieldPrimitive_H____INCLUDED_


#include "Newton.h"
#include "RenderPrimitive.h"


class HeightFieldPrimitive: public RenderPrimitive   
{
	public:
	HeightFieldPrimitive(NewtonWorld* nWorld);
	virtual ~HeightFieldPrimitive();

	NewtonBody* GetRigidBody() const;

	static void Destructor (const NewtonBody* body);
	static void DebugCallback (const NewtonBody* bodyWithTreeCollision, const NewtonBody* body, int faceID, int vertexCount, const dFloat* vertex, int vertexstrideInBytes);

	private:
	NewtonBody* m_level;
};

#endif 

