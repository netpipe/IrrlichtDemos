// Original code and artwork by Paulo Oliveira (ProSoft, http://br.geocities.com/paulo_cmv/)
// The code is Public Domain.
// With updates from gammaray and torleif

#ifndef _CLENSFLARESCENENODE_H_INCLUDED_
#define _CLENSFLARESCENENODE_H_INCLUDED_

#include "ILensFlareSceneNode.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{

/** CLensFlareSceneNode

Extends functionality to ILensFlareSceneNode

 */

class CLensFlareSceneNode : public ILensFlareSceneNode
{
public:
	// constructor
	CLensFlareSceneNode(ISceneNode* parent, scene::ISceneManager* mgr,
			s32 id=-1, const core::vector3df& position=core::vector3df());

//	virtual u32 getMaterialCount() const;

	// Returns SMaterial list
	virtual video::SMaterial& getMaterial(u32 i);

	// Scene creation function
	virtual void OnRegisterSceneNode();

	// Upon draw
	virtual void render();

	// returns a bounding box where it is
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	// Resets screen size
	virtual void setScreenSize(const core::dimension2d<s32> &);

	// returns where the camera was created
	virtual const core::vector3df& getStartPosition();

private:
	// Size of screen in pixels
	core::dimension2d<s32> Screensize;

	// starting pos of camera
	core::vector3df StartPosition;

	// Bounding box
	core::aabbox3d<f32> BBox;

	// Internal 'list' of textures used
	video::SMaterial Material;

	// Matrix of each textures position
	video::S3DVertex vertices[4];

	// Where to draw each node
	u16 indices[6];
};


} // end namespace scene
} // end namespace irr

#endif

