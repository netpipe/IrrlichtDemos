#include <vector>
#include <irrlicht.h>
#include "Ragdoll.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace irr;

static std::vector<RagDoll*> v_RagDolls;
static std::vector<btRigidBody*> v_Boxes;

static btDiscreteDynamicsWorld* dynamicsWorld;
static scene::ISceneManager *smgr;

IrrlichtDevice *device;
video::IVideoDriver *driver;
u32 TimeStamp;
u32 DeltaTime;

class Keyboard : public IEventReceiver
{
	bool OnEvent(const SEvent& event)
	{
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
			if (event.MouseInput.isLeftPressed())
				v_RagDolls.push_back(new RagDoll(dynamicsWorld, smgr, btVector3(0,25,0), rand()%10+1.0f));
			else if (event.MouseInput.isRightPressed())
			{
				// add a new box
				scene::ISceneNode *node = smgr->addCubeSceneNode(1);
				node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
				node->setScale(core::vector3df(rand()%3+1,rand()%3+1,rand()%3+1));
				node->setPosition(smgr->getActiveCamera()->getPosition());


				// Set the initial position of the object
				btTransform Transform;
				Transform.setIdentity();
				Transform.setOrigin(btVector3(node->getPosition().X, node->getPosition().Y, node->getPosition().Z));

				// Give it a default MotionState
				btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

				// Create the shape
				btVector3 HalfExtents(node->getScale().X * 0.5f, node->getScale().Y * 0.5f, node->getScale().Z * 0.5f);
				btCollisionShape *Shape = new btBoxShape(HalfExtents);

				// Add mass
				btVector3 LocalInertia;
				Shape->calculateLocalInertia(1, LocalInertia);

				// Create the rigid body object
				btRigidBody *RigidBody = new btRigidBody(1, MotionState, Shape, LocalInertia);

				// Store a pointer to the irrlicht node so we can update it later
				RigidBody->setUserPointer((void *)(node));

				// Add it to the world
				dynamicsWorld->addRigidBody(RigidBody);
				v_Boxes.push_back(RigidBody);
			}

		return false;
	}
};
void main_loop();
int main()
{
	// Irrlicht Stuffs
	Keyboard eventReceiver;
	device = createDevice(video::EDT_OGLES2, core::dimension2du(1280,800), 16, false, false, false, &eventReceiver);
  driver = device->getVideoDriver();
  smgr = device->getSceneManager();

	device->getCursorControl()->setVisible(false);

	scene::ICameraSceneNode *cam = smgr->addCameraSceneNodeFPS(0, 50, 0.05f);
	cam->setPosition(core::vector3df(0,20,-20));
	cam->setTarget(core::vector3df(0));

	scene::IMesh *floor_mesh = smgr->addHillPlaneMesh("floor", core::dimension2df(4,4), core::dimension2du(50,50));
	scene::ISceneNode *floor = smgr->addMeshSceneNode(floor_mesh);

	scene::ILightSceneNode *light = smgr->addLightSceneNode(0, core::vector3df(0,20,0), video::SColorf(0.8,0.8,0.8), 22);



	// Initialize bullet
	btBroadphaseInterface *BroadPhase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000));
	btDefaultCollisionConfiguration *CollisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
	btSequentialImpulseConstraintSolver *Solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(Dispatcher, BroadPhase, Solver, CollisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0,-9.8,0));


	// Floor
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
  btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	u32 TimeStamp = device->getTimer()->getTime();
	DeltaTime = 0;
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else

	while (device->run())
	{
		DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

		dynamicsWorld->stepSimulation(DeltaTime * 0.001f, 60);

		for (std::vector<RagDoll*>::iterator it = v_RagDolls.begin(); it != v_RagDolls.end(); ++it)
			(*it)->Update();

		for (std::vector<btRigidBody*>::iterator it = v_Boxes.begin(); it != v_Boxes.end(); ++it)
		{
			//UpdateRender(*Iterator);
			scene::ISceneNode *Node = static_cast<scene::ISceneNode *>((*it)->getUserPointer());

			// Set position
			btVector3 Point = (*it)->getCenterOfMassPosition();
			Node->setPosition(core::vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));

			// Set rotation
			btVector3 EulerRotation;
			RagDoll::QuaternionToEuler((*it)->getOrientation(), EulerRotation);
			Node->setRotation(core::vector3df(EulerRotation[0], EulerRotation[1], EulerRotation[2]));
		}

		driver->beginScene(true, true, video::SColor(255,80,80,150));

			smgr->drawAll();
			device->getGUIEnvironment()->getBuiltInFont()->draw(L"LEFT CLICK to spawn Rag Doll\nRIGHT CLICK to spawn a Box", core::recti(0,20,driver->getScreenSize().Width,50),
																													video::SColor(255,255,255,255), true, false);
		driver->endScene();
	}
	#endif
	device->drop();

	return 0;
}

void main_loop()
{
	device->run();
	driver->beginScene(true, true, video::SColor(255,200,200,200));
		DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

		dynamicsWorld->stepSimulation(DeltaTime * 0.001f, 60);

		for (std::vector<RagDoll*>::iterator it = v_RagDolls.begin(); it != v_RagDolls.end(); ++it)
			(*it)->Update();

		for (std::vector<btRigidBody*>::iterator it = v_Boxes.begin(); it != v_Boxes.end(); ++it)
		{
			//UpdateRender(*Iterator);
			scene::ISceneNode *Node = static_cast<scene::ISceneNode *>((*it)->getUserPointer());

			// Set position
			btVector3 Point = (*it)->getCenterOfMassPosition();
			Node->setPosition(core::vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));

			// Set rotation
			btVector3 EulerRotation;
			RagDoll::QuaternionToEuler((*it)->getOrientation(), EulerRotation);
			Node->setRotation(core::vector3df(EulerRotation[0], EulerRotation[1], EulerRotation[2]));
		}

		driver->beginScene(true, true, video::SColor(255,80,80,150));

			smgr->drawAll();
			device->getGUIEnvironment()->getBuiltInFont()->draw(L"LEFT CLICK to spawn Rag Doll\nRIGHT CLICK to spawn a Box", core::recti(0,20,driver->getScreenSize().Width,50),
																													video::SColor(255,255,255,255), true, false);
		driver->endScene();
}
