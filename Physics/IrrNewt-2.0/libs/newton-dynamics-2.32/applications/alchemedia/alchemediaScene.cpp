/////////////////////////////////////////////////////////////////////////////
// Name:        alchemediaScene.cpp
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
#include "alchemedia.h"
#include "alchemediaScene.h"
#include "alchemediaCamera.h"
#include "alchemediaTexCache.h"
#include "alchemediaSceneNode.h"
#include "alchemediaStaticMesh.h"


alchemediaScene::alchemediaScene()
	:dScene (NULL), m_explorer(NULL), m_textureCache(NULL)
{
}

alchemediaScene::alchemediaScene(alchemediaTextureCache* const textureCache)
	:dScene (NULL), m_explorer(NULL), m_textureCache(textureCache)
{
}

alchemediaScene::alchemediaScene(const alchemediaScene& me)
	:dScene (me), m_explorer(me.m_explorer), m_textureCache(me.m_textureCache)
{
}

alchemediaScene::~alchemediaScene(void)
{
	UnloadTextures ();
}


void alchemediaScene::SetNewtonWorld (NewtonWorld* const newton)
{
	m_newton = newton;
}

void alchemediaScene::SetExplorer (alchemediaExplorerView* const explorer)
{
	m_explorer = explorer;
}

void alchemediaScene::SetTextCache (alchemediaTextureCache* const textureCache)
{
	m_textureCache = textureCache;
}


alchemediaScene::SceneNodeSelection& alchemediaScene::GetSelectedNodes()
{
	return m_selectedNodes;
}



void alchemediaScene::LoadTextures (const char* const firstTextPath)
{
	dTreeNode* const root = GetRootNode();
	for (void* ptr0 = GetFirstChild (root); ptr0; ptr0 = GetNextChild(root, ptr0)) {
		dTreeNode* const texNode0 = GetNodeFromLink(ptr0);
		dNodeInfo* const info = GetInfoFromNode(texNode0);
		if (info->IsType(dTextureNodeInfo::GetRttiType())) {

			dTextureNodeInfo* const texture = (dTextureNodeInfo*) info;
			if (texture->GetInternalId() == -1) {
				char pathName[2048];
				//neImageTga image;
				const char* const texName = dGetNameFromPath (texture->GetPathName());
				sprintf (pathName, "%s/%s", firstTextPath, texName);
				m_textureCache->LoadTexture (pathName, texture);
			}
		}
	}
}


void alchemediaScene::UnloadTextures ()
{
	dTreeNode* root = GetRootNode();
	for (void* ptr0 = GetFirstChild (root); ptr0; ptr0 = GetNextChild(root, ptr0)) {
		dTreeNode* const texNode0 = GetNodeFromLink(ptr0);
		dNodeInfo* const info = GetInfoFromNode(texNode0);
		if (info->IsType(dTextureNodeInfo::GetRttiType())) {
			dTextureNodeInfo* const texture = (dTextureNodeInfo*) info;
			if (texture->GetInternalId() != -1) {
				if (texture->GetRef() == 1) {
					m_textureCache->UnloadTexture (texture);
				}
			}
		}
	}

}


bool alchemediaScene::Deserialize (const char* const fileName)
{
	bool state = dScene::Deserialize(fileName);

	MergeDuplicateTextures();
	UpdateAllOOBB ();

	char texPath[256];
	dExtractPathFromFullName (fileName, texPath);
	LoadTextures (texPath);

	return state;
}


void alchemediaScene::DeselectAll ()
{
	m_selectedNodes.RemoveAll();

//	neSceneExplorer* explorer = (neSceneExplorer*) ((neMainFrame*)wxGetApp().GetTopWindow())->GetCommandPanel()->FindWindow(_T("neSceneExplorer"));
//	explorer->UnselectAll();
}


void alchemediaScene::AddToSelection (dTreeNode* const node)
{
	m_selectedNodes.Insert(node, node);
}

void alchemediaScene::UpdateMultiSelection (dTreeNode* const node)
{
	// select this node
	SceneNodeSelection::dTreeNode* const oldNode = m_selectedNodes.Find(node);
	if (!oldNode) {
		//selection->m_currentSelection.Insert(item, node);
		m_selectedNodes.Insert(node, node);
	} else {
		m_selectedNodes.Remove(node);
	}

	//	neMainFrame* mainFrame =(neMainFrame*)wxGetApp().GetTopWindow();
//	neSceneExplorer* explorer = (neSceneExplorer*) ((neMainFrame*)wxGetApp().GetTopWindow())->GetCommandPanel()->FindWindow(_T("neSceneExplorer"));
//	explorer->UpdateSelections (node);
}

void alchemediaScene::UpdateSimpleSelection (dTreeNode* const node)
{
	SceneNodeSelection::dTreeNode* const oldNode = m_selectedNodes.Find(node);

	DeselectAll();	
	if (!oldNode) {
		//selection->m_currentSelection.Insert(item, node);
		m_selectedNodes.Insert(node, node);
	}

//	neSceneExplorer* explorer = (neSceneExplorer*) ((neMainFrame*)wxGetApp().GetTopWindow())->GetCommandPanel()->FindWindow(_T("neSceneExplorer"));	
//	explorer->UpdateSelections (node);
}

void alchemediaScene::ApplyMatrixToSelected (const dMatrix& matrix)
{
	dTreeNode* pool[D_GRAPH_MAX_STACK_DEPTH];

	// replace all affected scene nodes with instances
	int mark = GetLRU();
	SceneNodeSelection::Iterator iter (m_selectedNodes);
	for (iter.Begin(); iter; iter ++) {
		int stack = 1;
		pool[0] = iter.GetKey();

		while (stack) {

			stack --;
			dTreeNode* const node = pool[stack];
			//dSceneNodeInfo* sceneInfo = (dSceneNodeInfo*) GetInstanceInfoFromNode(node);	
			dSceneNodeInfo* sceneInfo = (dSceneNodeInfo*) GetInfoFromNode(node);	
			if (GetNodeLRU (node) != mark) {

				SetNodeLRU (node, mark); 
				sceneInfo->SetTransform(sceneInfo->GetTransform() * matrix);

				for (void* ptr = GetFirstChild(node); ptr; ptr = GetNextChild(node, ptr)) {
					dTreeNode* const child = GetNodeFromLink(ptr);
					dNodeInfo* const info = GetInfoFromNode(child);
					if (info->IsType(dSceneNodeInfo::GetRttiType())) {
						pool[stack] = child;
						stack ++;
					}
				}
			}
		}
	}
}

