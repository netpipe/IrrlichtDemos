#include "PhysicsManager.h"
#include <vector>



PhysicsManager::PhysicsManager(void):m_vehicle(nullptr),m_wheelShape(nullptr),
	m_vehicleRayCaster(nullptr)
{
	rightIndex = 0;
	upIndex = 1;
	forwardIndex = 2;
	wheelDirectionCS0 = btVector3(0,-1,0);
	wheelAxleCS = btVector3(-1,0,0);

	gEngineForce = 0.f;

	defaultBreakingForce = 10.f;
	gBreakingForce = 100.f;

	maxEngineForce = 400.f;//this should be engine/velocity dependent
	maxBreakingForce = 33.f;

	gVehicleSteering = 0.f;
	steeringIncrement = 0.05f;
	steeringClamp = 0.4f;
	wheelRadius = 0.5f;
	wheelWidth = 0.4f;
	wheelFriction = 400;//BT_LARGE_FLOAT;
	suspensionStiffness = 20.f;
	suspensionDamping = 2.3f;
	suspensionCompression = 4.4f;
	rollInfluence = 0.1f;//1.0f;

	chassisSize = btVector3(1.f,0.5f,2.f);
	suspensionRestLength = btScalar(1.1);

	CUBE_HALF_EXTENTS =  1;

	this->chassisAttachedNode = nullptr;
	this->wheel_1_AttachedNode= nullptr;
	this->wheel_2_AttachedNode= nullptr;
	this->wheel_3_AttachedNode= nullptr;
	this->wheel_4_AttachedNode = nullptr;
}


PhysicsManager::~PhysicsManager(void)
{
}

void PhysicsManager::initPhysics(){

	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_broadphase = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	///-----initialization_end-----
}

void PhysicsManager::destroyPhysics(){
	if (m_dynamicsWorld)
	{

		int i;
		for (i = m_dynamicsWorld->getNumConstraints() - 1; i >= 0; i--)
		{
			m_dynamicsWorld->removeConstraint(m_dynamicsWorld->getConstraint(i));
		}
		for (i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			m_dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}
	}
	//delete collision shapes
	for (int j = 0; j<m_collisionShapes.size(); j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	delete m_dynamicsWorld;
	m_dynamicsWorld=0;

	delete m_vehicleRayCaster;
	m_vehicleRayCaster = 0;

	delete m_vehicle;
	m_vehicle=0;

	delete m_wheelShape;
	m_wheelShape=0;

	delete m_solver;
	m_solver=0;

	delete m_broadphase;
	m_broadphase=0;

	delete m_dispatcher;
	m_dispatcher=0;

	delete m_collisionConfiguration;
	m_collisionConfiguration=0;
}

void PhysicsManager::stepPhysicsWorld(float timestep){

	if(m_vehicle)
	{



		int wheelIndex = 2;
		m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
		m_vehicle->setBrake(gBreakingForce,wheelIndex);
		wheelIndex = 3;
		m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
		m_vehicle->setBrake(gBreakingForce,wheelIndex);


		wheelIndex = 0;
		m_vehicle->setSteeringValue(gVehicleSteering,wheelIndex);
		wheelIndex = 1;
		m_vehicle->setSteeringValue(gVehicleSteering,wheelIndex);

		if(moveForwardRequested ){
			gEngineForce -= 80*timestep;
			if(gEngineForce<0){
				gEngineForce = 0;
				gBreakingForce = maxBreakingForce;
			}
		}else{
			gEngineForce += 80*timestep;
			if(gEngineForce>0){
				gEngineForce = 0;
				gBreakingForce = maxBreakingForce;
			}
		}




	}


	m_dynamicsWorld->stepSimulation(timestep, 2);
}

void PhysicsManager::createPhysicsGroundObject(float xRes,float zRes,btVector3 pos,void* attachedNode){

	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(xRes), btScalar(0.5), btScalar(zRes)));

	m_collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(pos);

	btScalar mass(0.);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	///body->setFriction(1000);
	body->setUserPointer(attachedNode);
