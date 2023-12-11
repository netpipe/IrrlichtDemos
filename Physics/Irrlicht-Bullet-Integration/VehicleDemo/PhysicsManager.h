#pragma once

/****************************************
Copyright 2018  Mahmoud Galal
****************************************/



#include "../IrrlichtBulletIntegration/baseheader.h"
#include "../IrrlichtBulletIntegration/InputEventHandler.h"

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"


//BulletDynamics.lib BulletCollision.lib LinearMath.lib BulletSoftBody.lib
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletSoftBody.lib")
#pragma comment(lib, "LinearMath.lib")

class PhysicsManager
{
public:
	PhysicsManager(void);
	~PhysicsManager(void);


	void initSoftPhysics();
	void initPhysics();
	void stepPhysicsWorld(float timestep);
	void createPhysicsGroundObject(float xRes,float zRes,btVector3 pos,void* attachedNode);
	void createVehicle();
	void destroyPhysics();
	void updateVehicle();
	void updatePhysics();
	void moveForward();
	void moveBackward();
	void turnLift();
	void turnRight();
	void setWheelRadius(float radius);
	void setWheelWidth(float width);
	void setChassisSize(btVector3& size);
	void setAttachedChassisNode(void* chassisAttachedNode){
		this->chassisAttachedNode =  chassisAttachedNode;
	}
	void setMaxEnginePower(float power){
		maxEngineForce = power;
	}
	void setAttachedWheelNodes(void* wheel_1_AttachedNode,void* wheel_2_AttachedNode,
		void* wheel_3_AttachedNode,void* wheel_4_AttachedNode){

			this->wheel_1_AttachedNode= wheel_1_AttachedNode;
			this->wheel_2_AttachedNode= wheel_2_AttachedNode;
			this->wheel_3_AttachedNode= wheel_3_AttachedNode;
			this->wheel_4_AttachedNode = wheel_4_AttachedNode;
	}
	void createPhysicsBoxFromEye(vector3df& position,vector3df& lookat,void* attachedNode);

private:
	btRigidBody* localCreateRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape);


	void* chassisAttachedNode;

	void* wheel_1_AttachedNode;
	void* wheel_2_AttachedNode;
	void* wheel_3_AttachedNode;
	void* wheel_4_AttachedNode;

	//Physics//////////////////////
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	btSequentialImpulseConstraintSolver* m_solver;
	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btSoftBodyWorldInfo softBodyWorldInfo;
	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

	btRigidBody* m_carChassis;
	btRigidBody* groundBody;
	int m_wheelInstances[4];
	btRaycastVehicle::btVehicleTuning	m_tuning;
	btVehicleRaycaster*	m_vehicleRayCaster;
	btRaycastVehicle*	m_vehicle ;
	btCollisionShape*	m_wheelShape;

	int CUBE_HALF_EXTENTS ;
	int rightIndex ;
	int upIndex;
	int forwardIndex;
	btVector3 wheelDirectionCS0;
	btVector3 wheelAxleCS;
	btVector3 chassisSize;

	bool moveForwardRequested,movebackwardRequested;


	///btRaycastVehicle is the interface for the constraint that implements the raycast vehicle
	///notice that for higher-quality slow-moving vehicles, another approach might be better
	///implementing explicit hinged-wheel constraints with cylinder collision, rather then raycasts
	float	gEngineForce;

	float	defaultBreakingForce;
	float	gBreakingForce;

	float	maxEngineForce;//this should be engine/velocity dependent
	float	maxBreakingForce;

	float	gVehicleSteering;
	float	steeringIncrement;
	float	steeringClamp ;
	float	wheelRadius ;
	float	wheelWidth;
	float	wheelFriction;//BT_LARGE_FLOAT;
	float	suspensionStiffness;
	float	suspensionDamping ;
	float	suspensionCompression ;
	float	rollInfluence;//1.0f;


	btScalar suspensionRestLength;


	//////////////////////////////////////////////
};

