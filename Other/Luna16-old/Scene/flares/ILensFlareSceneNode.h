#ifndef _ILENSFLARESCENENODE_H_INCLUDED_
#define _ILENSFLARESCENENODE_H_INCLUDED_
#include <irrlicht.h>

namespace irr
{
namespace scene
{

/** ILensFlareSceneNode

Creates a line of textures to imitate camera glare

*/
class ILensFlareSceneNode : public ISceneNode {
    public:
		// --- Constructor ---
        ILensFlareSceneNode(ISceneNode* parent, ISceneManager* mgr,
			s32 id, const core::vector3df& position):ISceneNode(parent, mgr){}

		// --- Returns texture list ---
        virtual video::SMaterial& getMaterial(s32 i) = 0;

		// --- called upon creation ---
        virtual void OnRegisterSceneNode() = 0;

		// --- called upon draw ---
        virtual void render() = 0;

		// --- reset screen size ---
		virtual void setScreenSize(const core::dimension2d<s32> &) = 0;

		// --- returns where the camera was created ---

		virtual void setposition(core::vector3df Pos)= 0;

        virtual core::vector3df getStartPosition() = 0;
};

} // end namespace scene
} // end namespace irr
#endif
