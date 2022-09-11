// Copyright (C) 2002-2011 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CSphericalTerrainSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "S3DVertex.h"
#include "SMeshBuffer.h"
#include "SMesh.h"

namespace irr
{
namespace scene
{

//! constructor
CSphericalTerrainSceneNode::CSphericalTerrainSceneNode(const io::path& heightMapFileName, ISceneManager* mgr, f32 radius, f32 maxHeight, ISceneNode* parent, s32 id,
			const core::vector3df& position, const core::vector3df& rotation, const core::vector3df& scale)
: IMeshSceneNode(parent, mgr, id, position, rotation, scale), Mesh(0), MaxHeight(maxHeight),
	Radius(radius)
{
	#ifdef _DEBUG
	setDebugName("CSphericalTerrainSceneNode");
	#endif

	io::IReadFile* file = SceneManager->getFileSystem()->createAndOpenFile(heightMapFileName);
	HeightmapFile = file->getFileName();

	if(!file)
	{
//		os::Printer::log("Could not load spherical terrain, because file could not be opened.",
//		heightMapFileName, ELL_ERROR); ilogger
	}
    else
        Mesh = createSphericalTerrainMesh(SceneManager->getVideoDriver()->createImageFromFile(file), radius);

    if(file)
		file->drop();
}

CSphericalTerrainSceneNode::CSphericalTerrainSceneNode(io::IReadFile* heightMapFile, ISceneManager* mgr, f32 radius, f32 maxHeight, ISceneNode* parent, s32 id,
			const core::vector3df& position, const core::vector3df& rotation, const core::vector3df& scale)
: IMeshSceneNode(parent, mgr, id, position, rotation, scale), Mesh(0), MaxHeight(maxHeight),
	Radius(radius)
{
	#ifdef _DEBUG
	setDebugName("CSphericalTerrainSceneNode");
	#endif

    HeightmapFile = heightMapFile->getFileName();
	if(!heightMapFile)
	{
		//os::Printer::log("Could not load spherical terrain, because file could not be opened.", ELL_ERROR);
	}
	else
        Mesh = createSphericalTerrainMesh(SceneManager->getVideoDriver()->createImageFromFile(heightMapFile), radius);
}

//! destructor
CSphericalTerrainSceneNode::~CSphericalTerrainSceneNode()
{
	if (Mesh)
		Mesh->drop();
}


//! renders the node.
void CSphericalTerrainSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (Mesh && driver)
	{
		driver->setMaterial(Mesh->getMeshBuffer(0)->getMaterial());
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawMeshBuffer(Mesh->getMeshBuffer(0));
		if ( DebugDataVisible & scene::EDS_BBOX )
		{
			video::SMaterial m;
			m.Lighting = false;
			driver->setMaterial(m);
			driver->draw3DBox(Mesh->getMeshBuffer(0)->getBoundingBox(), video::SColor(255,255,255,255));
		}
	}
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CSphericalTerrainSceneNode::getBoundingBox() const
{
	return Mesh ? Mesh->getBoundingBox() : Box;
}


void CSphericalTerrainSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CSphericalTerrainSceneNode::getMaterial(u32 i)
{
	if (i>0 || !Mesh)
		return ISceneNode::getMaterial(i);
	else
		return Mesh->getMeshBuffer(i)->getMaterial();
}

//! returns amount of materials used by this scene node.
u32 CSphericalTerrainSceneNode::getMaterialCount() const
{
	return 1;
}


//! Writes attributes of the scene node.
void CSphericalTerrainSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	ISceneNode::serializeAttributes(out, options);

	out->addFloat("Radius", Radius);
	out->addInt("PolyCountX", PolyCountX);
	out->addInt("PolyCountY", PolyCountY);
}


//! Reads attributes of the scene node.
void CSphericalTerrainSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	f32 oldRadius = Radius;
	u32 oldPolyCountX = PolyCountX;
	u32 oldPolyCountY = PolyCountY;

	Radius = in->getAttributeAsFloat("Radius");
	PolyCountX = in->getAttributeAsInt("PolyCountX");
	PolyCountY = in->getAttributeAsInt("PolyCountY");
	// legacy values read for compatibility with older versions
	u32 polyCount = in->getAttributeAsInt("PolyCount");
	if (PolyCountX ==0 && PolyCountY == 0)
		PolyCountX = PolyCountY = polyCount;

