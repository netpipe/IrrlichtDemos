// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include <IMesh.h>
#include <IMeshBuffer.h>
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "irrBulletTriangleMeshShape.h"

using namespace irr;
using namespace core;
using namespace scene;

ITriangleMeshShape::ITriangleMeshShape() : CollisionMesh(0)
{
}

btTriangleMesh *ITriangleMeshShape::createTriangleMesh(IMesh* const mesh)
{
    btVector3 vertices[3];
	u32 i, j, k;
	s32 index, numVertices;
	u16* mb_indices;
	const vector3df &scale = node->getScale();

	auto pTriMesh = new btTriangleMesh();

	for(i = 0; i < mesh->getMeshBufferCount(); i++)
	{
		auto mb = mesh->getMeshBuffer(i);

        //////////////////////////////////////////////////////////////////////////
		// Extract vertex data                                                  //
		// Because the vertices are stored as structs with no common base class,//
		// We need to handle each type separately                               //
		//////////////////////////////////////////////////////////////////////////
		if(mb->getVertexType() == irr::video::EVT_STANDARD)
		{
			auto mb_vertices = (irr::video::S3DVertex*)mb->getVertices();
			mb_indices = mb->getIndices();
			numVertices = mb->getVertexCount();
			for(j = 0; j < mb->getIndexCount(); j+=3)
			{ //get index into vertex list
				for (k = 0; k < 3; k++)
				{
				    //three verts per triangle
					index = mb_indices[j+k];
					if (index > numVertices) continue;
					//convert to btVector3
					vertices[k] = irrlichtToBulletVector(mb_vertices[index].Pos * scale); // 1100
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}

		}
		else
		if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
		{
			// Same but for S3DVertex2TCoords data
			auto mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
			u16* mb_indices = mb->getIndices();
			s32 numVertices = mb->getVertexCount();
			for(j = 0; j < mb->getIndexCount(); j+=3)
			{   
				//index into irrlicht data
				for (k = 0; k < 3; k++)
				{
					s32 index = mb_indices[j+k];
					if (index > numVertices) continue;
					vertices[k] = irrlichtToBulletVector(mb_vertices[index].Pos * scale);
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}
		}

		// Does not handle the EVT_TANGENTS type
	}

	return pTriMesh;
}

ITriangleMeshShape::~ITriangleMeshShape()
{
    if(CollisionMesh != nullptr)
        delete CollisionMesh;
}
