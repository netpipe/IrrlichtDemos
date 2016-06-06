#include "CBulletPhysicsUtils.h"

#include "btBulletDynamicsCommon.h"

//------------------------------------------------------------------------------
//! ConvertIrrMeshToBulletTriangleArray
//! get mesh data. Bullet can use the meshbuffer data directly in btTriangleIndexVertexArray,
//! but Irrlicht indices are 16 bits wide which is not compatible with Bullet :(
//! So we are just using a btTriangleMesh instead, although it would probably
//! be faster to store the indices in int arrays and use these in a btTriangleIndexVertexArray
btTriangleIndexVertexArray* ConvertIrrMeshToBulletTriangleArray(
  irr::scene::IMesh* pMesh, 
  const irr::core::vector3df& scaling)
{	
  btVector3 vertices[3];
  irr::u32 i, j, numVertices, numIndices;
  irr::u16* aIrrIndices;

  const int vertStride = sizeof(btVector3);
  const int indexStride = 3*sizeof(int);
  int totalTriangles;

  btTriangleIndexVertexArray* indexVertexArrays = new btTriangleIndexVertexArray();
  btIndexedMesh aIndexedMesh;

  for (i = 0; i < pMesh->getMeshBufferCount(); i++)
  {
    irr::scene::IMeshBuffer* mb = pMesh->getMeshBuffer(i);

    //extract vertex data
    //because the vertices are stored as structs with no common base class,
    // we need to handle each type separately
    if (mb->getVertexType()==irr::video::EVT_STANDARD)
    {
      irr::video::S3DVertex* aIrrVertices=(irr::video::S3DVertex*)mb->getVertices();
      aIrrIndices = mb->getIndices();
      numVertices = mb->getVertexCount();
      numIndices = mb->getIndexCount();
      totalTriangles = numIndices/3;


      // TODO: don't forget delete [] 
      int* aBtIndexBuffer = new int[numIndices];
      btVector3* aBtVertBuffer = new btVector3[numVertices];

      // fill ind
      for (j = 0; j < numIndices; j++)
      { 
        aBtIndexBuffer[j] = aIrrIndices[j];
      }
      // fill vert
      for (j = 0; j < numVertices; j++)
      { 
        aBtVertBuffer[j].setValue(
          aIrrVertices[j].Pos.X*scaling.X, 
          aIrrVertices[j].Pos.Y*scaling.Y,
          aIrrVertices[j].Pos.Z*scaling.Z);
      }

      aIndexedMesh.m_numTriangles = totalTriangles;
      aIndexedMesh.m_triangleIndexBase = (const unsigned char *)aBtIndexBuffer;
      aIndexedMesh.m_triangleIndexStride = indexStride;
      aIndexedMesh.m_numVertices = numVertices;
      aIndexedMesh.m_vertexBase = (const unsigned char *) &aBtVertBuffer[0].x();
      aIndexedMesh.m_vertexStride = vertStride;
      indexVertexArrays->addIndexedMesh(aIndexedMesh);
    }
    else if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
    {
      //same but for S3DVertex2TCoords data
      irr::video::S3DVertex2TCoords* aIrrVertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
      aIrrIndices = mb->getIndices();
      numVertices = mb->getVertexCount();
      numIndices = mb->getIndexCount();
      totalTriangles = numIndices/3;

      int* aBtIndexBuffer = new int[numIndices];
      btVector3* aBtVertBuffer = new btVector3[numVertices];

      // fill ind
      for (j = 0; j < numIndices; j++)
      { 
        aBtIndexBuffer[j] = aIrrIndices[j];
      }
      // fill vert
      for (j = 0; j < numVertices; j++)
      { 
        aBtVertBuffer[j].setValue(
          aIrrVertices[j].Pos.X*scaling.X, 
          aIrrVertices[j].Pos.Y*scaling.Y,
          aIrrVertices[j].Pos.Z*scaling.Z);
      }

      aIndexedMesh.m_numTriangles = totalTriangles;
      aIndexedMesh.m_triangleIndexBase = (const unsigned char *)aBtIndexBuffer;
      aIndexedMesh.m_triangleIndexStride = indexStride;
      aIndexedMesh.m_numVertices = numVertices;
      aIndexedMesh.m_vertexBase = (const unsigned char *)&aBtVertBuffer[0].x();
      aIndexedMesh.m_vertexStride = vertStride;
      indexVertexArrays->addIndexedMesh(aIndexedMesh);
    }	
    //not bothering with EVT_TANGENTS vertex type
  }

  return indexVertexArrays;
}