	Radius = core::max_(Radius, 0.0001f);

	if ( !core::equals(Radius, oldRadius) || PolyCountX != oldPolyCountX || PolyCountY != oldPolyCountY)
	{
		if (Mesh)
			Mesh->drop();

        io::IReadFile* file = SceneManager->getFileSystem()->createAndOpenFile(HeightmapFile);
        Mesh = createSphericalTerrainMesh(SceneManager->getVideoDriver()->createImageFromFile(file), Radius);
        if(file)
            file->drop();
	}

	ISceneNode::deserializeAttributes(in, options);
}

//! Creates a clone of this scene node and its children.
ISceneNode* CSphericalTerrainSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	if (!newParent)
		newParent = Parent;
	if (!newManager)
		newManager = SceneManager;

    io::IReadFile* file = SceneManager->getFileSystem()->createAndOpenFile(HeightmapFile.c_str());
	CSphericalTerrainSceneNode* nb = new CSphericalTerrainSceneNode(file, newManager, Radius, MaxHeight, newParent,		ID, RelativeTranslation);
    if(file)
    {
        file->drop();
    }

	nb->cloneMembers(this, newManager);
	nb->getMaterial(0) = Mesh->getMeshBuffer(0)->getMaterial();

	if ( newParent )
		nb->drop();
	return nb;
}

void CSphericalTerrainSceneNode::setMaxHeight(f32 maxHeight)
{
    MaxHeight = maxHeight;
    if(Mesh)
        Mesh->drop();

    io::IReadFile* file = SceneManager->getFileSystem()->createAndOpenFile(HeightmapFile);
    Mesh = createSphericalTerrainMesh(SceneManager->getVideoDriver()->createImageFromFile(file), Radius);
    if(file)
        file->drop();
}

