// Copyright (C) 2006 Cestmir "CZestmyr" Houska
// This file uses the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef _C_CROSS_PLANE_SCENE_NODE_BY_CZESTMYR_3573_
#define _C_CROSS_PLANE_SCENE_NODE_BY_CZESTMYR_3573_

#include <irrlicht.h>

namespace irr{
namespace scene{

class CCrossPlaneSceneNode : public ISceneNode{
public:

	CCrossPlaneSceneNode (ISceneNode *parent, ISceneManager *mgr, s32 id=-1, f32 relativeTopWidth = 1.0f, const core::vector3df &position=core::vector3df(0, 0, 0), const core::vector3df &rotation=core::vector3df(0, 0, 0), const core::vector3df &scale=core::vector3df(1.0f, 1.0f, 1.0f));

	~CCrossPlaneSceneNode();

	//void OnPreRender();
	void OnRegisterSceneNode();

	void render();

	const core::aabbox3df& getBoundingBox() const;

        s32 getMaterialCount();

        video::SMaterial& getMaterial(s32 i);

protected:
	video::S3DVertex* plane1;
        video::S3DVertex* plane2;
        u16* indices;
	core::aabbox3d<f32> boundingBox;
        video::SMaterial material;
};

}
} 

#endif
