/****************************************
Copyright 2018  Mahmoud Galal
****************************************/



#include "../IrrlichtBulletIntegration/baseheader.h"
#include "../IrrlichtBulletIntegration/InputEventHandler.h"

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"

#include <iostream>
using namespace std;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

//BulletDynamics.lib BulletCollision.lib LinearMath.lib BulletSoftBody.lib
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletSoftBody.lib")
#pragma comment(lib, "LinearMath.lib")
#endif


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
//////////////////////////////////////////////


int lastFPS = -1;
//Debug text view
IGUIStaticText* textNode;
ISceneManager* scenemgr;
btSoftBody* cloth;
IVideoDriver* driver ;

void initSoftPhysics(){

	btVector3 worldAabbMin(-1000,-1000,-1000);
	btVector3 worldAabbMax(1000,1000,1000);

	m_collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration(); 
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration); 

	m_broadphase = /*new btAxisSweep3(worldAabbMin,worldAabbMax,32766);*/new btDbvtBroadphase();

	m_solver = new btSequentialImpulseConstraintSolver; 

	m_dynamicsWorld = new btSoftRigidDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
	m_dynamicsWorld->getDispatchInfo().m_enableSPU = true;

	softBodyWorldInfo.m_broadphase = m_broadphase;
	softBodyWorldInfo.m_dispatcher = m_dispatcher;
	softBodyWorldInfo.m_gravity = m_dynamicsWorld->getGravity();
	softBodyWorldInfo.m_sparsesdf.Initialize();
}
//Starts physics simulation
void stepPhysicsWorld(float timestep){
	m_dynamicsWorld->stepSimulation(timestep*2, 10); 
}

void destroyPhysics(){
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

	delete m_solver;
	m_solver=0;

	delete m_broadphase;
	m_broadphase=0;

	delete m_dispatcher;
	m_dispatcher=0;

	delete m_collisionConfiguration;
	m_collisionConfiguration=0;
}


void createSoftBody(const btScalar s,
	const int numX,
	const int numY, 
	const int fixed = 3) {

		cloth = btSoftBodyHelpers::CreatePatch(softBodyWorldInfo,
			btVector3(-s/2,s+1,0),
			btVector3(+s/2,s+1,0),
			btVector3(-s/2,s+1,+s),
			btVector3(+s/2,s+1,+s),
			numX,numY, 
			fixed,true);

		cloth->getCollisionShape()->setMargin(0.0001f);
		btSoftBody::Material* pm = cloth->appendMaterial();
		pm->m_kLST		=	0.004;
		pm->m_kAST		=	0.004;
		pm->m_kVST		=	0.004;
		cloth->generateBendingConstraints(2,pm);
		cloth->setTotalMass(39); 

		//cloth->m_cfg.citerations = 2;
		//cloth->m_cfg.diterations = 2;
		cloth->m_cfg.piterations = 2;
		cloth->m_cfg.kDP = 0.0008f;
		cloth->m_cfg.kLF			=	0.008;
		cloth->m_cfg.kDG			=	0.0006;
		cloth->m_cfg.aeromodel	=	btSoftBody::eAeroModel::V_TwoSidedLiftDrag;
		//cloth->m_cfg.collisions =btSoftBody::fCollision::CL_RS;

		cloth->setWindVelocity(btVector3(20, -90.0, -9.0));

		((btSoftRigidDynamicsWorld*)m_dynamicsWorld)->addSoftBody(cloth);
		//btSoftBodyHelpers::ReoptimizeLinkOrder(cloth);

		IAnimatedMesh* hillPlaneMesh = scenemgr->addHillPlaneMesh( "Curtain",
			core::dimension2d<f32>(4,4),
			core::dimension2d<u32>(numX-1,numY-1), 0, 0,
			core::dimension2d<f32>(0,0),
			core::dimension2d<f32>(10,10));

		ISceneNode* plane =  scenemgr->addAnimatedMeshSceneNode(hillPlaneMesh);
		plane->setMaterialTexture(0, driver->getTexture("../media/box1.jpg"));
		plane->setMaterialFlag(video::EMF_LIGHTING, false);
		plane->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		plane->setPosition(core::vector3df(0,- 40,0));


		cloth->setUserPointer(hillPlaneMesh);
		IMeshBuffer* buffer =  hillPlaneMesh->getMeshBuffer(0);

		cout<<"Number of vertices = "<<buffer->getVertexCount()<<endl;

}

void renderSoftBody(){
	btSoftRigidDynamicsWorld* softWorld =   ((btSoftRigidDynamicsWorld*)m_dynamicsWorld);
	btSoftBodyArray sarray = softWorld->getSoftBodyArray();
	if(sarray.size()>0){
		btSoftBody*	psb=(btSoftBody*)sarray[0];
		if(psb ){
			IAnimatedMesh* plane = (IAnimatedMesh*)psb->getUserPointer();
			IMeshBuffer* buffer =  plane->getMeshBuffer(0);
			//cout<<"Number of Nodes = " <<psb->m_nodes.size()<<endl;
			//

			for(int i = 0;i<buffer->getVertexCount();i++){
				const btSoftBody::Node&	n=psb->m_nodes[i];
				btVector3 pos =  n.m_x;
				btVector3 normal =  n.m_n;

				buffer->getPosition(i).X = pos.x();
				buffer->getPosition(i).Y = pos.y();
				buffer->getPosition(i).Z = pos.z();

				buffer->getNormal(i).X = normal.x();
				buffer->getNormal(i).Y =  normal.y();
				buffer->getNormal(i).Z =  normal.z();
				//buffer[i] = NULL;

			}
		}
	}
}

