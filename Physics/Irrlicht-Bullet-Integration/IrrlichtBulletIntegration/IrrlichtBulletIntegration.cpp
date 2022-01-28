/****************************************
	Copyright 2018  Mahmoud Galal
****************************************/


#include "baseheader.h"
#include "InputEventHandler.h"
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

//BulletDynamics.lib BulletCollision.lib LinearMath.lib
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "LinearMath.lib")
#endif

#include "physicsutils.h"

 int lastFPS = -1;
 //Debug text view
 IGUIStaticText* textNode;
 //Physics//////////////////////
 btDiscreteDynamicsWorld* dynamicsWorld;
 btSequentialImpulseConstraintSolver* solver;
 btBroadphaseInterface* overlappingPairCache;
 btCollisionDispatcher* dispatcher;
 btDefaultCollisionConfiguration* collisionConfiguration;
 //keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
//////////////////////////////////////////////
/*
	Initializes Bullet Physics....
*/
 void initializePhysics(){
	 ///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	///-----initialization_end-----
 }

 /**
  Free Physics
 **/
 void destroyPhysics(){
	 ///-----cleanup_start-----
	 int i;
	 for (i = dynamicsWorld->getNumConstraints() - 1; i >= 0; i--)
            {
                 dynamicsWorld->removeConstraint(dynamicsWorld->getConstraint(i));
            }
	//remove the rigidbodies from the dynamics world and delete them
	for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();

 }

 /**
	Creates a ground Physics object
 **/
 void createPhysicsGroundObject(){
   //the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(150.), btScalar(1.), btScalar(150.)));

		collisionShapes.push_back(groundShape);

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
//		body->setUserIndex(5);
		body->setFriction(1);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

 }

 /**
    Drops a physics box
 **/
 void createPhysicsBox(ISceneManager* scenemgr,IVideoDriver* driver){
	//create a dynamic rigidbody

	 btCollisionShape* colShape = createBoxShape(btVector3(5,5,5));
		//btCollisionShape* colShape = new btBoxShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(3.f);

		startTransform.setOrigin(btVector3(10, 40, 20));
		btQuaternion quat(btVector3(0.4,.02,.1),67);
		startTransform.setRotation(quat);


		btRigidBody* body =  createRigidBody(dynamicsWorld,mass,startTransform,colShape);// new btRigidBody(rbInfo);
		body->setFriction(1);
//		body->setUserIndex(10);

		IMeshSceneNode* cubeNode = scenemgr->addCubeSceneNode(10.0f,NULL,-1,vector3df(0,3,10));
	    cubeNode->setMaterialType(EMT_SOLID);
	    cubeNode->setMaterialTexture(0,driver->getTexture("../media/box1.jpg"));
        cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);
		cubeNode->setVisible(false);
		//saving the visual node to the physics node
		body->setUserPointer(cubeNode);
 }

 /**
	Fires a box from the camera position to the lookat
 **/
 void createPhysicsBoxFromEye(ISceneManager* scenemgr,IVideoDriver* driver,vector3df& position,vector3df& lookat){
	//create a dynamic rigidbody

		btCollisionShape* colShape = new btBoxShape(btVector3(5,5,5));
		//btCollisionShape* colShape = new btBoxShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(6.f);

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
//		body->setUserIndex(10);
		body->setDamping(0.01,0.01);


		ISceneNode* cubeNode = scenemgr->addCubeSceneNode(10.0f,NULL,-1,vector3df(0,3,10));
	    cubeNode->setMaterialType(EMT_SOLID);
	    cubeNode->setMaterialTexture(0,driver->getTexture("../media/box.jpg"));
        cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);
		cubeNode->setVisible(false);


		body->setUserPointer(cubeNode);
		body->setFriction(1);
		btVector3 force(lookat.X,lookat.Y,lookat.Z);


		dynamicsWorld->addRigidBody(body);
		body->applyImpulse(250*force,btVector3(0.,0.,0.));

 }

 //Starts physics simulation
 void stepPhysicsWorld(float timestep){
     dynamicsWorld->stepSimulation(timestep*2, 10);
 }

 // update 3d objects transformations with the Physics results
 void renderPhysicsObjectsPose(){

 //print positions of all objects
	  int numOfObjects = dynamicsWorld->getNumCollisionObjects();
	 stringw str = L"Number of physics objects:";
	 str+=numOfObjects;
	 str+= L"\nPress C to drop a box\n";
	 str+= L"Press H to Create a Chain\n";
	 str+= L"Press E to Fire a box\n";

	 textNode->setText(str.c_str());

		for (int j = numOfObjects - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
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

 /*
		Create a hanging  chain of boxes with joints between parts
 */
 void createChain(int chainitemscount,btVector3& suspensionPoint,ISceneManager* scenemgr,IVideoDriver* driver){

	 //create a few dynamic rigidbodies
		// Re-using the same collision is better for memory usage and performance
        btBoxShape* colShape = createBoxShape(btVector3(4,4,1));

		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);

		btAlignedObjectArray<btRigidBody*> boxes;
		int lastBoxIndex = chainitemscount-1;
		for(int i=0;i<chainitemscount;++i) {
			startTransform.setOrigin(btVector3(
				btScalar(suspensionPoint.x()),
									 btScalar(suspensionPoint.y()+i*2),
									 btScalar(suspensionPoint.z())
									 )
									 );
			boxes.push_back(createRigidBody(dynamicsWorld, (i  ==lastBoxIndex)?0:mass,startTransform,colShape));

			IMeshSceneNode* cubeNode = scenemgr->addCubeSceneNode(8.0f,NULL,-1,vector3df(0,0,0),vector3df(0,0,0),
				vector3df(1,1,0.25f));
			cubeNode->setMaterialType(EMT_SOLID);
			cubeNode->setMaterialTexture(0,driver->getTexture("../media/steel-box.jpg"));
			cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);
			boxes[i]->setUserPointer(cubeNode);
		}

		//add N-1 spring constraints
		for(int i=0;i<chainitemscount-1;++i) {
			btRigidBody* b1 = boxes[i];
			btRigidBody* b2 = boxes[i+1];

			btPoint2PointConstraint* leftSpring = new btPoint2PointConstraint(*b1, *b2, btVector3(-2,4,0), btVector3(-2,-4,0));

			dynamicsWorld->addConstraint(leftSpring);

			btPoint2PointConstraint* rightSpring = new btPoint2PointConstraint(*b1, *b2, btVector3(2,4,0), btVector3(2,-4,0));

			dynamicsWorld->addConstraint(rightSpring);
		}
}


