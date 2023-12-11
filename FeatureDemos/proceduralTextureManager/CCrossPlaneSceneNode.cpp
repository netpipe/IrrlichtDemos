// Copyright (C) 2006 Cestmir "CZestmyr" Houska
// This file uses the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "CCrossPlaneSceneNode.h"

namespace irr{
namespace scene{

CCrossPlaneSceneNode::CCrossPlaneSceneNode (ISceneNode *parent, ISceneManager *mgr, s32 id, f32 relativeTopWidth, const core::vector3df &position, const core::vector3df &rotation, const core::vector3df &scale) :
ISceneNode (parent, mgr, id, position, rotation, scale)
{
	plane1 = new video::S3DVertex[4];
	plane2 = new video::S3DVertex[4];
	indices = new u16[12];
	relativeTopWidth /= 2;

	material.Lighting = false;
	material.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;

	boundingBox = core::aabbox3d<f32>(-0.5f, 0, -0.5f, 0.5f, 1.0f, 0.5f);

	video::SColor white = video::SColor(255, 255, 255, 255);

	plane1[0] = video::S3DVertex(0, 0, -0.5f, 0, 0, 0, white, 0.1f, 0.95f);
	plane1[1] = video::S3DVertex(0, 1.0f, -relativeTopWidth, 0, 0, 0, white, 0.1f, 0.1f);
	plane1[2] = video::S3DVertex(0, 1.0f, relativeTopWidth, 0, 0, 0, white, 0.95f, 0.1f);
	plane1[3] = video::S3DVertex(0, 0, 0.5f, 0, 0, 0, white, 0.95f, 0.95f);
	plane2[0] = video::S3DVertex(-0.5f, 0, 0, 0, 0, 0, white, 0.1f, 0.95f);
	plane2[1] = video::S3DVertex(-relativeTopWidth, 1.0f, 0, 0, 0, 0, white, 0.1f, 0.1f);
	plane2[2] = video::S3DVertex(relativeTopWidth, 1.0f, 0, 0, 0, 0, white, 0.95f, 0.1f);
	plane2[3] = video::S3DVertex(0.5f, 0, 0, 0, 0, 0, white, 0.95f, 0.95f);
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;
}

CCrossPlaneSceneNode::~CCrossPlaneSceneNode()
{
	delete[] plane1;
	delete[] plane2;
	delete[] indices;
}

//void CCrossPlaneSceneNode::OnPreRender(){
void CCrossPlaneSceneNode::OnRegisterSceneNode(){
	if (IsVisible == true)
		SceneManager->registerNodeForRendering(this);
	ISceneNode::OnRegisterSceneNode();
}

void CCrossPlaneSceneNode::render(){
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	driver->setMaterial(material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawIndexedTriangleList(&plane1[0], 4, &indices[0], 2);
	driver->drawIndexedTriangleList(&plane2[0], 4, &indices[0], 2);
}

const core::aabbox3df& CCrossPlaneSceneNode::getBoundingBox() const
{
	return boundingBox;
}

s32 CCrossPlaneSceneNode::getMaterialCount(){
	return 1;
}

video::SMaterial& CCrossPlaneSceneNode::getMaterial(s32 i){
	return material;
}

}
}
