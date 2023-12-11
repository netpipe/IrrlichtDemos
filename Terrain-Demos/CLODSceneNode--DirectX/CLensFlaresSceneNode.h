#ifndef _CLENSFLARESSCENENODE_H_INCLUDED_
#define _CLENSFLARESSCENENODE_H_INCLUDED_

#include "irrlicht.h"

using namespace irr;

class CLensFlareSceneNode : public scene::ISceneNode {
private:
    core::dimension2d<s32> screensize;
public:
       
	CLensFlareSceneNode( ISceneNode* parent, scene::ISceneManager* mgr, s32 id, const core::vector3df& position );

	video::SMaterial& getMaterial(s32 i);
	
	int getMaterialCount();

    void OnPreRender();

    void render();

    const core::aabbox3d<f32>& getBoundingBox() const;

private:
	core::aabbox3d<f32> BBox;
	video::SMaterial material;
	video::S3DVertex vertices[4];
	u16 indices[6];
};

#endif
