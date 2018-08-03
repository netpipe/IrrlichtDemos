
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_SCENE_H
#define GX_SCENE_H

#include "ML_Transform.h"

class gxModel;

class gxSceneNode
{
public:

			gxSceneNode();
	virtual	~gxSceneNode();

	virtual void	Render();
	virtual void	Update();

	gxModel *		GetModel();
	void			SetModel(gxModel * model);

	gxSceneNode *	GetParent();
	void			SetParent(gxSceneNode * newParent);

	gxSceneNode *	GetFirstChild();
	gxSceneNode *	GetNextChild(gxSceneNode * child);

	bool			IsDescendentOf(gxSceneNode * potentialAncestor);

	mlTransform	&	GetTransform();
	void			SetTransform(const mlTransform & transform);
	
	void			Pick();

private:

	void			AddChild(gxSceneNode * newChild);
	void			RemoveChild(gxSceneNode * child);

	gxModel *		m_model;

	gxSceneNode *	m_parent;
	gxSceneNode *	m_firstChild;
	gxSceneNode *	m_sibling;

	mlTransform		m_localToParentTransform;
};

class gxSceneUtility
{
public:

	static mlTransform BuildTransform(gxSceneNode * node, gxSceneNode * ancestor);
};

#endif // GX_SCENE_H
