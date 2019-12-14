/////////////////////////////////////////////////////////////////////////////
// Name:        alchemediaStaticMesh.h
// Purpose:     
// Author:      Julio Jerez
// Modified by: 
// Created:     22/05/2010 08:02:08
// RCS-ID:      
// Copyright:   Copyright (c) <2010> <Newton Game Dynamics>
// License:     
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely
/////////////////////////////////////////////////////////////////////////////

#ifndef __ALCHEMEDIA_STATIC_MESH_H__
#define __ALCHEMEDIA_STATIC_MESH_H__

#include "dMeshNodeInfo.h"

class alchemediaStaticMesh: public dMeshNodeInfo
{
public:
	D_DEFINE_CLASS_NODE_ESSENCIALS(alchemediaStaticMesh, dMeshNodeInfo)

	alchemediaStaticMesh(void);
	alchemediaStaticMesh(dScene* world);
	alchemediaStaticMesh(NewtonMesh* mesh);
	alchemediaStaticMesh(const alchemediaStaticMesh& me);

	~alchemediaStaticMesh(void);

	void InvalidateCache();

	virtual void ConvertToPolygons();
	virtual void ConvertToTriangles();
	virtual void WeldTJoints ();
	virtual void RemoveUnusedVertices(dScene* const world, dScene::dTreeNode* const myNode);
	virtual void SmoothNormals (dFloat angleInRadiants);


//	virtual void DrawGizmo(dScene* const world, dScene::dTreeNode* const myNode, const dMatrix& coordinaSystem, const dVector& color, dGizmoMode mode, dFloat size) const;
//	virtual dGizmoHandle GetHighlightedGizmoHandle(dScene* const world, dScene::dTreeNode* const myNode, const dMatrix& coordinaSystem, const dVector& screenPosition, dGizmoMode mode, dFloat size) const;
//	virtual void DrawGizmoHandle(dScene* world, const dMatrix& coordinaSystem, dGizmoMode mode, dGizmoHandle handle, const dVector& color, dFloat size) const;
	
	void DrawWireFrame(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const;
	void DrawSolidWireFrame(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const;
	void DrawGouraudShaded(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const;

	
	mutable GLint m_solidDipalyList;
	mutable GLint m_gouraudDipalyList;
	mutable GLint m_wireFrameDipalyList;
	
};

#endif