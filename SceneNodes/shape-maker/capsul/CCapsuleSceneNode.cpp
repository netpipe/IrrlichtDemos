// Copyright (C) 2008-2012 Benjamin Hampe
// This file uses parts of the "Irrlicht Engine"
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CCapsuleSceneNode.h"
#include "CNormalSceneNode.h"

#include <irrlicht.h>

namespace irr{
namespace scene{

CCapsuleSceneNode::CCapsuleSceneNode(ISceneManager* mgr, ISceneNode* parent, f32 radius, f32 height, u32 cols, u32 rows)
: ISceneNode(parent, mgr, -1, core::vector3df(0,0,0), core::vector3df(0,0,0), core::vector3df(1,1,1))
{
	Center = this->createCylinderMeshBuffer(radius, height, cols);
	Top = this->createUpperSphereHalfMeshBuffer(radius, cols, rows, core::vector3df(0.f,.5f*height,0.f));
	Bottom = this->createLowerSphereHalfMeshBuffer(radius, cols, rows, core::vector3df(0.f,-.5f*height,0.f));

	VertexCount = 0;
	IndexCount = 0;

	if (Top)
	{
		//Top->Material.setTexture(0, SceneManager->getVideoDriver()->getTexture("../media/ubuntu_logo128.png"));
		BoundingBox.addInternalBox(Top->BoundingBox);
		VertexCount+=Top->Vertices.size();
		IndexCount+=Top->Indices.size();
//		calculateNormals(Top);
	}

	if (Center)
	{
		//Center->Material.setTexture(0, SceneManager->getVideoDriver()->getTexture("../media/irrlicht_logo.png"));
		BoundingBox.addInternalBox(Center->BoundingBox);
		VertexCount+=Center->Vertices.size();
		IndexCount+=Center->Indices.size();
//		calculateNormals(Center);
	}

	if (Bottom)
	{
		//Bottom->Material.setTexture(0, SceneManager->getVideoDriver()->getTexture("../media/Windows128.png"));
		BoundingBox.addInternalBox(Bottom->BoundingBox);
		VertexCount+=Bottom->Vertices.size();
		IndexCount+=Bottom->Indices.size();
//		calculateNormals(Bottom);
	}

	setDebugDataVisible( EDS_BBOX | EDS_NORMALS );
}

CCapsuleSceneNode::~CCapsuleSceneNode()
{
	if (Top)
		Top->drop();
	if (Center)
		Center->drop();
	if (Bottom)
		Bottom->drop();
	if (TopNormals)
		TopNormals->drop();
	if (CenterNormals)
		CenterNormals->drop();
	if (BottomNormals)
		BottomNormals->drop();
}

void
CCapsuleSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

const core::aabbox3d<f32>&
CCapsuleSceneNode::getBoundingBox() const
{
	return BoundingBox;
}

u32
CCapsuleSceneNode::getMaterialCount() const
{
	return 3;
}
video::SMaterial&
CCapsuleSceneNode::getMaterial(u32 i)
{
	if (i>2) i %= 3;

	if (i==0)
		return Top->Material;
	else if (i==1)
		return Center->Material;
	else
		return Bottom->Material;
}

void
CCapsuleSceneNode::render()
{
	if (!IsVisible)
		return;

	if (!SceneManager)
		return;

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!driver)
		return;

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	if (Top)
	{
		driver->setMaterial(Top->Material);
		driver->drawMeshBuffer(Top);
//		driver->draw3DBox(Top->BoundingBox,0xFF000000);
	}
	if (Center)
	{
		driver->setMaterial(Center->Material);
		driver->drawMeshBuffer(Center);
//		driver->draw3DBox(Center->BoundingBox,0xFFFF0000);
	}
	if (Bottom)
	{
		driver->setMaterial(Bottom->Material);
		driver->drawMeshBuffer(Bottom);
//		driver->draw3DBox(Bottom->BoundingBox,0xFFFFFF00);
	}

//	if (DebugDataVisible)
//	{
//		if (Top)
//		{
//			if (!TopNormals)
//				TopNormals = createNormalMeshBuffer(Top);
//
//			if (TopNormals)
//				renderNormalMeshBuffer(TopNormals);
//		}
//
//		if (Center)
//		{
//			if (!CenterNormals)
//				TopNormals = createNormalMeshBuffer(Center);
//
//			if (CenterNormals)
//				renderNormalMeshBuffer(CenterNormals);
//		}
//
//		if (Bottom)
//		{
//			if (!BottomNormals)
//				BottomNormals = createNormalMeshBuffer(Bottom);
//
//			if (BottomNormals)
//				renderNormalMeshBuffer(BottomNormals);
//		}
//
//	}
//	driver->draw3DBox(BoundingBox,0xFFFFFFFF);