//------------------------------------------------------------------------------
//! ConvertIrrMeshToBulletTriangleMesh
//! get mesh data. Bullet can use the meshbuffer data directly in btTriangleIndexVertexArray,
//! but Irrlicht indices are 16 bits wide which is not compatible with Bullet :(
//! So we are just using a btTriangleMesh instead, although it would probably
//! be faster to store the indices in int arrays and use these in a btTriangleIndexVertexArray
btTriangleMesh* ConvertIrrMeshToBulletTriangleMesh(
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
/*
	scene::SMeshBufferLightMap smb;
	g_terrain->getMeshBufferForLOD(smb,0);
	*/

    //extract vertex data
    //because the vertices are stored as structs with no common base class,
    // we need to handle each type separately
    if(mb->getVertexType()==irr::video::EVT_STANDARD)
    {
      irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
      mb_indices = mb->getIndices();
      numVertices = mb->getVertexCount();
      numIndices = mb->getIndexCount();
      for(j=0;j<numIndices;j+=3)
      { //get index into vertex list
        for (k=0;k<3;k++) 
        { //three verts per triangle
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
    else if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
    {
      //same but for S3DVertex2TCoords data
      irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
      mb_indices = mb->getIndices();
      numVertices = mb->getVertexCount();
      numIndices = mb->getIndexCount();

	  /*irr::scene::IMesh *pMesh = pTerrain->getMesh();

			irr::scene::SMeshBufferLightMap smb;
			pTerrain->getMeshBufferForLOD(smb,0);

			int nv = smb.getVertexCount ();
			int ni = smb.getIndexCount 	();*/



      for(j=0;j<numIndices;j+=3)
      { //index into irrlicht data
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

//------------------------------------------------------------------------------
//! ConvertIrrMeshToBulletConvexHullShape
//! ConvexHullShape implements an implicit (getSupportingVertex) Convex Hull of a Point Cloud (vertices)
//! No connectivity is needed.
btConvexHullShape* ConvertIrrMeshToBulletConvexHullShape(
  irr::scene::IMesh* pMesh, 
  const irr::core::vector3df& scaling)
{
  btVector3 vertices[3];
  irr::u32 i,j,k,index,numVertices;
  irr::u16* mb_indices;

  irr::core::array<btPoint3> aBtVertBuffer(512);

  for (i=0; i<pMesh->getMeshBufferCount(); i++)
  {
    irr::scene::IMeshBuffer* mb=pMesh->getMeshBuffer(i);

    //extract vertex data
    //because the vertices are stored as structs with no common base class,
    // we need to handle each type separately
    if(mb->getVertexType()==irr::video::EVT_STANDARD)
    {
      irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
      mb_indices = mb->getIndices();
      numVertices = mb->getVertexCount();
      for(j=0;j<mb->getIndexCount();j+=3)
      { //get index into vertex list
        for (k=0;k<3;k++) 
        { //three verts per triangle
          index = mb_indices[j+k];
          //convert to btPoint3
          aBtVertBuffer.push_back(btPoint3(
            mb_vertices[index].Pos.X*scaling.X,
            mb_vertices[index].Pos.Y*scaling.Y,
            mb_vertices[index].Pos.Z*scaling.Z));
        }
      }
    }
    else if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
    {
      //same but for S3DVertex2TCoords data
      irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
      irr::u16* mb_indices = mb->getIndices();
      irr::s32 numVertices = mb->getVertexCount();
      for(j=0;j<mb->getIndexCount();j+=3)
      { //index into irrlicht data
        for (k=0;k<3;k++) 
        {
          irr::s32 index = mb_indices[j+k];
          //convert to btPoint3
          aBtVertBuffer.push_back(btPoint3(
            mb_vertices[index].Pos.X*scaling.X,
            mb_vertices[index].Pos.Y*scaling.Y,
            mb_vertices[index].Pos.Z*scaling.Z));
        }
      }
    }	
    //not bothering with EVT_TANGENTS vertex type
  }

  btScalar ratio = aBtVertBuffer[0].length();
  for (i = 0; i < aBtVertBuffer.size(); i++)
    aBtVertBuffer[i].normalize();

  btConvexHullShape* aConvexHullShape = new btConvexHullShape((btScalar*)&aBtVertBuffer[0], aBtVertBuffer.size());
  aConvexHullShape->setLocalScaling(btVector3(ratio, ratio, ratio));
  aConvexHullShape->recalcLocalAabb();
  return aConvexHullShape;
}
