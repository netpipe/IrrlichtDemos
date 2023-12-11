#include "Physics.h"
/*testing
#include "../FileSystem/BulletBlendReader.h"
//#include "btBulletDynamicsCommon.h"
#include "../FileSystem/readblend/readblend.h"
#include "../FileSystem/readblend/blendtype.h"
#include "../FileSystem/blenderUp.h"
*/
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

Physics* Physics::instance = NULL;

Physics::Physics() {
	m_irrDevice = NULL;

	instance = this;
	refId = 0;

	btVector3 worldAabbMin ( -2000,-2000,-2000 );// set the world min/max
	btVector3 worldAabbMax ( 2000,2000,2000 );
	int maxProxies = 32766;

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher ( collisionConfiguration );
	solver = new btSequentialImpulseConstraintSolver();
	broadphase = new btAxisSweep3 ( worldAabbMin, worldAabbMax, maxProxies );
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->getDispatchInfo().m_enableSPU = true;
//	dynamicsWorld->setGravity ( btVector3 ( 0, -3.14, -0 ) ); //LHS y up

	simulationSpeed = (15); //15
	firstTick = true ;
}

Physics::~Physics() {
	clearBodies();
	delete dynamicsWorld;
	delete broadphase;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
}

btDiscreteDynamicsWorld *Physics::getDynamicsWorld(void)
{
  return dynamicsWorld;
}

btDefaultCollisionConfiguration *Physics::getCollisionConfiguration(void)
{
  return collisionConfiguration;
}

btCollisionDispatcher *Physics::getCollisionDispatcher(void)
{
  return dispatcher;
}

btSequentialImpulseConstraintSolver *Physics::getConstraintSolver(void)
{
  return solver;
}

void Physics::push_back(btRigidBody *body)
{
  m_rigidBodies.push_back(body);
}

void Physics::unregisterObject ( const unsigned long int &objectID ) { }

void Physics::tick() { }

void Physics::clearBodies(void) {
   /* taken from bullet vehicle physics
    	int i;
    	for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--){
    		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
    		btRigidBody* body = btRigidBody::upcast(obj);
    		if (body && body->getMotionState())
    		{
    			delete body->getMotionState();
    		}
    		m_dynamicsWorld->removeCollisionObject( obj );
    		delete obj;
    	}
    */
	for(list<btRigidBody *>::Iterator iter = m_rigidBodies.begin(); iter != m_rigidBodies.end(); ++iter){

		btRigidBody *body = *iter;
		// Delete irrlicht node.
		ISceneNode *node = static_cast<ISceneNode *>(body->getUserPointer());
		node->remove();

		dynamicsWorld->removeRigidBody(body);

		delete body->getMotionState();
		delete body->getCollisionShape();
		delete body;
	}
	m_rigidBodies.clear();
}

void Physics::updatePhysics(u32 deltaTime) {

    float minFPS = 1000000.f/60.f;
	if (deltaTime * 0.001f > minFPS)
		deltaTime = minFPS;


	dynamicsWorld->stepSimulation(deltaTime* 0.001f * simulationSpeed );//deltaTime * 0.001f * simulationSpeed,2

	for(list<btRigidBody*>::Iterator iter = m_rigidBodies.begin(); iter !=  m_rigidBodies.end(); ++iter)
		updateRender(*iter);
}

void Physics::updateRender(btRigidBody *body) {
	ISceneNode *node = static_cast<ISceneNode *>(body->getUserPointer());

	// Set position.
	btVector3 point = body->getCenterOfMassPosition();
	node->setPosition(vector3df(
		(f32)point[0],
		(f32)point[1],
		(f32)point[2]));

	// Set rotation.
	btVector3 eulerRot;
	quaternionToEuler(body->getOrientation(), eulerRot);
	node->setRotation(vector3df(
		eulerRot[0],
		eulerRot[1],
		eulerRot[2]));
}


btVector3 Physics::RaycastWorld(const btVector3 Start, btVector3 End, btVector3 &Normal) {

                btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
//                RayCallback.m_collisionFilterMask = FILTER_CAMERA;

                // Perform raycast
                dynamicsWorld->rayTest(Start, End, RayCallback);
                if(RayCallback.hasHit()) {
                        End = RayCallback.m_hitPointWorld;
                        Normal = RayCallback.m_hitNormalWorld;
                        return RayCallback.m_hitPointWorld;
                }
    //    }

      //  return ;
}