//	body->setUserIndex(5);
	body->setFriction(1);
	groundBody = body;

	//add the body to the dynamics world
	m_dynamicsWorld->addRigidBody(body);
}

void PhysicsManager::createVehicle(){
	int upAxis = 1;

	m_dynamicsWorld ->getSolverInfo().m_minimumSolverBatchSize = 128;//for direct solver, it is better to solve multiple objects together, small batches have high overhead
	m_dynamicsWorld->getSolverInfo().m_globalCfm = 0.00001;

	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(0,0,0));

	CUBE_HALF_EXTENTS = chassisSize.x();
	btCollisionShape* chassisShape = new btBoxShape(chassisSize);//btVector3(1.f,0.5f,2.f));
	m_collisionShapes.push_back(chassisShape);

	btCompoundShape* compound = new btCompoundShape();
	m_collisionShapes.push_back(compound);
	btTransform localTrans;
	localTrans.setIdentity();
	//localTrans effectively shifts the center of mass with respect to the chassis
	localTrans.setOrigin(btVector3(0,1,0));

	compound->addChildShape(localTrans,chassisShape);
	//Protruding part for the fork demo
	{
		btCollisionShape* suppShape = new btBoxShape(btVector3(CUBE_HALF_EXTENTS,12.f,2.5));
		btTransform suppLocalTrans;
		suppLocalTrans.setIdentity();
		//localTrans effectively shifts the center of mass with respect to the chassis
		suppLocalTrans.setOrigin(btVector3(0,0.50,chassisSize.z()/2-2.5));
		compound->addChildShape(suppLocalTrans, suppShape);
	}

	tr.setOrigin(btVector3(0,-30.f,20));

	m_carChassis = localCreateRigidBody(450,tr,compound);//chassisShape);
	m_carChassis->setUserPointer(chassisAttachedNode);
	//m_carChassis->setDamping(0.2,0.2);

	m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth,wheelRadius,wheelRadius));


	/////// create vehicle
	{

		m_vehicleRayCaster = new btDefaultVehicleRaycaster(m_dynamicsWorld);
		m_vehicle = new btRaycastVehicle(m_tuning,m_carChassis,m_vehicleRayCaster);

		///never deactivate the vehicle
		m_carChassis->setActivationState(DISABLE_DEACTIVATION);

		m_dynamicsWorld->addVehicle(m_vehicle);

		float connectionHeight = -7.0f;


		bool isFrontWheel=true;

		//choose coordinate system
		m_vehicle->setCoordinateSystem(rightIndex,upIndex,forwardIndex);


		btVector3 connectionPointCS0(CUBE_HALF_EXTENTS-(0.3*wheelWidth),connectionHeight,2*CUBE_HALF_EXTENTS-wheelRadius);

		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
		connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),connectionHeight,2*CUBE_HALF_EXTENTS-wheelRadius);

		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
		connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),connectionHeight,-2*CUBE_HALF_EXTENTS+wheelRadius);
		isFrontWheel = false;
		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
		connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(0.3*wheelWidth),connectionHeight,-2*CUBE_HALF_EXTENTS+wheelRadius);
		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

		for (int i=0;i<m_vehicle->getNumWheels();i++)
		{
			btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
			wheel.m_suspensionStiffness = suspensionStiffness;
			wheel.m_wheelsDampingRelaxation = suspensionDamping;
			wheel.m_wheelsDampingCompression = suspensionCompression;
			wheel.m_frictionSlip = wheelFriction;
			wheel.m_rollInfluence = rollInfluence;
			wheel.m_raycastInfo.m_groundObject = groundBody;

		}

	}

}

