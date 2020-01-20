#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>
#include <iostream>
#include "motionstatemanager.h"
#include "physics.h"
#include "bullethelper.h"
using namespace irr;

Physics::Physics(const core::aabbox3df irrBox){

    this->collisionConfiguration = new btDefaultCollisionConfiguration();
	/*this->broadPhase = new btAxisSweep3(
        toBulletVector(core::vector3df(irrBox.MinEdge)), toBulletVector(core::vector3df(irrBox.MaxEdge))
    );*/
    this->broadPhase = new btAxisSweep3(btVector3(-100000, -100000, -100000), btVector3(100000, 100000, 100000));

	this->dispatcher = new btCollisionDispatcher(collisionConfiguration);
	this->solver = new btSequentialImpulseConstraintSolver();
	this->initWorld();
}
Physics::~Physics(){
    delete this->collisionConfiguration;
    delete this->broadPhase;
    delete this->dispatcher;
    delete this->solver;
}
void Physics::initWorld(){

    this->World = new btDiscreteDynamicsWorld(this->dispatcher, this->broadPhase, this->solver, this->collisionConfiguration);
    this->World->setGravity(btVector3(0,-10,0));

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0),-1);
    this->ground = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, 0));
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(ground);

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI = btRigidBody::btRigidBodyConstructionInfo(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    groundRigidBodyCI.m_restitution = 1.f;
    groundRigidBodyCI.m_friction = 1.5f;
    groundRigidBodyCI.m_rollingFriction = 1.5f;
    groundRigidBodyCI.m_spinningFriction = 1.5f;

    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    this->World->addRigidBody(groundRigidBody);
    groundRigidBody->setUserPointer((void *) 0);
    this->Objects.push_back(groundRigidBody);

}
btTransform Physics::getGround(){ return this->ground; }
void Physics::UpdatePhysics(u32 TDeltaTime, u32 fps) {

    this->World->stepSimulation(TDeltaTime);

	///*DEPRECATED, MotionStateManager will handle this now!*/

//	for(std::list<btRigidBody *>::iterator Iterator = Objects.begin(); Iterator != Objects.end(); ++Iterator) {
//        if((*Iterator)->getUserPointer() != 0)
//		this->UpdateRender(*Iterator);
//
//	}


}
/*I know...Code duplication, need a refactoring.... Maybe a superclass is necessary here---> but it is too late now---*/
btRigidBody* Physics::createCannonBall(scene::ISceneNode* node, core::vector3df position, f32 radius){
    btTransform transformer;
    transformer.setIdentity();
    transformer.setOrigin(toBulletVector(position));

//    btDefaultMotionState *motion = new btDefaultMotionState(transformer);
    MotionStateManager *motion = new MotionStateManager(transformer, node);
    btCollisionShape *sphere = new btSphereShape(radius);

    btVector3 localInertia;
    sphere->calculateLocalInertia(2600.f, localInertia);
    btRigidBody::btRigidBodyConstructionInfo ballRigidBodyCI(2600,motion,sphere,localInertia);
    ballRigidBodyCI.m_friction =2.5f;
    ballRigidBodyCI.m_restitution = 0.001f;
    ballRigidBodyCI.m_spinningFriction = 2.1f;
    ballRigidBodyCI.m_rollingFriction = 2.1f;
    ballRigidBodyCI.m_angularDamping = 0.65f;
    btRigidBody *rigidBody = new btRigidBody(ballRigidBodyCI);
    rigidBody->setUserPointer((void *)(node));

    this->Objects.push_back(rigidBody);

    this->World->addRigidBody(rigidBody);
    return rigidBody;
}
btRigidBody* Physics::createCastleBlock(scene::ISceneNode* node, core::vector3df rotation, core::vector3df scale, core::vector3df position){
    btTransform transformer;
    transformer.setIdentity();
//    quat.setEuler(irr::core::DEGTORAD*rotation.X,irr::core::DEGTORAD*rotation.Y,irr::core::DEGTORAD*rotation.Z);
    transformer.setRotation(EulerToQuaternion(rotation));
    transformer.setOrigin(toBulletVector(position));
    MotionStateManager* motion = new MotionStateManager(transformer,node);
    core::vector3df extents = core::vector3df(scale.X,scale.Y,scale.Z)*0.5f;
    btCollisionShape* box = new btBoxShape(toBulletVector(extents));
    btVector3 localInertia;
    btScalar mass = 1500.f;
        btVector3 minEdge, maxEdge;

    box->calculateLocalInertia(mass, localInertia);
    box->getAabb(transformer,minEdge,maxEdge);
//    std::cout<<"btMin "<<minEdge[1]<<" irrMin"<<node->getBoundingBox().MinEdge.Y<<" irrMax"<<node->getBoundingBox().MaxEdge.Y<<" "<<maxEdge[1]<<" "<<std::endl;

    btRigidBody::btRigidBodyConstructionInfo blockInfo = btRigidBody::btRigidBodyConstructionInfo(mass,motion,box,localInertia);
    blockInfo.m_friction = 10.f;
    blockInfo.m_restitution = 0.001f;
    blockInfo.m_spinningFriction = 100.f;
    blockInfo.m_linearDamping = 0.05f;
    blockInfo.m_rollingFriction = 100.f;
    blockInfo.m_angularDamping = 0.8f;
    btRigidBody* rigidBody = new btRigidBody(blockInfo);
    rigidBody->activate(true);
    rigidBody->setUserPointer((void *) node);
    this->Objects.push_back(rigidBody);
    this->World->addRigidBody(rigidBody);
    return rigidBody;

}
btRigidBody* Physics::createTreasure(scene::IMeshSceneNode* node, core::vector3df scale, core::vector3df position){
    btTransform transformer;
    transformer.setIdentity();
    transformer.setOrigin(toBulletVector(position));
    core::vector3df _extent = node->getBoundingBox().getExtent();
    btCollisionShape* box = new btBoxShape(toBulletVector(_extent*0.5f));
    btVector3 minEdge, maxEdge;
    btVector3 localInertia;
    btScalar mass = 2780.f;
    transformer.setOrigin(btVector3(toBulletVector(position)[0], 0, toBulletVector(position)[2]));
        MotionStateManager *motion = new MotionStateManager(transformer,node);

    box->calculateLocalInertia(mass, localInertia);
    btRigidBody::btRigidBodyConstructionInfo targetInfo = btRigidBody::btRigidBodyConstructionInfo(mass,motion,box,localInertia);
        targetInfo.m_friction = 10.f;
    targetInfo.m_restitution = 0.001f;
    targetInfo.m_spinningFriction = 100.f;
    targetInfo.m_linearDamping = 0.05f;
    targetInfo.m_rollingFriction = 100.f;
    targetInfo.m_angularDamping = 0.8f;
    btRigidBody* rigidBody = new btRigidBody(targetInfo);
    rigidBody->activate(true);
    rigidBody->setUserPointer((void *) node);
    this->Objects.push_back(rigidBody);
    this->World->addRigidBody(rigidBody);
    return rigidBody;
}
///*DEPRECATED, MotionStateManager will handle this now!*/
void Physics::UpdateRender(btRigidBody *TObject) {
    return;
	scene::ISceneNode *Node = static_cast<scene::ISceneNode *>(TObject->getUserPointer());

	// Set position
	btVector3 Point = TObject->getCenterOfMassPosition();
	std::cout<<Point.getX()<<" "<<Point.getY()<<" "<<Point.getZ()<< "BULLET"<<std::endl;
	Node->setPosition(core::vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));
    std::cout<<Node->getAbsolutePosition().X<<" "<<Node->getAbsolutePosition().Y<<" "<<Node->getAbsolutePosition().Z<<"irrlicht"<<std::endl;

