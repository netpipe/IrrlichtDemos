#include "gimpactmeshshape.h"

using namespace irr;
using namespace core;
using namespace scene;

IGImpactMeshShape::IGImpactMeshShape(ISceneNode *n, IMesh *collMesh, f64 m)
{
    node = n;
    mass = m;

    type = ECST_GIMPACT;

    createShape(collMesh);
}

void IGImpactMeshShape::createShape(IMesh *mesh)
{
    btVector3 vertices[3];
	u32 i, j, k;
	s32 index, numVertices;
	u16* mb_indices;

	btTriangleMesh *pTriMesh = new btTriangleMesh();

	for(i = 0; i < mesh->getMeshBufferCount(); i++)
	{
		irr::scene::IMeshBuffer* mb=mesh->getMeshBuffer(i);

        //////////////////////////////////////////////////////////////////////////
		// Extract vertex data                                                  //
		// Because the vertices are stored as structs with no common base class,//
		// We need to handle each type separately                               //
		//////////////////////////////////////////////////////////////////////////
		if(mb->getVertexType() == irr::video::EVT_STANDARD)
		{
			irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
			mb_indices = mb->getIndices();
			numVertices = mb->getVertexCount();
			for(j=0;j<mb->getIndexCount();j+=3)
			{ //get index into vertex list
				for (k=0;k<3;k++)
				{
				    //three verts per triangle
					index = mb_indices[j+k];
					if (index > numVertices) return;
					//convert to btVector3
					vertices[k] = btVector3(mb_vertices[index].Pos.X,mb_vertices[index].Pos.Y,mb_vertices[index].Pos.Z); // 1100
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}

		}
		else
		if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
		{
			// Same but for S3DVertex2TCoords data
			irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
			u16* mb_indices = mb->getIndices();
			s32 numVertices = mb->getVertexCount();
			for(j=0;j<mb->getIndexCount();j+=3)
			{ //index into irrlicht data
				for (k=0;k<3;k++)
				{
					s32 index = mb_indices[j+k];
					if (index > numVertices) return;
					vertices[k] = btVector3(mb_vertices[index].Pos.X,mb_vertices[index].Pos.Y,mb_vertices[index].Pos.Z);
				}
				pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
			}
		}

		// Does not handle the EVT_TANGENTS type
	}

	gimpactShape = new btGImpactMeshShape(pTriMesh);
    shape = gimpactShape;
    gimpactShape->updateBound();

    calculateLocalInertia(vector3df(0.0f,0.0f,0.0f));
}

void IGImpactMeshShape::updateBound()
{
    gimpactShape->updateBound();
}

IGImpactMeshShape::~IGImpactMeshShape()
{
}