	core::stringw info(L"");
	info += L"Vertices = ";	info += VertexCount;
	info += L"\nIndices = "; info += IndexCount;

	core::position2di pos = SceneManager->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(getAbsolutePosition());
	core::recti TextRect = core::recti(0,0,100,20)+pos-core::position2di(0,10);

	driver->draw2DRectangle(0x7F0000FF,TextRect);

	SceneManager->getGUIEnvironment()->getBuiltInFont()->draw(
		info.c_str(),
		TextRect,
		0xFFFFFFFF, true,true);

}

SMeshBuffer*
CCapsuleSceneNode::createCylinderMeshBuffer(f32 radius, f32 height, u32 cols, const core::vector3df& pos)
{
	SMeshBuffer* buffer = new SMeshBuffer();
	if (!buffer)
		return 0;

	// set Material
	buffer->Material.MaterialType = video::EMT_SOLID;
	buffer->Material.Wireframe = true;
	buffer->Material.Lighting = false;
	buffer->Material.DiffuseColor = 0x7FFFFFFF;
	buffer->Material.BackfaceCulling = true;

	buffer->Vertices.clear();
	buffer->Vertices.reallocate(2*(cols+1));
	buffer->Vertices.set_used(0);

	buffer->Indices.clear();
	buffer->Indices.reallocate(6*cols);
	buffer->Indices.set_used(0);

	// calculate Vertices
	f32 x,y,z,u,s,c,phi,step;
	y = pos.Y + 0.5f*height;
	phi = 2.0f*core::PI / (f32)cols;
	step = 1.0f / (f32)cols;

	for (u32 i=0; i<cols+1; i++)
	{
		s = sinf((f32)i*phi);
		c = cosf((f32)i*phi);
		x = pos.X + radius*s;
		z = pos.Z - radius*c;
		u = (f32)i*step;// [0,1]

		buffer->Vertices.push_back(video::S3DVertex(x,-y,z,0,1,0,0x7FFFFFFF,u,1.0f));
		buffer->Vertices.push_back(video::S3DVertex(x, y,z,0,1,0,0x7FFFFFFF,u,0.0f));
	}

	// calculate Indices
	for (u32 i=0; i<cols; i++)
	{
		buffer->Indices.push_back(2*i+0);
		buffer->Indices.push_back(2*i+1);
		buffer->Indices.push_back(2*i+3);
		buffer->Indices.push_back(2*i+0);
		buffer->Indices.push_back(2*i+3);
		buffer->Indices.push_back(2*i+2);
	}
	// calculate BoundingBox
	buffer->BoundingBox.reset(core::vector3df(0,0,0));
	for (u32 i=0; i<buffer->Vertices.size(); i++)
		buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);

	return buffer;
}

//SMeshBuffer*
//createUpperSphereHalfMeshBuffer(f32 radius, u32 cols, u32 rows, const core::vector3df& pos = core::vector3df(0,0,0));

