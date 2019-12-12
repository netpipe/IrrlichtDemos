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



// ScenePrimitive.h: interface for the ScenePrimitive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENE_PRIMITIVE_H__)
#define AFX_SCENE_PRIMITIVE_H__

#include "Newton.h"
#include "../OGLModel.h"


class ScenePrimitive: public OGLModel  
{

	struct VisualProxy
	{	
		dMatrix m_matrix;
		OGLMesh* m_mesh;
		NewtonSceneProxy* m_sceneProxi;
	};

	public:
	ScenePrimitive(const NewtonWorld* nWorld, const char* name, int optimized);
	virtual ~ScenePrimitive();
	NewtonBody* GetRigidBody() const {return m_level;}

	private:

	void AddCollisionTree (NewtonCollision* scene, const NewtonWorld* nWorld, const char* name, int optimized);
	void AddPrimitives (NewtonCollision* scene, const NewtonWorld* nWorld);


	static void DebugCallback (const NewtonBody* bodyWithTreeCollision, const NewtonBody* body,	int faceID, int vertexCount, const dFloat* vertex, int vertexstrideInBytes);

	virtual void Render();

	public:
	NewtonBody* m_level;
	dList<VisualProxy> m_pieces; 
};

#endif 

