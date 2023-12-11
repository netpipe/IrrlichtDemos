#ifndef _SCENENODEANIMATORFOLLOWCAMERA_H
#define _SCENENODEANIMATORFOLLOWCAMERA_H

#include <irrlicht.h>

namespace irr
{
namespace scene
{

	class CSceneNodeAnimatorFollowCamera : public ISceneNodeAnimator
	{
	public:
		// constructor
		CSceneNodeAnimatorFollowCamera(const core::vector3df& startPosition);

		// animates a scene node
		virtual void animateNode(ISceneNode* node, u32 timeMs);

		// writes attributes of the scene node animator.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

		// reads attributes of the scene node animator.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

		// returns type of the scene node animator
		virtual ESCENE_NODE_ANIMATOR_TYPE getType() const;
		
		// creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);

		// returns the start position
		core::vector3df& getStartPosition() { return StartPosition; }
		
		// sets the start position
		void setStartPosition(core::vector3df& startPosition) { StartPosition = startPosition; }

	private:
		// start position
		core::vector3df StartPosition;
	};


} // end namespace scene
} // end namespace irr

#endif