SMeshBuffer*
CCapsuleSceneNode::createUpperSphereHalfMeshBuffer(f32 radius, u32 cols, u32 rows, const core::vector3df& pos)
{
	SMeshBuffer* buffer = new SMeshBuffer();
	if (!buffer)
		return 0;

	// set Material
	buffer->Material.MaterialType = video::EMT_SOLID;
	buffer->Material.Wireframe = true;
	buffer->Material.Lighting = false;
	buffer->Material.DiffuseColor = 0x7FFFFFFF;
	buffer->Material.BackfaceCulling = true;

	// make space for vertices
	buffer->Vertices.clear();
//	buffer->Vertices.reallocate(cols*rows+1);
	buffer->Vertices.set_used(0);

	// make space for indices
	buffer->Indices.clear();
//	buffer->Indices.reallocate(6*cols*rows);
	buffer->Indices.set_used(0);

	// calculate Vertices
	f32 x,y,z,u,v,alpha,phi;
	alpha = 2.0f*core::PI / (f32)cols; // [0°..360°]
	phi = 0.5f* core::PI / (f32)rows;  // [0°..90°] // shall begin on bottom

	for (u32 j=0; j<rows; ++j)
	{
		for (u32 i=0; i<cols; ++i)
		{
			const f32 sinPhi = sinf(phi * (f32)j);
			const f32 cosPhi = cosf(phi * (f32)j);
			const f32 sinAlpha = sinf(alpha * (f32)i);
			const f32 cosAlpha = cosf(alpha * (f32)i);
			x = pos.X + radius * cosPhi * sinAlpha;
			y = pos.Y + radius * sinPhi;
			z = pos.Z - radius * cosPhi * cosAlpha;
			u = 0.5f+0.5f*cosPhi * sinAlpha; // not finished
			v = 0.5f-0.5f*cosPhi * cosAlpha; // not finished
			buffer->Vertices.push_back(video::S3DVertex(x,y,z,0.f,1.f,0.f,0x7FFFFFFF,u,v));
		}
	}
	// add top center vertex
	buffer->Vertices.push_back(video::S3DVertex(pos.X,pos.Y+radius,pos.Z,0.f,1.f,0.f,0x7FFFFFFF,0.5f,0.5f));

	// calculate Indices
	for (u32 j=0; j<rows; j++)
	{
		for (u32 i=0; i<cols; i++)
		{
			// Top just triangles
			if (j==rows-1)
			{
				s32 i0; // A
				s32 i1; // B
				s32 i2 = buffer->Vertices.size()-1; // C - top center

				// border
				if (i==cols-1)
				{
					i0 = (j)*cols + (i);	// A
					i1 = (j)*cols + (0);	// B
				}
				// normal
				else
				{
					i0 = (j)*cols + (i);	// A
					i1 = (j)*cols + (i+1);	// B
				}
				buffer->Indices.push_back((u16)i0); // A
				buffer->Indices.push_back((u16)i2); // C
				buffer->Indices.push_back((u16)i1); // B
			}
			// Bottom to Top-1 consist each of 2 triangles
			else
			{
				s32 i0;	// A
				s32 i1;	// B
				s32 i2;	// C
				s32 i3;	// D

				// border
				if (i==cols-1)
				{
					i0 = (j)*cols + (i);	// A
					i1 = (j+1)*cols + (i);	// B
					i2 = (j+1)*cols + (0);	// C
					i3 = (j)*cols + (0);	// D
				}
				// normal
				else
				{
					i0 = (j)*cols + (i);	// A
					i1 = (j+1)*cols + (i);	// B
					i2 = (j+1)*cols + (i+1);// C
					i3 = (j)*cols + (i+1);	// D
				}
				buffer->Indices.push_back((u16)i0); // A
				buffer->Indices.push_back((u16)i1); // B
				buffer->Indices.push_back((u16)i2); // C
				buffer->Indices.push_back((u16)i0); // A
				buffer->Indices.push_back((u16)i2); // C
				buffer->Indices.push_back((u16)i3); // D
			}
		}
	}

	// calculate BoundingBox
	buffer->BoundingBox.reset(core::vector3df(0,0,0));
	for (u32 i=0; i<buffer->Vertices.size(); i++)
		buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);

	return buffer;
}

