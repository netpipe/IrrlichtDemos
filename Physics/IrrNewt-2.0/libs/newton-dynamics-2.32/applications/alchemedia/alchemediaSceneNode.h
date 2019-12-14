/////////////////////////////////////////////////////////////////////////////
// Name:        alchemediaSceneNode.h
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

#ifndef __ALCHEMEDIA_SCENE_NODE_H__
#define __ALCHEMEDIA_SCENE_NODE_H__

#include "dSceneNodeInfo.h"

class alchemediaSceneNode: public dSceneNodeInfo
{
public:
	D_DEFINE_CLASS_NODE_ESSENCIALS(alchemediaSceneNode, dSceneNodeInfo)

	alchemediaSceneNode(void);
	alchemediaSceneNode(dScene* world);
	alchemediaSceneNode(const alchemediaSceneNode& me);

	~alchemediaSceneNode(void);

	void DrawGizmo(dScene* const world, dScene::dTreeNode* const myNode, const dMatrix& coordinaSystem, const dVector& color, dGizmoMode mode, dFloat size) const;
	dGizmoHandle GetHighlightedGizmoHandle(dScene* const world, dScene::dTreeNode* const myNode, const dMatrix& coordinaSystem, const dVector& screenPosition, dGizmoMode mode, dFloat size) const;
	void DrawGizmoHandle(dScene* world, const dMatrix& coordinaSystem, dGizmoMode mode, dGizmoHandle handle, const dVector& color, dFloat size) const;


	void DrawWireFrame(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const;
	void DrawSolidWireFrame(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const;
	void DrawGouraudShaded(dScene* const world, dScene::dTreeNode* const myNode, const dVector& color) const;
};

#endif