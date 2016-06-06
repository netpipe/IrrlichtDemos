// Original code and artwork by Paulo Oliveira (ProSoft, http://br.geocities.com/paulo_cmv/)
// The code is Public Domain.
// With updates from gammaray and torleif

#ifndef _ILENSFLARESCENENODE_H_INCLUDED_
#define _ILENSFLARESCENENODE_H_INCLUDED_

#include "ISceneNode.h"

namespace irr
{
namespace scene
{

/** ILensFlareSceneNode

Creates a line of textures to imitate lens flare

*/
class ILensFlareSceneNode : public ISceneNode {
    public:
	// Constructor
        ILensFlareSceneNode(ISceneNode* parent, ISceneManager* mgr,
			s32 id=-1, const core::vector3df& position=core::vector3df())
		: ISceneNode(parent, mgr)
	{}

	// Returns material
        virtual video::SMaterial& getMaterial(u32 i) = 0;

	// called upon creation
        virtual void OnRegisterSceneNode() = 0;

	// called upon draw
        virtual void render() = 0;

	// reset screen size
	virtual void setScreenSize(const core::dimension2d<s32>& dim) = 0;

	// returns where the camera was created
	virtual const core::vector3df& getStartPosition() = 0;
};


} // end namespace scene
} // end namespace irr

#endif