void Physics::createBox(const btVector3 &pos, const btVector3 &scale, btScalar mass) {
	ISceneNode *node = m_irrDevice->getSceneManager()->addCubeSceneNode(1.0f);
	node->setScale(vector3df(
		(f32)scale[0],
		(f32)scale[1],
		(f32)scale[2]));
	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	node->setMaterialTexture(0, m_irrDevice->getVideoDriver()->getTexture("data/textures/material/wood1_512.png"));

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(pos);

	btDefaultMotionState *motionState = new btDefaultMotionState(transform);

	// Create the shape.
	btVector3 halfExtents(scale[0] * 0.5f, scale[1] * 0.5f, scale[2] * 0.5f);
	btCollisionShape *shape = new btBoxShape(halfExtents);

	// Add mass.
	btVector3 localInertia;
	shape->calculateLocalInertia(mass, localInertia);

	// Create the rigid body object.
	btRigidBody *body = new btRigidBody(mass, motionState, shape, localInertia);

	// Store pointer to object.
	body->setUserPointer((void *)(node));

	// Add the object to the world.
	dynamicsWorld->addRigidBody(body);
	//m_rigidBodies.push_back(body);
	push_back(body);
}

 btRigidBody *Physics::createSphere(const btVector3 &pos, btScalar radius, btScalar mass) {
	ISceneNode *node = m_irrDevice->getSceneManager()->addSphereSceneNode(radius, 32);

	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	node->setMaterialTexture(0, m_irrDevice->getVideoDriver()->getTexture("data/textures/material/marble1_256.png"));

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(pos);

	btDefaultMotionState *motionState = new btDefaultMotionState(transform);

	// Create the shape.
	btCollisionShape *shape = new btSphereShape(radius);

	// Add mass.
	btVector3 localInertia;
	shape->calculateLocalInertia(mass, localInertia);

	// Create the rigid body object.
	btRigidBody *body = new btRigidBody(mass, motionState, shape, localInertia);

	// Store pointer to object.
	body->setUserPointer((void *)(node));

	// Add the object to the world.
	dynamicsWorld->addRigidBody(body);
	//m_rigidBodies.push_back(body);
	push_back(body);

	return body;

}

void Physics::quaternionToEuler(const btQuaternion &quat, btVector3 &euler) {
	btScalar 	w = quat.getW(),
						x = quat.getX(),
						y = quat.getY(),
						z = quat.getZ();

	float wSquared = w * w,
				xSquared = x * x,
				ySquared = y * y,
				zSquared = z * z;

	euler.setX(atan2f(2.0f * (y * z + x * w), -xSquared - ySquared + zSquared + wSquared));
	euler.setY(asinf(-2.0f * (x * z - y * w)));
	euler.setZ(atan2f(2.0f * (x * y + z * w), xSquared - ySquared - zSquared + wSquared));

	euler *= RADTODEG;
}

void Physics::convertIrrMeshBufferBtTriangleMesh(IMeshBuffer *meshBuffer, btTriangleMesh *collisionMesh, vector3df scale)
{
  btVector3 triVerts[3];
  u16 *indices = meshBuffer->getIndices();

  if(meshBuffer->getVertexType() == EVT_STANDARD)
  {
    S3DVertex *verts = (S3DVertex *)meshBuffer->getVertices();
    for(u32 i = 0; i < meshBuffer->getIndexCount(); i+= 3)
    {
      triVerts[0] = btVector3(
        verts[indices[i]].Pos.X * scale.X,
        verts[indices[i]].Pos.Y * scale.Y,
        verts[indices[i]].Pos.Z * scale.Z);

      triVerts[1] = btVector3(
        verts[indices[i+1]].Pos.X * scale.X,
        verts[indices[i+1]].Pos.Y * scale.Y,
        verts[indices[i+1]].Pos.Z * scale.Z);

      triVerts[2] = btVector3(
        verts[indices[i+2]].Pos.X * scale.X,
        verts[indices[i+2]].Pos.Y * scale.Y,
        verts[indices[i+2]].Pos.Z * scale.Z);

      collisionMesh->addTriangle(triVerts[0], triVerts[1], triVerts[2]);
    }
  }
  else
  {
    S3DVertex2TCoords *verts = (S3DVertex2TCoords *)meshBuffer->getVertices();
    for(u32 i = 0; i < meshBuffer->getIndexCount(); i+= 3)
    {
      triVerts[0] = btVector3(
        verts[indices[i]].Pos.X * scale.X,
        verts[indices[i]].Pos.Y * scale.Y,
        verts[indices[i]].Pos.Z * scale.Z);

      triVerts[1] = btVector3(
        verts[indices[i+1]].Pos.X * scale.X,
        verts[indices[i+1]].Pos.Y * scale.Y,
        verts[indices[i+1]].Pos.Z * scale.Z);

      triVerts[2] = btVector3(
        verts[indices[i+2]].Pos.X * scale.X,
        verts[indices[i+2]].Pos.Y * scale.Y,
        verts[indices[i+2]].Pos.Z * scale.Z);

      collisionMesh->addTriangle(triVerts[0], triVerts[1], triVerts[2]);
    }
  }
  //printf("Converted Irrlicht mesh buffer to Bullet triangle mesh, numTriangles = %i\n", collisionMesh->getNumTriangles());
}

void Physics::registerIrrDevice(IrrlichtDevice *device) {
	m_irrDevice = device;

}
