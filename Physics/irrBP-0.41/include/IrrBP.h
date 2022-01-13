/*IRR-BULLET MAIN INCLUDE.*/
/*					IRR-BULLET
			An Irrlicht-Bullet Wrapper.
					  V.0.4.0
*/

/*!
    @mainpage IrrBP 0.4.0 Documentation


    @section intro Introduction

    Welcome to IrrBP - an Irrlicht-Bullet Physics wrapper.

	I wrote those few class to help you intagrate physics into your game, without knowing Bullet Physics Engine.
	The integration is very simple, and the function are very (irrlicht)user-friendly.
	You don't need to worry about optimization and memory leaks...IrrBP is alreading doing this!

	IrrBP comes out without any kind of memory leak, and its fully performant.

    If you have any questions or suggestions, email me at stefanoguerrini93@gmail.com


    @section IrrBPexample IrrBP Integration Example

	This example, shows you how to simply integrate bullet physics into your code.
	Here is it:

    @code
#include <irrlicht.h>
#include <IrrBP.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;


static CIrrBPManager * bulletmgr;
static ISceneManager* smgr;
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

bool Receiver::OnEvent(const irr::SEvent &event)
{

	if(event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
		{
			irr::scene::ISceneNode * node = smgr->addCubeSceneNode(10,0,-1,smgr->getActiveCamera()->getPosition());
			CIrrBPBoxBody * body = bulletmgr->addRigidBox(node,40);
			irr::core::vector3df rot = smgr->getActiveCamera()->getRotation();
			irr::core::matrix4 mat;
	
			mat.setRotationDegrees(rot);
			irr::core::vector3df forwardDir(irr::core::vector3df(mat[8],mat[9],mat[10]) *120);

			body->getBodyPtr()->setLinearVelocity(bullet::irrVectorToBulletVector(forwardDir) * 2);
	
		}
		
	}

				
	return false;
}
int main()
{

	

	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480));
	Receiver * recv = new Receiver();
	if (device == 0)
		return 1; // could not create selected driver.
	
	video::IVideoDriver* driver = device->getVideoDriver();
	 smgr = device->getSceneManager();

	device->getFileSystem()->addZipFileArchive("map-20kdm2.pk3");

	device->setEventReceiver(recv);

	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::IMeshSceneNode* node = 0;

	if (mesh)
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);

	if (node)
		node->setPosition(core::irr::core::vector3df(-1350,-130,-1400));

	
	ICameraSceneNode * cam =  smgr->addCameraSceneNodeFPS(0,100,0.1f);
	cam->setPosition(irr::core::vector3df(-20,60,-30));

	
	device->getCursorControl()->setVisible(false);

	bulletmgr = createBulletManager(device);
	bulletmgr->getWorld()->setGravity(irr::core::vector3df(0,-10,0));
	bulletmgr->addTrimesh(node,0);

	int xshift,yshift,zshift;
	IMeshSceneNode * Node;
	IMeshSceneNode * Node2;
	Node = smgr->addCubeSceneNode(5,0,-1,irr::core::vector3df(-20,30,0));

	Node->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	Node->setMaterialFlag(EMF_LIGHTING,false);
	Node->setMaterialTexture(0,driver->getTexture("sphere1.jpg"));
	CIrrBPBoxBody * box= bulletmgr->addRigidBox(Node,0);
	
	Node2 = smgr->addCubeSceneNode(5,0,-1,irr::core::vector3df(20,0,-20));
	Node2->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	Node2->setMaterialFlag(EMF_LIGHTING,false);
	Node2->setMaterialTexture(0,driver->getTexture("sphere1.jpg"));
	CIrrBPBoxBody * box2 = bulletmgr->addRigidBox(Node2,40);
	
	bulletmgr->buildSlideConstraint(box,box2);

	int lastFPS = -1;

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255,200,200,200));
			bulletmgr->stepSimulation();
			driver->setTransform(ETS_WORLD,matrix4());

			smgr->drawAll();
		
			int fps = driver->getFPS();

			if (lastFPS != fps)
			{
				
				core::stringw str = L"irrBP Example - HelloWorld [";
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

	delete recv;
	bulletmgr->drop();
	device->drop();
	return 0;
}
    @endcode

    @section linker Linker Settings

	Before you can compile the above example, you need to include a few libraries into your project:
	<ol>
		<li>Download BulletPhysics from <a href="http://code.google.com/p/bullet/downloads/list">here</a>
		<li>Create a solution for your compiler with cmake
		<li>Compile all static libraries
		<li>Import 
		<ul>
			<li>BulletCollision.lib
			<li>BulletDynamics.lib
			<li>BulletSoftBody.lib
			<li>LinearMath.lib
		</ul>
		<li> Add IrrBP Source and Include directory into your project
	</ol>

<b>Important NOTE:</b>
If you use a bullet version, different from the irrBP official bullet supported version (2.76 right now), you must pay attention when compiling BULLET PHYSICS:
To avoid linking problems in your project you need to verify (and even change) that the runtime library in BULLET_PHYSICS project is setted to "MultiThreaded".


*/

#ifndef C_IrrBP_H_
#define C_IrrBP_H_


#ifdef _WIN32 
	#ifndef WIN32
	#define WIN32
	#endif
#endif



#include "types.h"
#include "convert.h"
#include "CIrrBPWorld.h"
#include "CIrrBPManager.h"

static CIrrBPManager * createBulletManager(irr::IrrlichtDevice *device)
{
	return new CIrrBPManager(device);
}
static CIrrBPManager * createMultithreadedBulletManager(irr::IrrlichtDevice *device, int numThread=4)
{
	return new CIrrBPManager(device,numThread);
}

#endif
