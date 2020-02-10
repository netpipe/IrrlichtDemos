
#include "odeMesh.h"

odeMesh::odeMesh(dSpaceID dSpace, IAnimatedMesh *amesh, vector3df scale, vector3df pos)
{
    IMesh *mesh = amesh->getMesh(0, 255, -1, -1);
    
    for (i = 0; i < mesh->getMeshBufferCount(); i ++)
    {
        IMeshBuffer *mb = mesh->getMeshBuffer(i);
        
        u32 vertexCount = mb->getVertexCount();
        u32 indexCount = mb->getIndexCount();
        
        // get pointers from irrlicht
        S3DVertex *iVertices = (S3DVertex *)mb->getVertices();
        u16 *iIndices = mb->getIndices();
        
        // create ODE data. must remember these pointers and free them later (in deconstructor)
        dVector3 *dVertices = new dVector3[vertexCount];
        int *dIndices = new int[indexCount];
        
        for (j = 0; j < mb->getVertexCount(); j ++)
        {
            dVertices[j][0] = iVertices[j].Pos.X*scale.X;
            dVertices[j][1] = iVertices[j].Pos.Y*scale.Y;
            dVertices[j][2] = iVertices[j].Pos.Z*scale.Z;
            dVertices[j][3] = 0;            
        }
        
        for (j = 0; j < mb->getIndexCount(); j ++)
        {
            dIndices[j] = iIndices[j];
        }
        
        dTriMeshDataID dTriMeshData = dGeomTriMeshDataCreate();
        dGeomTriMeshDataBuildSingle(dTriMeshData,
        (const dReal *)dVertices, 4*sizeof(dReal), vertexCount,
        dIndices, indexCount, 3*sizeof(int));
        dGeomID dGeom = dCreateTriMesh(dSpace, dTriMeshData, 0, 0, 0);
        dGeomSetBody(dGeom, 0);
        
        // add to arrays so freed in deconstructor
        this->dTriMeshData.push_back(dTriMeshData);
        this->dGeom.push_back(dGeom);
        this->dVertices.push_back(dVertices);
        this->dIndices.push_back(dIndices);
    }
    
    setPosition(pos);
}

odeMesh::~odeMesh()
{
    for (i = 0; i < dTriMeshData.size(); i ++)
    {
         dGeomTriMeshDataDestroy(dTriMeshData[i]);
         dGeomDestroy(dGeom[i]);
         delete [] dVertices[i];
         delete [] dIndices[i];
    }
}

void odeMesh::setPosition(vector3df pos)
{
    for (i = 0; i < dGeom.size(); i ++)
    {
        dGeomSetPosition(dGeom[i], pos.X, pos.Y, pos.Z);
    }
}

void odeMesh::setRotation(vector3df rot)
{
    // could use same rotation code as in block.cpp (maybe)
    // BUT that only works for single geoms.
    // with an odeMesh we have several geoms...
    // If these share the same centre it *should* work.
    // FOLLOWING CODE IS UNTESTED!!
    dMatrix3 rota;
    dRFromEulerAngles(rota, rot.X*0.017453, rot.Y*0.017453, rot.Y*0.017453);
    for (i = 0; i < dGeom.size(); i ++)
    {
        dGeomSetRotation(dGeom[i], rota);
    }
}