/**
Creates a ground Physics object
**/
void createPhysicsGroundObject(){
	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(150.), btScalar(1.), btScalar(150.)));

		m_collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

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
		body->setUserIndex(5);
		body->setFriction(1);

		//add the body to the dynamics world
		m_dynamicsWorld->addRigidBody(body);
	}

}

void renderDynamicPhysicsObjects(){
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

void createPhysicsBoxFromEye(ISceneManager* scenemgr,IVideoDriver* driver,vector3df& position,vector3df& lookat){
	//create a dynamic rigidbody

	btCollisionShape* colShape = new btBoxShape(btVector3(5,5,5));
	//btCollisionShape* colShape = new btBoxShape(btScalar(1.));
	m_collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(26.f);

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
	body->setUserIndex(10);
	body->setDamping(0.01,0.01);


	ISceneNode* cubeNode = scenemgr->addCubeSceneNode(10.0f,NULL,-1,vector3df(0,3,10));
	cubeNode->setMaterialType(EMT_SOLID);
	cubeNode->setMaterialTexture(0,driver->getTexture("../media/box.jpg"));
	cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);
	cubeNode->setVisible(false);


	body->setUserPointer(cubeNode);
	body->setFriction(1);
	btVector3 force(lookat.X,lookat.Y,lookat.Z);	


	m_dynamicsWorld->addRigidBody(body);
	body->applyImpulse(1050*force,btVector3(0.,0.,0.));
}


int main(int argc,char* argv[])
{
	//Start physics
	initSoftPhysics();
	//Adding earth
	createPhysicsGroundObject();
	//Key event handler
	InputEventHandler myEventHandler;
	// start up the irrlicht engine
	IrrlichtDevice *device = createDevice(video::EDT_DIRECT3D9,
		dimension2d<u32>(800,600), 32, false, false, false,
		&myEventHandler);

	driver = device->getVideoDriver();
	scenemgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	ICameraSceneNode* cameraNode;

	device->setWindowCaption(L"Irrlicht-Bullet Softbody Demo");


	//Ground////
	IAnimatedMesh* hillPlaneMesh = scenemgr->addHillPlaneMesh( "myHill",
		core::dimension2d<f32>(30,30),
		core::dimension2d<u32>(10,10), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(10,10));
	ISceneNode* planeNode = scenemgr->addAnimatedMeshSceneNode(hillPlaneMesh);
	planeNode->setMaterialTexture(0, driver->getTexture("../media/stones.jpg"));
	planeNode->setMaterialFlag(video::EMF_LIGHTING, false);
	planeNode->setPosition(core::vector3df(0,-55,0));

	// add a first person shooter style user controlled camera
	cameraNode = scenemgr->addCameraSceneNodeFPS(NULL,20.0f,0.02f);
	cameraNode->setPosition(vector3df(0,10,-70));	 
	stringw instructions = L"Irrlicht - Bullet Soft Body Demo\n";
	instructions += L"Click (C) to create Curtain\n";
	instructions += L"Click (W) to Enable/Disable Wind\n";
	instructions += L"Click (E) to Throw a Box\n";
	instructions += L"Use Arrow Keys to move around";


	textNode = guienv->addStaticText(instructions.c_str(),
		rect<s32>(10,10,260,driver->getScreenSize().Height/4), true);

	u32 then = device->getTimer()->getTime();
	// draw everything
	while(device->run() && driver /*&& device->isWindowActive()*/)
	{
		const u32 now = device->getTimer()->getTime();       
		//update Physics

		//Close app
		if(myEventHandler.IsKeyDown(KEY_SPACE))
			break;
		if(myEventHandler.IsKeyDown(KEY_KEY_C)){
			myEventHandler.ResetCode(KEY_KEY_C);
			if(!cloth)
				createSoftBody(4*20,20,20);
		}
		//Fire a box
		if(myEventHandler.IsKeyDown(KEY_KEY_E)){
			vector3df lookat =  cameraNode->getTarget();			 
			vector3df	 pose =  cameraNode->getPosition();
			createPhysicsBoxFromEye(scenemgr,driver,pose,(lookat-pose).normalize());
			myEventHandler.ResetCode(KEY_KEY_E);
		}
		if(myEventHandler.IsKeyDown(KEY_KEY_W)){
			if(cloth)
				if(cloth->getWindVelocity().x() == 0)
					cloth->setWindVelocity(btVector3(25, -90.0, -29.0));
				else
					cloth->setWindVelocity(btVector3(0., 0.0, 0.0));

			myEventHandler.ResetCode(KEY_KEY_W);
		}

		driver->beginScene(true, true, video::SColor(255,0,0,255));
		scenemgr->drawAll();
		guienv->drawAll();
		driver->endScene();

		renderSoftBody();
		renderDynamicPhysicsObjects();
		// display frames per second in window title
		int fps = driver->getFPS();		
		if (lastFPS != fps)
		{
			stringw str = L"Irrlicht-Bullet Softbody Demo [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;			
			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}		
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		//Simulate Physics
		stepPhysicsWorld(frameDeltaTime);
		then = now;	
	}
	// delete device
	device->drop();
	//Clean
	destroyPhysics();

	return 0;
}
