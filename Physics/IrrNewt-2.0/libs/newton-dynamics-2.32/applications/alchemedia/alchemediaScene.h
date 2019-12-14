/////////////////////////////////////////////////////////////////////////////
// Name:        alchemediaScene.h
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

#ifndef __ALCHEMEDIA_SCENE_H__
#define __ALCHEMEDIA_SCENE_H__

class alchemediaExplorerView;
class alchemediaTextureCache;


class alchemriaRegisterClass
{
	public:
	alchemriaRegisterClass(const char* const className, const dNodeInfo* const singletonClass)
	{
		dNodeInfo::ReplaceSingletonClass (className, singletonClass);
	}
};

#define D_ALCHEMDIA_REGISTER_CLASS(className) \
	D_IMPLEMENT_CLASS_NODE(className) \
	static alchemriaRegisterClass __##className(className::BaseClassName(), &className::GetSingleton())


class alchemediaScene: public dScene
{
	public:
	class SceneNodeSelection: public dTree<dScene::dTreeNode*, dScene::dTreeNode*>
	{
	};

	alchemediaScene();
	alchemediaScene(alchemediaTextureCache* const m_textureCache);
	alchemediaScene(const alchemediaScene& me);
	~alchemediaScene(void);

	
	void SetNewtonWorld (NewtonWorld* const newton);
	void SetExplorer (alchemediaExplorerView* const explorer);
	void SetTextCache (alchemediaTextureCache* const textureCache);

	SceneNodeSelection& GetSelectedNodes();

	bool Deserialize (const char* const fileName);
	void LoadTextures (const char* const firstTextPath);
	void UnloadTextures ();

	void DeselectAll ();
	void AddToSelection (dTreeNode* const node);	
	void UpdateMultiSelection (dTreeNode* const node);
	void UpdateSimpleSelection (dTreeNode* const node);

	void ApplyMatrixToSelected (const dMatrix& matrix);


	private:
	
	
	
	SceneNodeSelection m_selectedNodes;
	alchemediaExplorerView* m_explorer;
	alchemediaTextureCache* m_textureCache;
};

#endif