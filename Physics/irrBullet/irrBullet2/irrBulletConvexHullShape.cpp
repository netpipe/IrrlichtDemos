// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include <IMesh.h>
#include <IMeshBuffer.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "irrBulletCollisionShape.h"
#include "irrBulletConvexHullShape.h"

using namespace irr;
using namespace core;
using namespace scene;

IConvexHullShape::IConvexHullShape(irr::scene::ISceneNode *n, irr::scene::IMesh *collMesh, f32 m)
{
    node = n;
    mass = m;

	type = ECollisionShapeType::ECST_CONVEXHULL;

    createShape(collMesh);
}

void IConvexHullShape::createShape(IMesh *collMesh)
{
    btConvexHullShape *hullShape = new btConvexHullShape();

    getConvexHull(collMesh, hullShape);

    shape = hullShape;
}


void IConvexHullShape::getConvexHull(IMesh *collMesh, btConvexHullShape *hullShape)
{
	u32 i, j;
	u32 numVertices;
	const vector3df &scale = node->getScale();

	for(i = 0; i < collMesh->getMeshBufferCount(); i++)
	{
		auto mb=collMesh->getMeshBuffer(i);

        //////////////////////////////////////////////////////////////////////////
		// Extract vertex data                                                  //
		// Because the vertices are stored as structs with no common base class,//
		// We need to handle each type separately                               //
		//////////////////////////////////////////////////////////////////////////
		if(mb->getVertexType() == irr::video::EVT_STANDARD)
		{
			auto mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
			numVertices = mb->getVertexCount();
			for(j=0;j<numVertices;j++)
			{
			    hullShape->addPoint(irrlichtToBulletVector(mb_vertices[j].Pos * scale));
			}

		}
		else
		if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
		{
			// Same but for S3DVertex2TCoords data
			auto mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
			u32 numVertices = mb->getVertexCount();
			for(j=0;j<numVertices;j++)
			{
			    hullShape->addPoint(irrlichtToBulletVector(mb_vertices[j].Pos * scale));
			}
		}

		// Does not handle the EVT_TANGENTS type
	}
}


IConvexHullShape::~IConvexHullShape()
{
}
