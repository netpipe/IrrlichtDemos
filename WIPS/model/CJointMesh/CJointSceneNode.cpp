// Copyright (C) 2011 Jaromir Veber

#include "CJointSceneNode.h"

namespace irr
{
namespace scene
{

//! constructor
CJointSceneNode::CJointSceneNode(ISceneNode* parent, ISceneManager* mgr, IJointMesh::SJoint* joint, s32 id)
: IJointSceneNode(parent, mgr, id), MyJoint(joint)
{
	#ifdef _DEBUG
	setDebugName("CJointSceneNode");
	#endif
	setName(MyJoint->getName().c_str());
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CJointSceneNode::getBoundingBox() const
{
	return Box;
}

//! Returns the relative transformation of the scene node.
core::matrix4 CJointSceneNode::getRelativeTransformation() const
{
	return GlobalAnimatedMatrix; // RelativeTransformation;
}

void CJointSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	//IJointSceneNode::serializeAttributes(out, options);
}

void CJointSceneNode::UpdateTransformation()
{
	GlobalAnimatedMatrix = MyJoint->getGlobalAnimatedMatrix();
}

void CJointSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	const core::stringc boneName = in->getAttributeAsString("BoneName");
	setName(boneName);
	//MyJoint = getParent -> getJoint(boneName); //TODO
}


} // namespace scene
} // namespace irr