int main(int argc,char* argv[])
{
	//Start physics
	initializePhysics();
	//Adding earth
	createPhysicsGroundObject();
	//Key event handler
	InputEventHandler myEventHandler;
	// start up the irrlicht engine
    IrrlichtDevice *device = createDevice(video::EDT_DIRECT3D9,
        dimension2d<u32>(800,600), 32, false, false, false,
		&myEventHandler);

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* scenemgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	ICameraSceneNode* cameraNode;

    device->setWindowCaption(L"Irrlicht - Bullet Integration");


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

	    textNode = guienv->addStaticText(L"Irrlicht - Bullet Integration Demo",
		 rect<s32>(10,10,260,driver->getScreenSize().Height/4), true);

	  u32 then = device->getTimer()->getTime();
    // draw everything
    while(device->run() && driver /*&& device->isWindowActive()*/)
    {
       const u32 now = device->getTimer()->getTime();
	   //update Physics
		renderPhysicsObjectsPose();
		//Close app
		if(myEventHandler.IsKeyDown(KEY_SPACE))
			break;
		// Drop a Box
		if(myEventHandler.IsKeyDown(KEY_KEY_C)){
			createPhysicsBox(scenemgr,driver);
			myEventHandler.ResetCode(KEY_KEY_C);
		}
		//Create hanging chain
		if(myEventHandler.IsKeyDown(KEY_KEY_H)){
//			createChain(10,btVector3(10+(-30+rand()%100 ),10,20 +(10+rand()%30 ))  ,scenemgr,driver);
			myEventHandler.ResetCode(KEY_KEY_H);
		}
		//Fire a box
		if(myEventHandler.IsKeyDown(KEY_KEY_E)){
			 vector3df lookat =  cameraNode->getTarget();
			 vector3df	 pose =  cameraNode->getPosition();
			 createPhysicsBoxFromEye(scenemgr,driver,pose,(lookat-pose).normalize());
			myEventHandler.ResetCode(KEY_KEY_E);
		}

		driver->beginScene(true, true, video::SColor(255,0,0,255));
        scenemgr->drawAll();
		guienv->drawAll();
        driver->endScene();

		// display frames per second in window title
        int fps = driver->getFPS();
		if (lastFPS != fps)
        {
            stringw str = L"Irrlicht-Bullet Engine Integration [";
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

