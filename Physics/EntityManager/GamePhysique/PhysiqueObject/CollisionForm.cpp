#include "CollisionForm.h"
//! the licence is on EntityManager.h

namespace GamePhysique
{
    CollisionForm::CollisionForm(irr::IrrlichtDevice*dev) : device(dev)
    {
    }
    CollisionForm::~CollisionForm()
    {
        delete Shape;
    }
    void CollisionForm::makeRigid(irr::scene::IMeshSceneNode *Node,float mass,GamePhysique::OBJECT_FORME fm)
    {
        LocalInertia = btVector3(0,0,0);
        irr::core::vector3df size = Node->getScale();
        switch(fm)
        {
            case GamePhysique::OFM_CUBE :
                Shape = new btBoxShape(btVector3(size.X,size.Y,size.Z)*5);
            break;
            case GamePhysique::OFM_SPHERE :
                Shape = new btSphereShape(btScalar((size.X+size.Y+size.Z)/3)*5);
            break;
            default :
                btTriangleMesh *CollisionMesh = new btTriangleMesh();
                meshToPhysic(Node->getMesh(),CollisionMesh,size);
                Shape = new btBvhTriangleMeshShape(CollisionMesh, true);
            break;
        }
        Shape->calculateLocalInertia(mass,LocalInertia);
    }
    void CollisionForm::makeTerrain(int height, int witdh, void*data)
    {
        Shape = new btHeightfieldTerrainShape(height,witdh,data,10.f,1,true,true);
        //Shape->calculateLocalInertia(mass,LocalInertia);
    }

    void CollisionForm::setOrigine(float mass,irr::core::vector3df org)
    {
        btScalar ms = mass;
        btVector3 origine = Outils::irrToPhy(org);
        Shape->calculateLocalInertia((btScalar)ms,origine);
    }

    void CollisionForm::meshToPhysic(irr::scene::IMesh *TMesh, btTriangleMesh *TCollisionMesh, const irr::core::vector3df &TScale)
    {
        for(irr::u32 u = 0; u < TMesh->getMeshBufferCount(); u++)
            meshBufferToPhysic(TMesh->getMeshBuffer(u),TCollisionMesh,TScale);
    }
    void CollisionForm::meshBufferToPhysic(irr::scene::IMeshBuffer *TMeshBuffer, btTriangleMesh *TCollisionMesh, const irr::core::vector3df &TScale)
    {
        btVector3 TriangleVertices[3];
        irr::u16 *Indices = TMeshBuffer->getIndices();
        if(TMeshBuffer->getVertexType() == irr::video::EVT_STANDARD)
        {
            irr::video::S3DVertex *Vertices = (irr::video::S3DVertex*)TMeshBuffer->getVertices();
            for(irr::u32 i = 0; i < TMeshBuffer->getIndexCount(); i += 3)
            {
                TriangleVertices[0] = btVector3(Vertices[Indices[i]].Pos.X * TScale.X,Vertices[Indices[i]].Pos.Y * TScale.Y,Vertices[Indices[i]].Pos.Z * TScale.Z);
                TriangleVertices[1] = btVector3(Vertices[Indices[i+1]].Pos.X * TScale.X,Vertices[Indices[i+1]].Pos.Y * TScale.Y,Vertices[Indices[i+1]].Pos.Z * TScale.Z);
                TriangleVertices[2] = btVector3(Vertices[Indices[i+2]].Pos.X * TScale.X,Vertices[Indices[i+2]].Pos.Y * TScale.Y,Vertices[Indices[i+2]].Pos.Z * TScale.Z);
                TCollisionMesh->addTriangle(TriangleVertices[0], TriangleVertices[1], TriangleVertices[2],true);
            }
        }
        else
        {
            irr::video::S3DVertex2TCoords *Vertices = (irr::video::S3DVertex2TCoords*)TMeshBuffer->getVertices();
            for(irr::u32 i = 0; i < TMeshBuffer->getIndexCount(); i += 3)
            {
                TriangleVertices[0] = btVector3(Vertices[Indices[i]].Pos.X * TScale.X,Vertices[Indices[i]].Pos.Y * TScale.Y,Vertices[Indices[i]].Pos.Z * TScale.Z);
                TriangleVertices[1] = btVector3(Vertices[Indices[i+1]].Pos.X * TScale.X,Vertices[Indices[i+1]].Pos.Y * TScale.Y,Vertices[Indices[i+1]].Pos.Z * TScale.Z);
                TriangleVertices[2] = btVector3(Vertices[Indices[i+2]].Pos.X * TScale.X,Vertices[Indices[i+2]].Pos.Y * TScale.Y,Vertices[Indices[i+2]].Pos.Z * TScale.Z);
                TCollisionMesh->addTriangle(TriangleVertices[0], TriangleVertices[1], TriangleVertices[2],true);
            }
        }
    }
    btSoftBody*CollisionForm::meshToSoftPhysic(irr::scene::IMesh*TMesh,Outils::MeshData*data, const irr::core::vector3df &TScale)
    {
        return NULL;
    }
    void CollisionForm::meshBufferToSoftPhysic(irr::scene::IMeshBuffer *TMesh, btTriangleMesh *TCollisionMesh, const irr::core::vector3df &TScale)
    {
    }
}