IMesh* CSphericalTerrainSceneNode::createSphericalTerrainMesh(video::IImage* heightMap, f32 radius)
{
	// thanks to Alfaz93 who made his code available for Irrlicht on which
	// this one is based!

	// we are creating the sphere mesh here.

	if (!heightMap)
        return 0;

    const core::dimension2d<u32> hMapSize= heightMap->getDimension();

    u32 polyCountX = hMapSize.Width-1;
    u32 polyCountY = hMapSize.Height-1;
    f32 maxHeight = MaxHeight;
    maxHeight /= 255.0f;

	if (polyCountX < 2)
		polyCountX = 2;
	if (polyCountY < 2)
		polyCountY = 2;
	while (polyCountX * polyCountY > 32767) // prevent u16 overflow
	{
		polyCountX /= 2;
		polyCountY /= 2;
	}

	const u32 polyCountXPitch = polyCountX+1; // get to same vertex on next level

	SMeshBuffer* buffer = new SMeshBuffer();

	buffer->Indices.reallocate((polyCountX * polyCountY) * 6);

	const video::SColor clr(100, 255,255,255);

	u32 level = 0;

	for (u32 p1 = 0; p1 < polyCountY-1; ++p1)
	{
		//main quads, top to bottom
		for (u32 p2 = 0; p2 < polyCountX - 1; ++p2)
		{
			const u32 curr = level + p2;
			buffer->Indices.push_back(curr + polyCountXPitch);
			buffer->Indices.push_back(curr);
			buffer->Indices.push_back(curr + 1);
			buffer->Indices.push_back(curr + polyCountXPitch);
			buffer->Indices.push_back(curr+1);
			buffer->Indices.push_back(curr + 1 + polyCountXPitch);
		}

		// the connectors from front to end
		buffer->Indices.push_back(level + polyCountX - 1 + polyCountXPitch);
		buffer->Indices.push_back(level + polyCountX - 1);
		buffer->Indices.push_back(level + polyCountX);

		buffer->Indices.push_back(level + polyCountX - 1 + polyCountXPitch);
		buffer->Indices.push_back(level + polyCountX);
		buffer->Indices.push_back(level + polyCountX + polyCountXPitch);
		level += polyCountXPitch;
	}

	const u32 polyCountSq = polyCountXPitch * polyCountY; // top point
	const u32 polyCountSq1 = polyCountSq + 1; // bottom point
	const u32 polyCountSqM1 = (polyCountY - 1) * polyCountXPitch; // last row's first vertex

	for (u32 p2 = 0; p2 < polyCountX - 1; ++p2)
	{
		// create triangles which are at the top of the sphere

		buffer->Indices.push_back(polyCountSq);
		buffer->Indices.push_back(p2 + 1);
		buffer->Indices.push_back(p2);

		// create triangles which are at the bottom of the sphere

		buffer->Indices.push_back(polyCountSqM1 + p2);
		buffer->Indices.push_back(polyCountSqM1 + p2 + 1);
		buffer->Indices.push_back(polyCountSq1);
	}

	// create final triangle which is at the top of the sphere

	buffer->Indices.push_back(polyCountSq);
	buffer->Indices.push_back(polyCountX);
	buffer->Indices.push_back(polyCountX-1);

	// create final triangle which is at the bottom of the sphere

	buffer->Indices.push_back(polyCountSqM1 + polyCountX - 1);
	buffer->Indices.push_back(polyCountSqM1);
	buffer->Indices.push_back(polyCountSq1);

	// calculate the angle which separates all points in a circle
	const f64 AngleX = 2 * core::PI / polyCountX;
	const f64 AngleY = core::PI / polyCountY;

	u32 i=0;
	f64 axz;

	// we don't start at 0.

	f64 ay = 0;//AngleY / 2;

	buffer->Vertices.set_used((polyCountXPitch * polyCountY) + 2);
	for (u32 y = 0; y < polyCountY; ++y)
	{
		ay += AngleY;
		const f64 sinay = sin(ay);
		axz = 0;

		// calculate the necessary vertices without the doubled one
		for (u32 xz = 0;xz < polyCountX; ++xz)
		{
		    // get height
            f32 height = heightMap->getPixel(xz, y).getAverage() * maxHeight;

			// calculate points position

            core::vector3df pos(static_cast<f32>(radius * cos(axz) * sinay),
						static_cast<f32>(radius * cos(ay)),
						static_cast<f32>(radius * sin(axz) * sinay));
            pos.setLength(radius + height);

			// for spheres the normal is the position
			core::vector3df normal(pos);
			normal.normalize();

			// calculate texture coordinates via sphere mapping
			// tu is the same on each level, so only calculate once
			f32 tu = 0.5f;
			if (y==0)
			{
				if (normal.Y != -1.0f && normal.Y != 1.0f)
					tu = static_cast<f32>(acos(core::clamp(normal.X/sinay, -1.0, 1.0)) * 0.5 *core::RECIPROCAL_PI64);
				if (normal.Z < 0.0f)
					tu=1-tu;
			}
			else
				tu = buffer->Vertices[i-polyCountXPitch].TCoords.X;
			buffer->Vertices[i] = video::S3DVertex(pos.X, pos.Y, pos.Z,
						normal.X, normal.Y, normal.Z,
						clr, tu,
						static_cast<f32>(ay*core::RECIPROCAL_PI64));
			++i;
			axz += AngleX;
		}
		// This is the doubled vertex on the initial position
		buffer->Vertices[i] = video::S3DVertex(buffer->Vertices[i-polyCountX]);
		buffer->Vertices[i].TCoords.X=1.0f;
		++i;
	}

	// the vertex at the top of the sphere
	buffer->Vertices[i] = video::S3DVertex(0.0f,radius,0.0f, 0.0f,1.0f,0.0f, clr, 0.5f, 0.0f);

	// the vertex at the bottom of the sphere
	++i;
	buffer->Vertices[i] = video::S3DVertex(0.0f,-radius,0.0f, 0.0f,-1.0f,0.0f, clr, 0.5f, 1.0f);

	// recalculate bounding box

	buffer->BoundingBox.reset(buffer->Vertices[i].Pos);
	buffer->BoundingBox.addInternalPoint(buffer->Vertices[i-1].Pos);
	buffer->BoundingBox.addInternalPoint(radius,0.0f,0.0f);
	buffer->BoundingBox.addInternalPoint(-radius,0.0f,0.0f);
	buffer->BoundingBox.addInternalPoint(0.0f,0.0f,radius);
	buffer->BoundingBox.addInternalPoint(0.0f,0.0f,-radius);

	SMesh* mesh = new SMesh();
	mesh->addMeshBuffer(buffer);
	buffer->drop();

	mesh->setHardwareMappingHint(EHM_STATIC);
	mesh->recalculateBoundingBox();

	heightMap->drop();

	return mesh;
}

} // end namespace scene
} // end namespace irr

