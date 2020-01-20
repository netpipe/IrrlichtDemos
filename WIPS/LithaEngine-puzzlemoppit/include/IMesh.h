
#ifndef I_MESH_H
#define I_MESH_H

#include "IVisibleGraphic.h"

class IMesh : public IVisibleGraphic
{
public:
	virtual ~IMesh(){}
	
	// Could be moved up to IVisibleGraphic??
	// Note that Scale() is applied to this bounding box internally.
	// (as scale is considered permanent, unlike in Irrlicht where the BB is completely untransformed)
	virtual core::aabbox3df GetBoundingBox() = 0;
	
	// Adds another rendering pass, using the materials of the specified mesh.
	// Note that the mesh specified here should have the same number of materials as
	// the original mesh. Mesh geometry is ignored (pass will be rendered with the geometry of
	// the original mesh).
	// (Irrlicht doesn't really have a separate materials file format so we just use a mesh)
	virtual void AddPass(const c8 *meshName) = 0;
	
	// an animation is: {start,end,FPS,looped?,interruptible?}
	// id is chosen by you. Sensible to use an enum, e.g. enum { IDLE,WALK,etc...}
	// NOTE: Some artists may assume animation starts at 1, but Irrlicht assumes it starts at 0.
	// (so you may need to subtract 1 from animation ranges)
	virtual void DefineAnimation(s32 id, u32 start, u32 end, f32 fps, bool looped, bool interruptible,
			f32 transitionTime = 0.1) = 0;
	
	// Set an animation defined with DefineAnimation.
	// This may fail if an animation that cannot be interrupted is currently playing.
	// If the animation specified is already being played nothing will happen.
	virtual void SetAnimation(s32 id) = 0;
	
	// Is this animation playing?
	// Not ended or been interrupted.
	virtual bool IsPlayingAnimation(s32 id) = 0;
	
	// Permanently stops any currently playing animation.
	//virtual void HaltAnimation() = 0;
	
	// Apply a permanent scale to the mesh.
	// (this will also scale the bounding box)
	virtual void Scale(const core::vector3df &scale) = 0;
	void Scale(f32 scale) { Scale(core::vector3df(scale)); }
	
	// Apply a permanent rotation to the mesh.
	// NOTE: This is not yet taken into account when generating physics geometry.
	// It's on the TODO list.
	virtual void Rotate(const core::vector3df &rotation) = 0;
	
	// Apply a permanent translation to the mesh.
	// This is not yet taken into account for physics geometry either.
	virtual void Translate(const core::vector3df &translation) = 0;
	
	// THIS FUNCTION SHOULD BE REMOVED IN THE FUTURE!!?
	// This class wraps an Irrlicht Animated Mesh scene node.
	// Don't try to modify the node or strange things may happen!
	// ESPECIALLY do not modify, add, or remove the node's materials. Material modifications
	// must be done through the IGraphic API.
	virtual scene::IAnimatedMeshSceneNode *GetIrrlichtNode() = 0;
};

#endif
