// modified by Reza (newleon@gmail.com)
// may 6, 2011

#include <irrlicht.h>
#include <iostream>
#include <IrrBP.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

static CIrrBPManager * bulletmgr;
static ISceneManager* smgr;
CIrrBPRaycastVehicle* ve;
//CIrrBPBoxBody* box;

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
	std::cout<<"CollisionPoint: "<<point.X<<" "<<point.Y<< " "<<point.Z<<std::endl;
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
		//	ve->applyEngineForce(100000,0);
		//	ve->applyEngineForce(100000,1);
			ve->applyEngineForce(100000,2);
			ve->applyEngineForce(100000,3);
		//	std::cout<<ve->getVehiclePtr()->getWheelInfo(1).m_engineForce;
/*			IMeshSceneNode * node = smgr->addCubeSceneNode(10,0,-1,smgr->getActiveCamera()->getPosition());
			
			CIrrBPBoxBody * body = bulletmgr->addRigidBox(node,10);
			
			irr::core::vector3df rot = smgr->getActiveCamera()->getRotation();
			irr::core::matrix4 mat;
	
			mat.setRotationDegrees(rot);
			irr::core::vector3df forwardDir(irr::core::vector3df(mat[8],mat[9],mat[10]) *120);
			
			body->getBodyPtr()->setLinearVelocity(bullet::irrVectorToBulletVector(forwardDir) * 70);
			body->addAnimator(bulletmgr->createCollisionCallbackAnimator(ON_COLLISION_RELEASE,&cBack));
			body->addAnimator(bulletmgr->createCollisionDeleteAnimator(ON_COLLISION_RELEASE));

			box->setPosition(vector3df(0,10,0));
 			box->getIrrlichtNode()->setPosition(vector3df(0,10,0));
			
			box->getMotionState()->getWorldTransform(bullet::getTransformFromIrrlichtNode(box->getIrrlichtNode()));
*/		}
		if(event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
		{
//			box->setKinematic(false);
		}
		
	}
	return false;
}

int main()
{
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(1280, 720));
	IVideoDriver* driver = device->getVideoDriver();
	smgr = device->getSceneManager();

	Receiver* recv = new Receiver();
	device->setEventReceiver(recv);

	IAnimatedMesh* mesh = smgr->getMesh("box.3ds");
	IMeshSceneNode* node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
	node->setScale(vector3df(500,1.5,500));
	node->setMaterialTexture(0,driver->getTexture("box0.jpg"));
	node->setMaterialFlag(EMF_LIGHTING,false);
	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100, 0.5f);
	cam->setPosition(vector3df(-20,60,-30));
	
	device->getCursorControl()->setVisible(false);

	bulletmgr = createBulletManager(device);
	bulletmgr->getWorld()->setGravity(vector3df(0, -10, 0));
	bulletmgr->getWorld()->setTimeStep(1.0f/1000.0f);
	bulletmgr->addTrimesh(node, 0);

    //-----------------------------------------------------------------------
    // setup vehicle
    //-----------------------------------------------------------------------

	ISceneNode* chassis = smgr->addCubeSceneNode(2,0,-1,vector3df(0,20,0));
	//ISceneNode* chassis = smgr->addMeshSceneNode(smgr->getMesh("box.3ds"));
	
	chassis->setMaterialFlag(EMF_LIGHTING, false);
	chassis->setScale(vector3df(0.5, 0.5, 0.5));
	ve = bulletmgr->addRaycastVehicle(bulletmgr->addRigidBox(chassis, 400));
//	cam->setTarget(chassis->getPosition());
	#define CUBE_HALF_EXTENTS 1.0f
	float wheelRadius = 0.5f;
	float wheelWidth = 0.4f;
	float connectionHeight = 1.2f;
	bool isFrontWheel = true;
	vector3df wheelDirectionCS0(0,-1,0);
	vector3df wheelAxleCS(-1,0,0);
	irrBPWheelInfoConstructionInfo info;
	f32 suspensionRestLength(0.6f);

    //-----------------------------------------------------------------------
    // setup wheels
    //-----------------------------------------------------------------------

	vector3df connectionPointCS0(CUBE_HALF_EXTENTS-(0.3f*wheelWidth), connectionHeight, 2.0f*CUBE_HALF_EXTENTS-wheelRadius);
	info.m_chassisConnectionCS = connectionPointCS0;
	info.m_wheelAxleCS = wheelAxleCS;
	info.m_wheelDirectionCS = wheelDirectionCS0;
	info.m_suspensionRestLength = suspensionRestLength;
	info.m_bIsFrontWheel = isFrontWheel;
	info.m_wheelRadius = wheelRadius;
	ve->addWheel(info); // wheel 1
	
	connectionPointCS0 = vector3df(-CUBE_HALF_EXTENTS+(0.3f*wheelWidth), connectionHeight, 2.0f*CUBE_HALF_EXTENTS-wheelRadius);
	info.m_chassisConnectionCS = connectionPointCS0;
	info.m_bIsFrontWheel = isFrontWheel;
	ve->addWheel(info); // wheel 2

	connectionPointCS0 = vector3df(-CUBE_HALF_EXTENTS+(0.3f*wheelWidth), connectionHeight, -2.0f*CUBE_HALF_EXTENTS+wheelRadius);
	info.m_chassisConnectionCS = connectionPointCS0;
	info.m_bIsFrontWheel = false;
	ve->addWheel(info); // wheel 3

	connectionPointCS0 = vector3df(CUBE_HALF_EXTENTS-(0.3f*wheelWidth), connectionHeight, -2.0f*CUBE_HALF_EXTENTS+wheelRadius);
	info.m_chassisConnectionCS = connectionPointCS0;
	info.m_bIsFrontWheel = false;
	ve->addWheel(info); // wheel 4

    //-----------------------------------------------------------------------
    // simulation loop
    //-----------------------------------------------------------------------

	int lastFPS = -1;

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, SColor(255,200,200,200));
			//bulletmgr->getWorld()->stepDebugDrawer();
			bulletmgr->stepSimulation();

			smgr->drawAll();
		
			int fps = driver->getFPS();

			if (lastFPS != fps)
			{
				
				stringw str = L"Irrlicht Engine - Quake 3 Map example [";
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

	//	In the end, delete the Irrlicht device.
	delete recv;
	bulletmgr->drop();
	device->drop();
	return 0;
}