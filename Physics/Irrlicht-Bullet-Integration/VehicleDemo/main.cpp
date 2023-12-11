
#include "PhysicsManager.h"
#include "../IrrlichtBulletIntegration/InputEventHandler.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int lastFPS = -1;
//Debug text view
IGUIStaticText* textNode;
ISceneManager* scenemgr;
IrrlichtDevice *device;
IVideoDriver* driver ;
ICameraSceneNode* cameraNode;

void showFPS( IrrlichtDevice *device ){
	int fps = driver->getFPS();
	if (lastFPS != fps)
	{
		stringw str = L"Irrlicht-Bullet Vehicle Demo [";
		str += driver->getName();
		str += "] FPS:";
		str += fps;
		device->setWindowCaption(str.c_str());
		lastFPS = fps;
	}
}

void createBoxProjectile(PhysicsManager& pm){

	ISceneNode* cubeNode = scenemgr->addCubeSceneNode(10.0f,NULL,-1,vector3df(0,3,10));
	cubeNode->setMaterialType(EMT_SOLID);
	cubeNode->setMaterialTexture(0,driver->getTexture("../media/box.jpg"));
	cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);
	cubeNode->setVisible(false);

	vector3df lookat =  cameraNode->getTarget();
	vector3df	 pose =  cameraNode->getPosition();
	pm.createPhysicsBoxFromEye(pose,(lookat-pose).normalize(),cubeNode);
}

ISceneNode* createWheel(){

	IAnimatedMesh* m = scenemgr->getMesh( "../media/wheel.dae");
	IAnimatedMeshSceneNode* animModel = scenemgr->addAnimatedMeshSceneNode(m);
	animModel->setMaterialTexture(0, driver->getTexture("../media/texture_tire1.jpg"));
	animModel->setMaterialFlag(video::EMF_LIGHTING, false);

	return animModel;
}

ISceneNode* createChassis(){
	IMeshSceneNode* chassis = scenemgr->addCubeSceneNode(20,NULL,-1,vector3df(0,0,0),vector3df(0,0,0),
		vector3df(1,0.25,2));
	chassis->setMaterialTexture(0, driver->getTexture("../media/box1.jpg"));
	chassis->setMaterialFlag(video::EMF_LIGHTING, false);

	IMeshSceneNode* chassisCabinet = scenemgr->addCubeSceneNode(5,chassis,-1,vector3df(0,30,4),vector3df(0,0,0),
		vector3df(3.5,10,1));
	chassisCabinet->setMaterialTexture(0, driver->getTexture("../media/box1.jpg"));
	chassisCabinet->setMaterialFlag(video::EMF_LIGHTING, false);

	return chassis;
}

int main(int argc,char* argv[])
{
	PhysicsManager physicsManager;
	physicsManager.initPhysics();

	//Key event handler
	InputEventHandler myEventHandler;
	// start up the irrlicht engine
	device = createDevice(video::EDT_OPENGL,
		dimension2d<u32>(800,600), 32, false, false, false,
		&myEventHandler);

	driver = device->getVideoDriver();
	scenemgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();


	//cameraNode = scenemgr->addCameraSceneNode();
	// add a first person shooter style user controlled camera
	cameraNode = scenemgr->addCameraSceneNodeFPS(NULL,20.0f,0.02f);
	cameraNode->setPosition(vector3df(0,-30,-20));
	///////////////////////////Ground///////////////////////////////////
	IAnimatedMesh* hillPlaneMesh = scenemgr->addHillPlaneMesh( "myHill",
		core::dimension2d<f32>(30,30),
		core::dimension2d<u32>(10,10), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(10,10));
	ISceneNode* planeNode = scenemgr->addAnimatedMeshSceneNode(hillPlaneMesh);
	planeNode->setMaterialTexture(0, driver->getTexture("../media/stones.jpg"));
	planeNode->setMaterialFlag(video::EMF_LIGHTING, false);
	planeNode->setPosition(core::vector3df(0,-50,150));

	physicsManager.createPhysicsGroundObject(30*10/2,30*10/2,btVector3(0,-50,150),planeNode);

	////////////////////////////////////////////////////////////
	//		Creating the vehicle
	//////////////////////////////////////////////////////////////
	physicsManager.setWheelRadius(3.5f);
	physicsManager.setWheelWidth(2.2f);
	ISceneNode* chassis =  createChassis();

//	physicsManager.setChassisSize(btVector3(10,2.5f,20));
	physicsManager.setAttachedChassisNode(chassis);

	ISceneNode* wheel1 = createWheel();
	ISceneNode* wheel2 = createWheel();
	ISceneNode* wheel3 = createWheel();
	ISceneNode* wheel4 = createWheel();
	physicsManager.setAttachedWheelNodes(wheel1,wheel2,wheel3,wheel4);
	physicsManager.setMaxEnginePower(1000);
	physicsManager.createVehicle();
	////////////////////////////////////////////////////////////////
	stringw instructions = L"Irrlicht - Bullet Vehicle Demo\n";
	instructions += L"Press (W) to Move Forward\n";
	instructions += L"Press (S) to Move Backward\n";
	instructions += L"Press (D) to Turn Right\n";
	instructions += L"Press (A) to Turn Left\n";
	instructions += L"Click (E) to Throw a Box\n";
	instructions += L"Use Arrow Keys to move around";


	textNode = guienv->addStaticText(instructions.c_str(),
		rect<s32>(10,10,260,driver->getScreenSize().Height/4), true);

	u32 then = device->getTimer()->getTime();
	while(device->run() && driver /*&& device->isWindowActive()*/)
	{
		const u32 now = device->getTimer()->getTime();
		driver->beginScene(true, true, video::SColor(255,0,0,255));
		scenemgr->drawAll();
		guienv->drawAll();
		driver->endScene();

		if(myEventHandler.IsKeyDown(KEY_SPACE))
			break;
		// Drop a Box
		if(myEventHandler.IsKeyDown(KEY_KEY_W)){
			physicsManager.moveForward();
			myEventHandler.ResetCode(KEY_KEY_W);
		}
		//Create hanging chain
		if(myEventHandler.IsKeyDown(KEY_KEY_S)){
			physicsManager.moveBackward();
			myEventHandler.ResetCode(KEY_KEY_S);
		}

		if(myEventHandler.IsKeyDown(KEY_KEY_A)){
			physicsManager.turnLift();
			myEventHandler.ResetCode(KEY_KEY_A);
		}
		if(myEventHandler.IsKeyDown(KEY_KEY_D)){
			physicsManager.turnRight();
			myEventHandler.ResetCode(KEY_KEY_D);
		}
		//Fire a box
		if(myEventHandler.IsKeyDown(KEY_KEY_E)){
			createBoxProjectile(physicsManager);
			myEventHandler.ResetCode(KEY_KEY_E);
		}

		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		//Simulate Physics
		physicsManager.stepPhysicsWorld(frameDeltaTime);
		physicsManager.updatePhysics();
		physicsManager.updateVehicle();
		then = now;
		showFPS(device);
	}
	physicsManager.destroyPhysics();
	// delete device
	device->drop();
	return 0;
}
