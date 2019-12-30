// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "SceneNodeAnimatorFollowCamera.h"

namespace irr
{
namespace scene
{

// constructor
CSceneNodeAnimatorFollowCamera::CSceneNodeAnimatorFollowCamera(const core::vector3df& startPosition)
: StartPosition(startPosition)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorFollowCamera");
	#endif
}

// animates a scene node
void CSceneNodeAnimatorFollowCamera::animateNode(ISceneNode* node, u32 timeMs)
{
	if (node)
	{
		if(node->getSceneManager())
		{
			ICameraSceneNode* camera = node->getSceneManager()->getActiveCamera();
			if(camera)
			{
				node->setPosition(StartPosition + camera->getPosition());
			}
		}
	}
}

// Writes attributes of the scene node animator.
void CSceneNodeAnimatorFollowCamera::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	out->addVector3d("StartPosition", StartPosition);
}

// Reads attributes of the scene node animator.
void CSceneNodeAnimatorFollowCamera::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	StartPosition = in->getAttributeAsVector3d("StartPosition");
}

// Returns type of the scene node animator
ESCENE_NODE_ANIMATOR_TYPE CSceneNodeAnimatorFollowCamera::getType() const 
{ 
	// return type of the scene node animator
	// (important when using with a custom scene animator node factory)
	return scene::ESNAT_UNKNOWN; //(ESCENE_NODE_ANIMATOR_TYPE) ECSNAT_FOLLOW_CAMERA; 
}

ISceneNodeAnimator* CSceneNodeAnimatorFollowCamera::createClone(ISceneNode* node, ISceneManager* newManager)
{
	CSceneNodeAnimatorFollowCamera * newAnimator = 
		new CSceneNodeAnimatorFollowCamera(StartPosition);

	return newAnimator;
}

} // end namespace scene
} // end namespace irr

