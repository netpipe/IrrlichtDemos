#include <irrlicht.h>
#include <iostream>
#include <IrrBP.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace std;
static CIrrBPManager * bulletmgr;
static ISceneManager* smgr;
CIrrBPRaycastVehicle * ve;
CIrrBPBoxBody * box;
class Receiver : public IEventReceiver
{
public:
		Receiver();
		virtual bool OnEvent(const SEvent& event);
private:
};
Receiver::Receiver()
{

}
class callbacker : public CollisionResultCallback
{
void addSingleResult(const vector3df & point)
{
	cout<<"CollisionPoint: "<<point.X<<" "<<point.Y<< " "<<point.Z<<endl;
	smgr->addCubeSceneNode(5,0,-1,point);
}
};
callbacker cBack;
bool Receiver::OnEvent(const irr::SEvent &event)
{

	if(event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
		{
			ve->applyEngineForce(10,0);
			ve->applyEngineForce(10,1);
			ve->applyEngineForce(10,2);
			ve->applyEngineForce(10,3);
			
			/*IMeshSceneNode * node = smgr->addCubeSceneNode(10,0,-1,smgr->getActiveCamera()->getPosition());
			
			CIrrBPBoxBody * body = bulletmgr->addRigidBox(node,10);
			
			irr::core::vector3df rot = smgr->getActiveCamera()->getRotation();
			irr::core::matrix4 mat;
	
			mat.setRotationDegrees(rot);
			irr::core::vector3df forwardDir(irr::core::vector3df(mat[8],mat[9],mat[10]) *120);
			
			body->getBodyPtr()->setLinearVelocity(irrVectorToBulletVector(forwardDir) * 70);
			body->addAnimator(bulletmgr->createCollisionCallbackAnimator(ON_COLLISION_RELEASE,&cBack));
			body->addAnimator(bulletmgr->createCollisionDeleteAnimator(ON_COLLISION_RELEASE));

			box->setPosition(vector3df(0,10,0));
 			//box->getIrrlichtNode()->setPosition(vector3df(0,10,0));
			
			//box->getMotionState()->getWorldTransform(bullet::getTransformFromIrrlichtNode(box->getIrrlichtNode()));*/
		}
		if(event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
		{
			box->setKinematic(false);
		}
		
	}

				
	return false;
}

int main()
{

	

	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1280, 720));
//	Receiver * recv = new Receiver();
	if (device == 0)
		return 1; // could not create selected driver.
	
	video::IVideoDriver* driver = device->getVideoDriver();
	 smgr = device->getSceneManager();

	device->getFileSystem()->addZipFileArchive("map-20kdm2.pk3");

	//device->setEventReceiver(recv);

	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::IMeshSceneNode* node = 0;

	if (mesh)
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);

	if (node)
		node->setPosition(core::vector3df(-1350,-130,-1400));
	
	
	ICameraSceneNode * cam =  smgr->addCameraSceneNodeFPS(0,100,0.5f);
	cam->setPosition(vector3df(-20,60,-30));
	
	
	device->getCursorControl()->setVisible(false);

	bulletmgr = createBulletManager(device);
	bulletmgr->getWorld()->setGravity(vector3df(0,-10,0));
	bulletmgr->getWorld()->setTimeStep(1.0f/1000.0f);
	bulletmgr->addTrimesh(node,0);
	
	ISceneNode * chassis = smgr->addCubeSceneNode(2,0,-1,vector3df(0,-20,0));
	ve = bulletmgr->addRaycastVehicle(bulletmgr->addRigidBox(chassis,40));
	#define CUBE_HALF_EXTENTS 1
	float	wheelRadius = 0.5f;
	float	wheelWidth = 0.4f;
	float connectionHeight = 1.2f;
	bool isFrontWheel=true;
	vector3df wheelDirectionCS0(0,-1,0);
	vector3df wheelAxleCS(-1,0,0);
	irrBPWheelInfoConstructionInfo info;
	irr::f32 suspensionRestLength(0.6);

	vector3df connectionPointCS0(CUBE_HALF_EXTENTS-(0.3*wheelWidth),connectionHeight,2*CUBE_HALF_EXTENTS-wheelRadius);
	info.m_chassisConnectionCS = connectionPointCS0;
	info.m_wheelAxleCS = wheelAxleCS;
	info.m_wheelDirectionCS = wheelDirectionCS0;
	info.m_suspensionRestLength = suspensionRestLength;
	info.m_bIsFrontWheel = isFrontWheel;
	info.m_wheelRadius = wheelRadius;
	ve->addWheel(info);

	connectionPointCS0 = vector3df(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),connectionHeight,2*CUBE_HALF_EXTENTS-wheelRadius);
	info.m_chassisConnectionCS = connectionPointCS0;
	ve->addWheel(info);

	connectionPointCS0 = vector3df(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),connectionHeight,-2*CUBE_HALF_EXTENTS+wheelRadius);
	info.m_chassisConnectionCS = connectionPointCS0;
	info.m_bIsFrontWheel = false;
	ve->addWheel(info);

	connectionPointCS0 = vector3df(CUBE_HALF_EXTENTS-(0.3*wheelWidth),connectionHeight,-2*CUBE_HALF_EXTENTS+wheelRadius);
	info.m_chassisConnectionCS = connectionPointCS0;
	info.m_bIsFrontWheel = false;
	ve->addWheel(info);
	int lastFPS = -1;

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255,200,200,200));
			//bulletmgr->getWorld()->stepDebugDrawer();
			bulletmgr->stepSimulation();

			smgr->drawAll();
		
			int fps = driver->getFPS();

			if (lastFPS != fps)
			{
				
				core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
			driver->endScene();
			
		}
		else
			device->yield();
	}
	/*
	In the end, delete the Irrlicht device.
*/
//	delete recv;
	bulletmgr->drop();
	device->drop();
	return 0;
}

/*
That's it. Compile and play around with the program.
**/
