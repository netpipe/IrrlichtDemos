// Copyright (C) 2011 Jaromir Veber

#ifndef __C_JOINT_SCENE_NODE_H_INCLUDED__
#define __C_JOINT_SCENE_NODE_H_INCLUDED__

// Used with JointMesh and IAnimatedMeshSceneNode, for boned meshes

#include "IJointSceneNode.h"
#include "IJointMesh.h"

namespace irr
{
namespace scene
{

	class CJointSceneNode : public IJointSceneNode
	{
	public:
		//! constructor
		CJointSceneNode(ISceneNode* parent, ISceneManager* mgr, IJointMesh::SJoint* joint,
			s32 id=-1 );

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! Returns the relative transformation of the scene node.
		virtual core::matrix4 getRelativeTransformation() const;

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

		void UpdateTransformation();
	private:
		IJointMesh::SJoint* MyJoint;
		core::aabbox3d<f32> Box;
		core::matrix4 GlobalAnimatedMatrix;

	};


} // end namespace scene
} // end namespace irr

#endif

