// Copyright (C) 2011 Jaromir Veber


#ifndef __I_JOINT_SCENE_NODE_H_INCLUDED__
#define __I_JOINT_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"

namespace irr
{
namespace scene
{

	/** \brief Interface for bones used for skeletal animation.
	/** Used to attach other scene nodes like Emitters, Lights etc. to skeletal Meshes */
	class IJointSceneNode : public ISceneNode
	{
	public:
		IJointSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id=-1) :
			ISceneNode(parent, mgr, id) { }

		//! Returns the relative transformation of the scene node.
		virtual core::matrix4 getRelativeTransformation() const = 0;

		//! The render method.
		/** Does nothing as bones are not visible. */
		virtual void render() { }

		//! No animator allowed for Joint nodes
		virtual void addAnimator (ISceneNodeAnimator *animator) { }

		//! Type of this node - Empty or make new type - Joint
		virtual ESCENE_NODE_TYPE getType () const { return ESNT_EMPTY; }

		//! Should be implemented
		virtual const core::aabbox3d<f32>& getBoundingBox() const = 0;

		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const = 0;

		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0) = 0;
	};


} // end namespace scene
} // end namespace irr

#endif

