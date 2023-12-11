#ifndef PHYSICS_H
#define PHYSICS_H

#include <irrlicht.h>

#include <btBulletDynamicsCommon.h>

#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btMatrix3x3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btAlignedObjectArray.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"//picking

#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btUniformScalingShape.h"

#include <map>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Physics
{
	public:

		Physics();
		~Physics();

		void unregisterObject ( const unsigned long int &PhysicObjectID );
		void tick();
		void clearBodies ( void );

		void updatePhysics ( u32 deltaTime );
		void updateRender ( btRigidBody *body );

		void createBox ( const btVector3 &pos, const btVector3 &scale, btScalar mass );
		 btRigidBody *createSphere ( const btVector3 &pos, btScalar radius, btScalar mass );

		void quaternionToEuler ( const btQuaternion &quat, btVector3 &euler );

		void registerIrrDevice ( IrrlichtDevice *device );

    void convertIrrMeshBufferBtTriangleMesh(IMeshBuffer *meshBuffer, btTriangleMesh *collisionMesh, vector3df scale);

    void push_back(btRigidBody *body);

		static Physics* getInstance() { return instance; }
		static Physics* instance;

    btVector3 RaycastWorld(const btVector3 Start, btVector3 End, btVector3 &Normal);

    // Get functions.
    btDiscreteDynamicsWorld *getDynamicsWorld(void);
    btDefaultCollisionConfiguration *getCollisionConfiguration(void);
    btCollisionDispatcher *getCollisionDispatcher(void);
    btSequentialImpulseConstraintSolver *getConstraintSolver(void);

	//private:

		bool firstTick;

		unsigned long int refId;
		unsigned long int lasttick;

		f32 simulationSpeed;

		IrrlichtDevice *m_irrDevice;

		list<btRigidBody*> m_rigidBodies;

		btDiscreteDynamicsWorld *dynamicsWorld;
		btDefaultCollisionConfiguration *collisionConfiguration;
		btCollisionDispatcher *dispatcher;
		btBroadphaseInterface *broadphase;
		btSequentialImpulseConstraintSolver *solver;
};

#endif
