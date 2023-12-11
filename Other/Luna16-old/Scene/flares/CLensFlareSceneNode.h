#ifndef _CLENSFLARESCENENODE_H_INCLUDED_
#define _CLENSFLARESCENENODE_H_INCLUDED_

#include "ILensFlareSceneNode.h"

namespace irr
{
namespace scene
{

/** CLensFlareSceneNode

Extends functionallty to ILensFlareSceneNode

*/
class CLensFlareSceneNode : public ILensFlareSceneNode {
    public:
		// --- constructor ---
        CLensFlareSceneNode(ISceneNode* parent, scene::ISceneManager* mgr,
			s32 id, const core::vector3df& position);

		// --- Returns SMaterial list ---
        virtual video::SMaterial& getMaterial(s32 i);

		// --- Scene creation function ---
        virtual void OnRegisterSceneNode();

		// --- Upon draw ---
        virtual void render();

		// --- returns a bounding box where it is ---
        virtual const core::aabbox3d<f32>& getBoundingBox() const;

		// --- Resets screen size ---
		virtual void setScreenSize(const core::dimension2d<s32> &);

		// --- returns where the camera was created ---
		virtual core::vector3df getStartPosition();

		core::vector3df Posit;
        virtual void setposition(irr::core::vector3df pos);

	//	virtual void setposition(core::vector3df Pos);
    private:

		// --- Size of screen in pixels ---
        core::dimension2d<s32> screensize;

		// --- Bounding box ---
        core::aabbox3d<f32> BBox;

		// --- Internal 'list' of textures used ---
        video::SMaterial material;

		// --- Matrix of each textures position ---
        video::S3DVertex vertices[4];

		// --- Where to draw each node ---
        u16 indices[6];

		// --- starting pos of camera ---
		core::vector3df startPosition;

};

} // end namespace scene
} // end namespace irr
#endif