void PhysicsManager::updateVehicle(){
	if(m_vehicle ){
		std::vector<void*> allWheels ;
		allWheels.push_back(wheel_1_AttachedNode);
		allWheels.push_back(wheel_2_AttachedNode);
		allWheels.push_back(wheel_3_AttachedNode);
		allWheels.push_back(wheel_4_AttachedNode);

		for (int i=0;i<m_vehicle->getNumWheels();i++)
		{
			//synchronize the wheels with the (interpolated) chassis worldtransform
			m_vehicle->updateWheelTransform(i,true);

			btTransform tr = m_vehicle->getWheelInfo(i).m_worldTransform;
			btVector3 pos = tr.getOrigin();
			btQuaternion orn = tr.getRotation();
			quaternion q1(0,0,1,PI/4);
			quaternion q(orn.getX(),orn.getY(),orn.getZ(),orn.getW());

			vector3df Euler;
			q.toEuler(Euler);
			Euler *= RADTODEG;
			//m_vehicle->getWheelInfo(i).m_raycastInfo.m_groundObject = groundBody;
			void* wheel = allWheels.at(i);
			ISceneNode* node = reinterpret_cast<ISceneNode*>(wheel);
			if(node){
				node->setPosition(vector3df(pos.x(),pos.y(),pos.z()));
				node->setRotation(Euler);

			}


		}
	}
}

void PhysicsManager::updatePhysics(){



	int numOfObjects = m_dynamicsWorld->getNumCollisionObjects();
	for (int j = numOfObjects - 1; j >= 0; j--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);

			ISceneNode* cube = reinterpret_cast<ISceneNode*>( body->getUserPointer());
			if(cube != NULL){//update cubes only
				btVector3 origin = trans.getOrigin();
				//update rotation
				btQuaternion rot = trans.getRotation();
				quaternion q(rot.getX(),rot.getY(),rot.getZ(),rot.getW());
				vector3df Euler;
				q.toEuler(Euler);
				Euler *= RADTODEG;

				cube->setPosition(vector3df(origin.getX(),origin.getY(),origin.getZ()));
				cube->setRotation(Euler);
				cube->setVisible(true);
			}
		}
		else
		{
			trans = obj->getWorldTransform();
		}

	}
}

void PhysicsManager::moveForward(){
	gEngineForce = maxEngineForce;
	gBreakingForce = 0.f;
	moveForwardRequested = true;
}

void PhysicsManager::moveBackward(){
	gEngineForce = -maxEngineForce;
	gBreakingForce = 0.f;
	moveForwardRequested = false;
	movebackwardRequested = true;
}
void PhysicsManager::turnRight(){
	gVehicleSteering += steeringIncrement;
	if (	gVehicleSteering > steeringClamp)
		gVehicleSteering = steeringClamp;
}
void PhysicsManager::turnLift(){
	gVehicleSteering -= steeringIncrement;
	if (	gVehicleSteering < -steeringClamp)
		gVehicleSteering = -steeringClamp;
}
void PhysicsManager::setWheelRadius(float radius){
	this->wheelRadius = radius;
}
void PhysicsManager::setWheelWidth(float width){
	this->wheelWidth = width;
}
void PhysicsManager::setChassisSize(btVector3& size){
	this->chassisSize = size;
}

void PhysicsManager::createPhysicsBoxFromEye(vector3df& position,vector3df& lookat,void* attachedNode){
	//create a dynamic rigidbody

	btCollisionShape* colShape = new btBoxShape(btVector3(5,5,5));
	//btCollisionShape* colShape = new btBoxShape(btScalar(1.));
	m_collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(66.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(position.X, position.Y, position.Z));
	btQuaternion quat(btVector3(0.4,.02,.1),67);
	startTransform.setRotation(quat);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
//	body->setUserIndex(10);
	body->setDamping(0.01,0.01);

	body->setUserPointer(attachedNode);
	body->setFriction(1);
	btVector3 force(lookat.X,lookat.Y,lookat.Z);


	m_dynamicsWorld->addRigidBody(body);
	body->applyImpulse(1350*force,btVector3(0.,0.,0.));

}


btRigidBody* PhysicsManager::localCreateRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);

	btRigidBody* body = new btRigidBody(cInfo);
	//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else
	btRigidBody* body = new btRigidBody(mass,0,shape,localInertia);
	body->setWorldTransform(startTransform);
#endif//
//	body->setUserIndex(55);
	m_dynamicsWorld->addRigidBody(body);
	return body;
}