SMeshBuffer*
CCapsuleSceneNode::createLowerSphereHalfMeshBuffer(f32 radius, u32 cols, u32 rows, const core::vector3df& pos)
{
	SMeshBuffer* buffer = new SMeshBuffer();
	if (!buffer)
		return 0;

	// set Material
	buffer->Material.MaterialType = video::EMT_SOLID;
	buffer->Material.Wireframe = true;
	buffer->Material.Lighting = false;
	buffer->Material.DiffuseColor = 0x7FFFFFFF;
	buffer->Material.BackfaceCulling = true;

	buffer->Vertices.clear();
//	buffer->Vertices.reallocate(cols*rows+1);
	buffer->Vertices.set_used(0);

	buffer->Indices.clear();
//	buffer->Indices.reallocate(6*cols*rows);
	buffer->Indices.set_used(0);

	// calculate Vertices
	f32 x,y,z,u,v,alpha,phi;
	alpha = 2.0f*core::PI / (f32)cols; // [0°..360°]
	phi = 0.5f* core::PI / (f32)rows;  // [0°..90°] // shall begin on bottom

	for (u32 j=0; j<rows; ++j)
	{
		for (u32 i=0; i<cols; ++i)
		{
			const f32 sinPhi = sinf(phi * (f32)j);
			const f32 cosPhi = cosf(phi * (f32)j);
			const f32 sinAlpha = sinf(alpha * (f32)i);
			const f32 cosAlpha = cosf(alpha * (f32)i);
			x = pos.X + radius * cosPhi * sinAlpha;
			y = pos.Y - radius * sinPhi;
			z = pos.Z - radius * cosPhi * cosAlpha;
			u = 0.5f+0.5f*cosPhi * sinAlpha;
			v = 0.5f-0.5f*cosPhi * cosAlpha;
			buffer->Vertices.push_back(video::S3DVertex(x,y,z,0.f,1.f,0.f,0x7FFFFFFF,u,v));
		}
	}
	// add top center vertex
	buffer->Vertices.push_back(video::S3DVertex(pos.X,pos.Y-radius,pos.Z,0.f,1.f,0.f,0x7FFFFFFF,0.5f,0.5f));

	// calculate Indices
	for (u32 j=0; j<rows; j++)
	{
		for (u32 i=0; i<cols; i++)
		{
			// Top just triangles
			if (j==rows-1)
			{
				s32 i0; // A
				s32 i1; // B
				s32 i2 = buffer->Vertices.size()-1; // C - top center

				// border
				if (i==cols-1)
				{
					i0 = (j)*cols + (i);	// A
					i1 = (j)*cols + (0);	// B
				}
				// normal
				else
				{
					i0 = (j)*cols + (i);	// A
					i1 = (j)*cols + (i+1);	// B
				}
				buffer->Indices.push_back((u16)i1); // A
				buffer->Indices.push_back((u16)i2); // C
				buffer->Indices.push_back((u16)i0); // B
			}
			// Bottom to Top-1 consist each of 2 triangles
			else
			{
				s32 i0;	// A
				s32 i1;	// B
				s32 i2;	// C
				s32 i3;	// D

				// border
				if (i==cols-1)
				{
					i0 = (j)*cols + (i);	// A
					i1 = (j+1)*cols + (i);	// B
					i2 = (j+1)*cols + (0);	// C
					i3 = (j)*cols + (0);	// D
				}
				// normal
				else
				{
					i0 = (j)*cols + (i);	// A
					i1 = (j+1)*cols + (i);	// B
					i2 = (j+1)*cols + (i+1);// C
					i3 = (j)*cols + (i+1);	// D
				}
				buffer->Indices.push_back((u16)i2); // A
				buffer->Indices.push_back((u16)i1); // B
				buffer->Indices.push_back((u16)i0); // C
				buffer->Indices.push_back((u16)i3); // A
				buffer->Indices.push_back((u16)i2); // C
				buffer->Indices.push_back((u16)i0); // D
			}
		}
	}

	// calculate BoundingBox
	buffer->BoundingBox.reset(core::vector3df(0,0,0));
	for (u32 i=0; i<buffer->Vertices.size(); i++)
		buffer->BoundingBox.addInternalPoint(buffer->Vertices[i].Pos);

	return buffer;
}

} // END NAMSPACE SCENE
} // END NAMSPACE IRR