//	// Set rotation ---testing
	core::vector3df Euler;
	const btQuaternion& TQuat = TObject->getOrientation();
	core::quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
	q.toEuler(Euler);
	Euler *= core::RADTODEG;
	Node->setRotation(Euler);
}
/*@TODO make a class that handles this!!!*/
/*
btTriangleMesh* triangle = new btTriangleMesh(true,false);
    u16* indices;
    s32 num_vertex;
    btVector3 points[3];
    for(u32 i = 0; i < node->getMesh()->getMeshBufferCount(); i++ ){
        scene::IMeshBuffer* mesh_buffer = node->getMesh()->getMeshBuffer(i);
        if(mesh_buffer->getVertexType() == video::EVT_STANDARD){
            video::S3DVertex* vertices = (video::S3DVertex*) mesh_buffer->getVertices();
            indices = mesh_buffer->getIndices();
            num_vertex = mesh_buffer->getVertexCount();
            for(u32 j = 0; j < mesh_buffer->getIndexCount(); j+=3){
                for(s32 k = 0; k < 3; k++){
                    s32 index = indices[j+k];
                    if(index > num_vertex) { std::cout<<"error"<<std::endl; return 0; }
                    points[k] = btVector3(
                    btScalar(vertices[index].Pos.X),
                    btScalar(vertices[index].Pos.Y),
                    btScalar(vertices[index].Pos.Z)
                    );
                }

                triangle->addTriangle(points[0], points[1], points[2]);
            }
        }
        else if(mesh_buffer->getVertexType() == video::EVT_2TCOORDS){
            video::S3DVertex2TCoords* vertices = (video::S3DVertex2TCoords*) mesh_buffer->getVertices();
            indices = mesh_buffer->getIndices();
            num_vertex = mesh_buffer->getVertexCount();
            for(u32 j = 0; j < mesh_buffer->getIndexCount(); j+=3){
                for(s32 k = 0; k < 3; k++){
                    s32 index = indices[j+k];
                    if(index > num_vertex) { std::cout<<"error"<<std::endl; return 0; }
                    points[k] = btVector3(vertices[index].Pos.X,vertices[index].Pos.Y,vertices[index].Pos.Z);
                }

                triangle->addTriangle(points[0], points[1], points[2]);
            }
        }
    }*/
