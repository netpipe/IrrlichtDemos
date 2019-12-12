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

#include "toolbox_stdafx.h"
#include "OGLModel.h"
#include "OGLMesh.h"
#include "dBone.h"
#include "TargaToOpenGl.h"
#include "PhysicsUtils.h"


dMatrix OGLModel::m_matrixPalete[MAX_PALETTE_MATRIX];

OGLLoaderContext::OGLLoaderContext()
{
}

OGLLoaderContext::~OGLLoaderContext()
{
}


dModel* OGLLoaderContext::CreateModel ()
{
	return new OGLModel();
}


dMesh* OGLLoaderContext::CreateMesh (const char* name)
{
	OGLMesh* mesh;
	mesh = new OGLMesh (name);
	return mesh;
}

dBone* OGLLoaderContext::CreateBone (dBone* parent)
{
	return new dBone (parent);
}

void OGLLoaderContext::LoaderFixup (dModel* model)
{
	// load all textures
	for (dList<dMeshInstance>::dListNode* node = model->m_meshList.GetFirst(); node; node = node->GetNext()) { 
		OGLMesh* oglMesh = (OGLMesh*) node->GetInfo().m_mesh;
		oglMesh->OptimizeForRender ();

		for (dMesh::dListNode* subMesh = oglMesh->GetFirst(); subMesh; subMesh = subMesh->GetNext()) {
			dSubMesh& segment = subMesh->GetInfo();
			segment.m_textureHandle = LoadTexture(segment.m_textureName);
		}
	}
}


void OGLLoaderContext::SetUserData (const NewtonBody* body, dModel* model, const char* bindkeyword)
{
	NewtonBodySetUserData(body, model);
}

void OGLLoaderContext::SetTransformCallback (const NewtonBody* body, const char* bindkeyword)
{
	NewtonBodySetTransformCallback(body, PhysicsSetTransform);
}

void OGLLoaderContext::SetForceAndTorqueCallback (const NewtonBody* body, const char* bindkeyword)
{
	NewtonBodySetForceAndTorqueCallback(body, PhysicsApplyGravityForce);
}

void OGLLoaderContext::SetDestructorCallback (const NewtonBody* body, const char* bindkeyword)
{
	NewtonBodySetDestructorCallback(body, PhysicsBodyDestructor);
}




OGLModel::OGLModel(void)
{
	m_node = NULL;
}

OGLModel::~OGLModel(void)
{
}

void OGLModel::InitFromModel (const dModel& source)
{

	dModel::InitFromModel (source);
}


dMatrix OGLModel::GetMatrix () const
{
	_ASSERTE (m_skeleton.GetCount());
	dBone* rootBone = m_skeleton.GetFirst()->GetInfo();
	return rootBone->GetMatrix();
}


void OGLModel::SetMatrix (const dMatrix& matrix)
{
	_ASSERTE (m_skeleton.GetCount());
//	if (m_skeleton.GetCount()) {
		dBone* rootBone = m_skeleton.GetFirst()->GetInfo();
		rootBone->SetMatrix(matrix);
//	}
}



void OGLModel::Render()
{
	_ASSERTE (m_skeleton.GetCount());

//	if (m_skeleton.GetCount()) {
//	UpdateMatrixPalette (m_matrix, m_matrixPalete, sizeof (m_matrixPalete) / sizeof (dMatrix));
	dMatrix matrix (GetIdentityMatrix());
	UpdateMatrixPalette (matrix, m_matrixPalete, sizeof (m_matrixPalete) / sizeof (dMatrix));

	for (dList<dMeshInstance>::dListNode* node = m_meshList.GetFirst(); node; node = node->GetNext()) { 
		dMeshInstance& instance = node->GetInfo();
		OGLMesh* mesh = (OGLMesh*)instance.m_mesh;
		if (instance.GetModifier()) {
			mesh->Render (m_matrixPalete, (dSkinModifier*) instance.GetModifier());
		} else {
			glPushMatrix();
			glMultMatrix(&m_matrixPalete[instance.m_boneID][0][0]);
			mesh->Render (NULL);
			glPopMatrix();
		}
	}

//	} else {
//		glPushMatrix();
//		glMultMatrix(&m_matrix[0][0]);
//		for (dList<dMeshInstance>::dListNode* node = m_meshList.GetFirst(); node; node = node->GetNext()) { 
//			OGLMesh* mesh;
//			mesh = (OGLMesh*)node->GetInfo().m_mesh;
//			mesh->Render (NULL);
//		}
//		glPopMatrix();
//	}
}


