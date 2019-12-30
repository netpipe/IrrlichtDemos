// kudo's to go for this code http://sssiii.seesaa.net/article/109665032.html

#include <iostream>
#include <map>
#include <vector>
#include <list>

#include <btBulletDynamicsCommon.h>
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBody.h"

#include "BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpa2.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btIDebugDraw.h"
#include "LinearMath/btConvexHull.h"

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

std::map<int, btSoftBody::Node*> vertices;
std::vector<int> indices;
// Temporary placeholder for nodes
std::map<btSoftBody::Node*, int> node_map;

#define CUBE_HALF_EXTENTS 15

struct MeshData
{
    btScalar *mqo_vertices;
    int *mqo_indices;
    int indexCount, vertexCount;
};

MeshData testMesh;


/*

    delete [] testMesh.mqo_indices;
    delete [] testMesh.mqo_vertices;

 //   delete dynamicsWorld;
 //   delete solver;
 //   delete collisionConfiguration;
  //  delete dispatcher;
 //   delete broadphase;

*/

/*
int init {

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager *smgr = device->getSceneManager();

    ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 100, 0.5, -1, 0, 0, false);
    camera->setPosition(core::vector3df(0,400,-300));
    camera->setFarValue(10000);
    camera->setTarget(core::vector3df(0, 300, 0));

    IAnimatedMesh *cubeMesh = smgr->getMesh("../../media/sydney.md2");
    IAnimatedMeshSceneNode *cubeNode = smgr->addAnimatedMeshSceneNode(cubeMesh, 0, -1, vector3df(0, 500, 50), vector3df(0,110,0), vector3df(1,1,1), false);
    cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);
    cubeNode->setMaterialTexture(0, driver->getTexture("../../media/sydney.bmp"));
    cubeNode->setLoopMode(false);

    IAnimatedMesh *planemesh = smgr->addHillPlaneMesh("myHill", dimension2d<f32>(24, 24), dimension2d<u32>(100, 100));
    ISceneNode *q3sn = smgr->addOctTreeSceneNode(planemesh);
    q3sn->setMaterialFlag(video::EMF_LIGHTING, false);

    q3sn->setMaterialTexture(0, driver->getTexture("../../media/wall.jpg"));


    btSoftBodyWorldInfo   m_softBodyWorldInfo;
    btVector3 worldAabbMin(-10000,-10000,-10000);
    btVector3 worldAabbMax(10000,10000,10000);
    int maxProxies = 1024;
    btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);
    m_softBodyWorldInfo.m_broadphase = broadphase;

    //    const btDefaultCollisionConfigurationInfo collisionConfigInfo;
    btDefaultCollisionConfiguration* collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    m_softBodyWorldInfo.m_dispatcher = dispatcher;

    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    btSoftRigidDynamicsWorld* dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    btCollisionShape* groundShape = new btBoxShape (btVector3(2000,CUBE_HALF_EXTENTS,2000));

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-CUBE_HALF_EXTENTS/2.0,0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));

    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    groundRigidBody->setCollisionFlags( groundRigidBody->getCollisionFlags() |  btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );

    dynamicsWorld->addRigidBody(groundRigidBody);
    dynamicsWorld->setGravity(btVector3(0,-60.0,0));

    m_softBodyWorldInfo.m_sparsesdf.Initialize();
    m_softBodyWorldInfo.m_gravity.setValue(0,-10.0,0);
    m_softBodyWorldInfo.air_density      =   (btScalar)1.2;
    m_softBodyWorldInfo.water_density   =   0;
    m_softBodyWorldInfo.water_offset   =   0;
    m_softBodyWorldInfo.water_normal   =   btVector3(0,0,0);

    int cMeshBuffer, j;
    IMeshBuffer *mb;
    video::S3DVertex* mb_vertices;
    u16* mb_indices;


    for (cMeshBuffer=0; cMeshBuffer<cubeMesh->getMesh(0)->getMeshBufferCount(); cMeshBuffer++)
    {
        mb = cubeMesh->getMesh(0)->getMeshBuffer(cMeshBuffer);
        mb_vertices = (irr::video::S3DVertex*)mb->getVertices();
        mb_indices  = mb->getIndices();
    }

    std::map<int, int> index_map;
    std::map<int, int> bullet_map;
    std::map<int, S3DVertex> vertex_map;
    int count = 0;
    for (int i=0; i<mb->getIndexCount(); i++)
    {
        int iIndex = mb_indices[i];
        vector3df iVertex = mb_vertices[iIndex].Pos;
        bool isFirst = true;
        for (int j=0; j<i; j++)
        {
            int jIndex = mb_indices[j];
            vector3df jVertex = mb_vertices[jIndex].Pos;
            if (iVertex == jVertex)
            {
                index_map.insert(std::make_pair(i, j));
                isFirst = false;
                break;
            }
        }

        if (isFirst)
        {
            index_map.insert(std::make_pair(i, i));
            bullet_map.insert(std::make_pair(i, count));
            vertex_map.insert(std::make_pair(count, mb_vertices[iIndex]));
            count++;
        }
    }

    testMesh.indexCount = mb->getIndexCount();
    testMesh.mqo_indices = new int[testMesh.indexCount];
    testMesh.vertexCount = vertex_map.size();
    testMesh.mqo_vertices = new btScalar[testMesh.vertexCount*3];

    std::cout << "IndexCount=" << testMesh.indexCount << ", VertexCount=" << testMesh.vertexCount << std::endl;

    for (j=0; j<mb->getIndexCount(); j++)
    {
        int index1 = index_map.find(j)->second;
        int index2 = bullet_map.find(index1)->second;
        testMesh.mqo_indices[j]   = index2;
    }

    for (j=0; j<testMesh.vertexCount; j++)
    {
        testMesh.mqo_vertices[3*j] =   vertex_map[j].Pos.X;
        testMesh.mqo_vertices[3*j+1] = vertex_map[j].Pos.Y;
        testMesh.mqo_vertices[3*j+2] = -vertex_map[j].Pos.Z;
    }

    std::cout << "create softbody" << std::endl;
    btSoftBody* cubeSoftBody = btSoftBodyHelpers::CreateFromTriMesh(
        m_softBodyWorldInfo, testMesh.mqo_vertices, testMesh.mqo_indices, testMesh.indexCount/3);

    std::cout << "create map" << std::endl;
    for (int i=0; i<cubeSoftBody->m_faces.size(); i++)
    {
        btSoftBody::Face face = cubeSoftBody->m_faces[i];

        for (int j=0; j<3; j++)
        {
            if (node_map.find(face.m_n[j]) == node_map.end())
            {
                node_map.insert(std::make_pair(face.m_n[j], node_map.size()));
            }
        }

        for (int j=0; j<3; j++)
        {
            indices.push_back(node_map.find(face.m_n[j])->second);
        }
    }
    // Reverse node->index to index->node (should be unique on both sides)
    std::map<btSoftBody::Node*, int>::const_iterator node_iter;
    for (node_iter = node_map.begin(); node_iter != node_map.end(); ++node_iter)
    {

        vertices.insert(std::make_pair(node_iter->second, node_iter->first));
    }


    std::cout << "update Irrlicht vertices" << std::endl;

    std::map<int, int> testMesh_map;

    std::map<int, btSoftBody::Node*>::const_iterator it;


    for (int i=0; i<mb->getVertexCount(); i++)
    {
        for (it=vertices.begin(); it != vertices.end(); ++it)
        {
            int v_index = it->first;
            btSoftBody::Node* node = it->second;
            if (node->m_x.x() ==  mb_vertices[i].Pos.X &&
                node->m_x.y() ==  mb_vertices[i].Pos.Y &&
                node->m_x.z() == -mb_vertices[i].Pos.Z)
            {
                testMesh_map.insert(std::make_pair(i, v_index));
                break;
            }
        }
    }

    // SoftBodyのパラメータ設定
    std::cout << "addSoftBody" << std::endl;
    cubeSoftBody->m_cfg.kDP = 0.0;// Damping coefficient [0,1]
    cubeSoftBody->m_cfg.kDF = 0.2;// Dynamic friction coefficient [0,1]
    cubeSoftBody->m_cfg.kMT = 0.02;// Pose matching coefficient [0,1]
    cubeSoftBody->m_cfg.kCHR = 1.0;// Rigid contacts hardness [0,1]
    cubeSoftBody->m_cfg.kKHR = 0.8;// Kinetic contacts hardness [0,1]
    cubeSoftBody->m_cfg.kSHR = 1.0;// Soft contacts hardness [0,1]
    cubeSoftBody->m_cfg.piterations=2;
    cubeSoftBody->m_materials[0]->m_kLST = 0.8;
    cubeSoftBody->m_materials[0]->m_kAST = 0.8;
    cubeSoftBody->m_materials[0]->m_kVST = 0.8;
    cubeSoftBody->scale(btVector3(10,10,10));
    cubeSoftBody->setPose(true, true);
    cubeSoftBody->generateBendingConstraints(2);
    cubeSoftBody->randomizeConstraints();

    btMatrix3x3 m;
    m.setIdentity();
    cubeSoftBody->transform(btTransform(m,btVector3(0, 400, 0)));
    dynamicsWorld->addSoftBody(cubeSoftBody);

    std::cout << "start simulation" << std::endl;

}

int tick (void)
{


        // 60Hz
        dynamicsWorld->stepSimulation(1/60.0f, 1);

        for (int i=0; i<mb->getVertexCount(); i++)
        {
            int index = testMesh_map.find(i)->second;
            btSoftBody::Node* node = vertices.find(index)->second;
            mb_vertices[i].Pos.X = node->m_x.x();
            mb_vertices[i].Pos.Y = node->m_x.y();
            mb_vertices[i].Pos.Z = -node->m_x.z();
        }

        if (KEY_SPACE)
        {
            cubeSoftBody->addForce(btVector3(0, 10, 0), 0);
        }
        else if (KEY_ESCAPE)
        {
            break;
        }

        driver->beginScene(true, true, SColor(0,200,200,200));
        smgr->drawAll();
        driver->endScene();
    }
    device->drop();

    // Clean up
    for(int i=dynamicsWorld->getNumCollisionObjects()-1;i>0;i--)
    {
        btCollisionObject*   obj=dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody*      body=btRigidBody::upcast(obj);
        if(body&&body->getMotionState())
        {
            delete body->getMotionState();
        }
        while(dynamicsWorld->getNumConstraints())
        {
            btTypedConstraint*   pc=dynamicsWorld->getConstraint(0);
            dynamicsWorld->removeConstraint(pc);
            delete pc;
        }
        btSoftBody* softBody = btSoftBody::upcast(obj);
        if (softBody)
        {
            dynamicsWorld->removeSoftBody(softBody);
        } else
        {
            dynamicsWorld->removeCollisionObject(obj);
        }
        delete obj;
    }

    return 0;
}
*/
