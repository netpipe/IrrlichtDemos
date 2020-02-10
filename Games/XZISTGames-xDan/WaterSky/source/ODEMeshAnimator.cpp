
#include "ODEMeshAnimator.h"

ODEMeshAnimator::ODEMeshAnimator(scene::ISceneNode *node, scene::IAnimatedMesh *animatedMesh,
ODEPhysics *dPhysics, u32 creationFlags)
    : ODEAnimator(node, dPhysics, creationFlags)
{
    makeFromMesh( animatedMesh->getMesh(0), core::vector3df(1,1,1));
}

void ODEMeshAnimator::clearAll()
{
    for (u16 i = 0; i < dTriMeshData.size(); i ++)
    {
        dGeomTriMeshDataDestroy(dTriMeshData[i]);
    }
    
    ODEAnimator::clearAll();
    
    for (u16 i = 0; i < dVertices.size(); i ++)
    {
        delete [] dVertices[i];
        delete [] dIndices[i];
    }
}

void ODEMeshAnimator::makeFromMesh(scene::IMesh *mesh, core::vector3df scale)
{
    if (true) // combine meshes
    {
        // count total vertex and index counts from all mesh buffers
        u32 vertexCountTotal = 0;
        u32 indexCountTotal = 0;
        for (u32 i = 0; i < mesh->getMeshBufferCount(); i ++)
        {
            scene::IMeshBuffer *mb = mesh->getMeshBuffer(i);
            vertexCountTotal += mb->getVertexCount();
            indexCountTotal += mb->getIndexCount();
        }
        // create ODE copy. Should free these later - after destroying the ODE geom.
        dVector3 *dVertices = new dVector3[vertexCountTotal];
        int *dIndices = new int[indexCountTotal];
        // fill the copies...
        vertexCountTotal = 0;
        indexCountTotal = 0;
        for (u32 i = 0; i < mesh->getMeshBufferCount(); i ++)
        {
            scene::IMeshBuffer *mb = mesh->getMeshBuffer(i);
        
            u32 vertexCount = mb->getVertexCount();
            u32 indexCount = mb->getIndexCount();
        
            video::S3DVertex *iVertices = (video::S3DVertex *)mb->getVertices();
            u16 *iIndices = mb->getIndices();
        
            for (u32 j = 0; j < vertexCount; j ++)
            {
                dVertices[vertexCountTotal+j][0] = iVertices[j].Pos.X*scale.X;
                dVertices[vertexCountTotal+j][1] = iVertices[j].Pos.Y*scale.Y;
                dVertices[vertexCountTotal+j][2] = iVertices[j].Pos.Z*scale.Z;
                dVertices[vertexCountTotal+j][3] = 0;            
            }
        
            for (u32 j = 0; j < indexCount; j ++)
            {
                dIndices[indexCountTotal+j] = vertexCountTotal+iIndices[j];
            }
            
            vertexCountTotal += vertexCount;
            indexCountTotal += indexCount;
        }
        
        // make ode stuff
        dTriMeshDataID dTriMeshData = dGeomTriMeshDataCreate();
        dGeomTriMeshDataBuildSingle(dTriMeshData,
        (const dReal *)dVertices, 4*sizeof(dReal), vertexCountTotal,
        dIndices, indexCountTotal, 3*sizeof(int));
        dGeomID dGeom = dCreateTriMesh(dPhysics->dMainSpace, dTriMeshData, 0, 0, 0);
        dGeomSetBody(dGeom, 0);
        
        if (creationFlags & ODEANIM_CREATE_BODIES)
        {
            dReal density = 0.01;
            dBodyID dBody = dBodyCreate(dPhysics->dWorld);
            dMass mass;
            dMassSetTrimesh(&mass, density, dGeom);
            dBodySetMass(dBody, &mass);
            dGeomSetBody(dGeom, dBody);
            this->dBodies.push_back(dBody);
        }
        
        // add to arrays so freed in deconstructor
        this->dTriMeshData.push_back(dTriMeshData);
        this->dGeoms.push_back(dGeom);
        this->dVertices.push_back(dVertices);
        this->dIndices.push_back(dIndices);
    }
    else // Don't combine meshes (Un-thought-out, should not be used yet...)
    {
    for (u32 i = 0; i < mesh->getMeshBufferCount(); i ++)
    {
        scene::IMeshBuffer *mb = mesh->getMeshBuffer(i);
        
        u32 vertexCount = mb->getVertexCount();
        u32 indexCount = mb->getIndexCount();
        
        // get pointers from irrlicht
        video::S3DVertex *iVertices = (video::S3DVertex *)mb->getVertices();
        u16 *iIndices = mb->getIndices();
        
        // create ODE data. must remember these pointers and free them later (in deconstructor)
        dVector3 *dVertices = new dVector3[vertexCount];
        int *dIndices = new int[indexCount];
        
        for (u32 j = 0; j < mb->getVertexCount(); j ++)
        {
            dVertices[j][0] = iVertices[j].Pos.X*scale.X;
            dVertices[j][1] = iVertices[j].Pos.Y*scale.Y;
            dVertices[j][2] = iVertices[j].Pos.Z*scale.Z;
            dVertices[j][3] = 0;            
        }
        
        for (u32 j = 0; j < mb->getIndexCount(); j ++)
        {
            dIndices[j] = iIndices[j];
        }
        
        dTriMeshDataID dTriMeshData = dGeomTriMeshDataCreate();
        dGeomTriMeshDataBuildSingle(dTriMeshData,
        (const dReal *)dVertices, 4*sizeof(dReal), vertexCount,
        dIndices, indexCount, 3*sizeof(int));
        dGeomID dGeom = dCreateTriMesh(dPhysics->dMainSpace, dTriMeshData, 0, 0, 0);
        dGeomSetBody(dGeom, 0);
        
        if (creationFlags & ODEANIM_CREATE_BODIES)
        {
            dReal density = 0.01;
            dBodyID dBody = dBodyCreate(dPhysics->dWorld);
            dMass mass;
            dMassSetTrimesh(&mass, density, dGeom);
            dBodySetMass(dBody, &mass);
            dGeomSetBody(dGeom, dBody);
            this->dBodies.push_back(dBody);
        }
        
        // add to arrays so freed in deconstructor
        this->dTriMeshData.push_back(dTriMeshData);
        this->dGeoms.push_back(dGeom);
        this->dVertices.push_back(dVertices);
        this->dIndices.push_back(dIndices);
    }
    }
}

