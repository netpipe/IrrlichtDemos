// Copyright (C) 2008-2012 Benjamin Hampe
// This file uses parts of the "Irrlicht Engine"
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CNormalSceneNode.h"

#include <irrlicht.h>
#include <iostream>
namespace irr{
namespace scene{

CNormalSceneNode::CNormalSceneNode(
		IMeshSceneNode* src,
		f32 normalLength,
		bool monochrom,
		video::SColor color,
		ISceneNode* parent,
		ISceneManager* mgr,
		s32 id,
		const core::vector3df& position,
		const core::vector3df& rotation,
		const core::vector3df& scale
)
: IMeshSceneNode(parent, mgr, id, position, rotation, scale)
, Mesh(0)
{
	if (!parent)
		setParent(SceneManager->getRootSceneNode());

	if (!src)
	{	std::cout << "CNormalSceneNode::ctr() - ERROR no Source MeshSceneNode"<<std::endl;
		return;
	}

//	SceneManager->getMeshManipulator()->recalculateNormals(src->getMesh(), true, true);

	Mesh = new SMesh();
	if (!Mesh)
	{	std::cout << "CNormalSceneNode::ctr() - ERROR no mesh created"<<std::endl;
		return;
	}

	u32 c = src->getMesh()->getMeshBufferCount();
	std::cout << "MeshBufferCount = "<<c<<std::endl;

	BBox = src->getMesh()->getBoundingBox();

	for (s32 i=0; i<(s32)c; ++i)
	{
		IMeshBuffer* srcBuffer = src->getMesh()->getMeshBuffer(i);
		if (srcBuffer)
		{
			SMeshBuffer* newBuffer = new SMeshBuffer();
			if (newBuffer)
			{
				std::cout << "MeshBuffer("<<i<<") has "<<srcBuffer->getVertexCount()
				<<" Vertices and "<<srcBuffer->getIndexCount()<<" Indices."<<std::endl;

				//! make room for Lines
				newBuffer->Vertices.reallocate(srcBuffer->getVertexCount()*2);
				newBuffer->Vertices.set_used(0);
				newBuffer->Indices.reallocate(srcBuffer->getVertexCount()*2);
				newBuffer->Indices.set_used(0);

				//! Material
				newBuffer->Material.MaterialType = video::EMT_SOLID;
				newBuffer->Material.Lighting = false;
				newBuffer->Material.Wireframe = true;
				newBuffer->Material.ColorMaterial = true;
				newBuffer->Material.FogEnable = false;

				//! Calculate Normals
				core::vector3df p0,n,p1;
				switch (srcBuffer->getVertexType())
				{
					case video::EVT_STANDARD:
					{
						video::S3DVertex* v0 = (video::S3DVertex*)srcBuffer->getVertices();
						for (s32 v=0; v<(s32)srcBuffer->getVertexCount(); ++v)
						{
							p0 = v0->Pos;
							n = v0->Normal;
//							n.normalize();
							p1 = p0 + (n*normalLength);
if (!monochrom)
{
							video::SColorf colorf(n.X, n.Y, n.Z);
							color = colorf.toSColor();
}
							newBuffer->Vertices.push_back(video::S3DVertex(p0.X,p0.Y,p0.Z, 0,0,1, color,0,0));
							newBuffer->Vertices.push_back(video::S3DVertex(p1.X,p1.Y,p1.Z, 0,0,1, color,0,0));
							newBuffer->Indices.push_back((u16)(2*v));
							newBuffer->Indices.push_back((u16)(2*v + 1));
							v0++;
						}
						break;
					}
					case video::EVT_2TCOORDS:
					{
						video::S3DVertex2TCoords* v0 = (video::S3DVertex2TCoords*)srcBuffer->getVertices();
						for (u32 v=0; v<srcBuffer->getVertexCount(); v++)
						{
							p0 = v0->Pos;
							n = v0->Normal;
							p1 = p0 + (n*normalLength);
if (!monochrom)
{
							video::SColorf colorf(n.X, n.Y, n.Z);
							color = colorf.toSColor();
}
							newBuffer->Vertices.push_back(video::S3DVertex(p0.X,p0.Y,p0.Z, 0,0,1, color,0,0));
							newBuffer->Vertices.push_back(video::S3DVertex(p1.X,p1.Y,p1.Z, 0,0,1, color,0,0));
							newBuffer->Indices.push_back(2*v);
							newBuffer->Indices.push_back(2*v + 1);
							v0++;
						}
						break;
					}
					case video::EVT_TANGENTS:
					{
						video::S3DVertexTangents* v0 = (video::S3DVertexTangents*)srcBuffer->getVertices();
						for (u32 v=0; v<srcBuffer->getVertexCount(); v++)
						{
							p0 = v0->Pos;
							n = v0->Normal;
							p1 = p0 + (n*normalLength);
if (!monochrom)
{
							video::SColorf colorf(n.X, n.Y, n.Z);
							color = colorf.toSColor();
}
							newBuffer->Vertices.push_back(video::S3DVertex(p0.X,p0.Y,p0.Z, 0,0,1, color,0,0));
							newBuffer->Vertices.push_back(video::S3DVertex(p1.X,p1.Y,p1.Z, 0,0,1, color,0,0));
							newBuffer->Indices.push_back(2*v);
							newBuffer->Indices.push_back(2*v + 1);
							v0++;
						}
						break;
					}
					default:
						break;
				}

				newBuffer->recalculateBoundingBox();
				Mesh->addMeshBuffer(newBuffer);
				newBuffer->drop();
			}
		}
	}

	AbsoluteTransformation = src->getAbsoluteTransformation();
	Mesh->recalculateBoundingBox();
	setPosition ( src->getPosition() );
	setRotation ( src->getRotation() );
	setScale ( src->getScale() );

}

CNormalSceneNode::~CNormalSceneNode()
{
	if (Mesh)
		Mesh->drop();
}

void
CNormalSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

const core::aabbox3d<f32>&
CNormalSceneNode::getBoundingBox() const
{
	return Mesh->BoundingBox;
}

u32
CNormalSceneNode::getMaterialCount() const
{
	return Mesh->getMeshBufferCount();
}
video::SMaterial&
CNormalSceneNode::getMaterial(u32 i)
{
	return Mesh->getMeshBuffer(i)->getMaterial();
}

void
CNormalSceneNode::render()
{
	if (!Mesh)
		return;

	if (!IsVisible)
		return;

	if (!SceneManager)
		return;

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	for (u32 i=0; i<Mesh->getMeshBufferCount(); i++)
	{
		IMeshBuffer* buffer = Mesh->getMeshBuffer(i);
		if (buffer)
		{
			driver->setMaterial(buffer->getMaterial());
			driver->drawVertexPrimitiveList(
				buffer->getVertices(), buffer->getVertexCount(),
				buffer->getIndices(), buffer->getIndexCount() / 2,
				video::EVT_STANDARD, EPT_LINES,	video::EIT_16BIT);
		}
	}

	if (DebugDataVisible)
	{
		video::SMaterial NullMaterial;
		NullMaterial.ColorMaterial = false;
		NullMaterial.Wireframe=true;
		NullMaterial.Lighting=false;
	//	NullMaterial.DiffuseColor.set(255,255,0,255);

		driver->setMaterial(NullMaterial);
		driver->draw3DBox( Mesh->getBoundingBox(), video::SColor(255,255,0,0));
		driver->draw3DBox( BBox, video::SColor(255,255,0,255));
	}
}

void CNormalSceneNode::setMesh(IMesh* mesh)
{
	if (!mesh)
		return;

	if (mesh == Mesh)
		return;

	if (Mesh)
		Mesh->drop();

//	Mesh = mesh;
}

IMesh* CNormalSceneNode::getMesh(void)
{
	return Mesh;
}

void CNormalSceneNode::setReadOnlyMaterials(bool readonly)
{
	// empty
}

bool CNormalSceneNode::isReadOnlyMaterials() const
{
	return false;
}

} // END NAMSPACE SCENE
} // END NAMSPACE IRR
