#ifndef UTILS
#define UTILS

#include <irrlicht.h>
#include "btBulletDynamicsCommon.h"
using namespace irr;
SIMD_FORCE_INLINE btTriangleMesh* ConvertIrrMeshToBulletTriangleMesh(
    irr::scene::IMesh* pMesh,
    const irr::core::vector3df& scaling)
{
    btVector3 vertices[3];
    irr::u32 i,j,k,index,numVertices,numIndices;
    irr::u16* mb_indices;

    btTriangleMesh *pTriMesh = new btTriangleMesh();

    for (i=0; i<pMesh->getMeshBufferCount(); i++)
    {
        irr::scene::IMeshBuffer* mb=pMesh->getMeshBuffer(i);

        //extract vertex data
        //because the vertices are stored as structs with no common base class,
        // we need to handle each type separately
        if (mb->getVertexType()==irr::video::EVT_STANDARD)
        {
            irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
            mb_indices = mb->getIndices();
            numVertices = mb->getVertexCount();
            numIndices = mb->getIndexCount();
            for (j=0;j<numIndices;j+=3)
            {
                //get index into vertex list
                for (k=0;k<3;k++)
                {
                    //three verts per triangle
                    index = mb_indices[j+k];
                    //convert to btVector3
                    // we apply scaling factor directly to verticies
                    vertices[k] = btVector3(mb_vertices[index].Pos.X*scaling.X,
                                            mb_vertices[index].Pos.Y*scaling.Y,
                                            mb_vertices[index].Pos.Z*scaling.Z);
                }
                pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
            }


        }
        else if (mb->getVertexType()==irr::video::EVT_2TCOORDS)
        {
            //same but for S3DVertex2TCoords data
            irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
            mb_indices = mb->getIndices();
            numVertices = mb->getVertexCount();
            numIndices = mb->getIndexCount();
            for (j=0;j<numIndices;j+=3)
            {
                //index into irrlicht data
                for (k=0;k<3;k++)
                {
                    index = mb_indices[j+k];
                    // we apply scaling factor directly to verticies
                    vertices[k] = btVector3(mb_vertices[index].Pos.X*scaling.X,
                                            mb_vertices[index].Pos.Y*scaling.Y,
                                            mb_vertices[index].Pos.Z*scaling.Z);
                }
                pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
            }
        }
        //not bothering with EVT_TANGENTS vertex type
    }

    return pTriMesh;
}
#endif
