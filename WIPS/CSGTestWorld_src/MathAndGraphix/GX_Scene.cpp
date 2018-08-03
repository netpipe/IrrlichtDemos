
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_Scene.h"

#include "GX_Model.h"

//#include "GL/glut.h"
#include "GL/glut.h"

gxSceneNode::gxSceneNode()
{
	m_model			= NULL;

	m_parent		= NULL;
	m_firstChild	= NULL;
	m_sibling		= NULL;

	m_localToParentTransform.SetMatrix(mlMatrix3x4Identity);
}

gxSceneNode::~gxSceneNode()
{
	SetParent(NULL);

	while(GetFirstChild())
		delete GetFirstChild();
}

void gxSceneNode::Render()
{
	glPushMatrix();

	mlMatrix4x4 mMatrix = mlMatrix4x4(m_localToParentTransform.GetMatrix());
	mlMatrix4x4 * pMatrix = &mMatrix;
	mlFloat * pafMatrix = reinterpret_cast<mlFloat*>(pMatrix);

	glMultMatrixf(pafMatrix);

	if(m_model)
	{
		m_model->Render();
	}

	for(gxSceneNode * child = GetFirstChild(); child != NULL; child = GetNextChild(child))
	{
		child->Render();
	}

	glPopMatrix();
}

void gxSceneNode::Update()
{
	for(gxSceneNode * child = GetFirstChild(); child != NULL; child = GetNextChild(child))
	{
		child->Update();
	}
}

mlTransform	&	gxSceneNode::GetTransform()
{
	return m_localToParentTransform;
}

void			gxSceneNode::SetTransform(const mlTransform & transform)
{
	m_localToParentTransform = transform;
}

gxModel * gxSceneNode::GetModel()
{
	return m_model;
}

void gxSceneNode::SetModel(gxModel * model)
{
	m_model = model;
}

gxSceneNode * gxSceneNode::GetParent()
{
	return m_parent;
}

void gxSceneNode::SetParent(gxSceneNode * newParent)
{
	assert((! newParent) || (! newParent->IsDescendentOf(this)));
	assert(newParent != this);

	if (m_parent != NULL)
	{
		m_parent->RemoveChild(this);
	}

	m_parent = newParent;

	if (newParent != NULL)
	{
		newParent->AddChild(this);
	}
}

void gxSceneNode::AddChild(gxSceneNode * newChild)
{
	if(m_firstChild)
	{
		gxSceneNode * currentNode = m_firstChild;

		while(currentNode->m_sibling != NULL)
		{
			currentNode = currentNode->m_sibling;
		}
		currentNode->m_sibling = newChild;
		newChild->m_sibling = NULL;
	}
	else
	{
		newChild->m_sibling = NULL;
		m_firstChild = newChild;
	}
}

void gxSceneNode::RemoveChild(gxSceneNode * child)
{
	assert(child != NULL);

	gxSceneNode **pp = &m_firstChild;
	while (*pp != NULL)
	{
		if (*pp == child)
		{
			*pp = child->m_sibling;
			child->m_sibling = NULL;
			return;
		}
		pp = & (*pp)->m_sibling;
	}
	assert(false);
}

bool gxSceneNode::IsDescendentOf(gxSceneNode * potentialAncestor)
{
	for (gxSceneNode * ancestor = GetParent(); ancestor != NULL; ancestor = ancestor->GetParent())
	{
		if (potentialAncestor == ancestor)
			return true;
	}
	return false;
}

gxSceneNode * gxSceneNode::GetFirstChild()
{
	return m_firstChild;
}

gxSceneNode * gxSceneNode::GetNextChild(gxSceneNode * child)
{
	if(child->GetParent() == this)
	{
		return child->m_sibling;
	}
	else
	{
		return NULL;
	}
}

mlTransform gxSceneUtility::BuildTransform(gxSceneNode * node, gxSceneNode * ancestor)
{
	gxSceneNode * traversingNode = node;

	mlTransform transform;

	while (traversingNode != ancestor)
	{
		transform.TransformSelf(traversingNode->GetTransform());
		traversingNode = traversingNode->GetParent();

		if(traversingNode == NULL)
		{
			return mlTransformIdentity;
		}
	}

	transform.Invert();

	return transform;
}

