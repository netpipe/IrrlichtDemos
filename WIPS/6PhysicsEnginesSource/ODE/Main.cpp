#include "ODEClass.h"
#include "../Includes/IrrEventsClass.h"
#include "../Includes/DebugDisplayClass.h"

IrrlichtDevice *device;
ODEClass *Physics;
scene::ISceneManager *smgr;
u32 numboxes = 0;
DebugDisplayClass *debugdisplay;

void ThrowBox()
{
	dBodyID body;
	scene::ISceneNode *node;
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("Meshes/techcon2.3ds"));
	node->getMaterial(0).setFlag(video::EMF_LIGHTING,false);
	node->setPosition(smgr->getActiveCamera()->getPosition());
	node->setScale(core::vector3df(0.15,0.15,0.15));
	body = Physics->setPhysics(node,10.0);

	if(body == NULL)
	{
		return;
	}

	core::vector3df camvec = (smgr->getActiveCamera()->getTarget() - smgr->getActiveCamera()->getPosition()).normalize()*300;
	dBodySetForce(body, camvec.X, camvec.Y, camvec.Z);

	numboxes += 1;
}

void Screenshot()
{
	debugdisplay->takeScreenshot();
}

void Quit()
{
	device->closeDevice();
}

int main(int argc, char **argv)
{
 	IrrEvents EventReceiver;
 	EventReceiver.on_key_esc = Quit;
 	EventReceiver.on_mouse_left = ThrowBox;
 	EventReceiver.on_key_f6 = Screenshot;

	device = createDevice(video::EDT_OPENGL,core::dimension2du(640, 480),32,false,false,false,&EventReceiver);
	video::IVideoDriver *driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	gui::IGUIEnvironment *guienv = device->getGUIEnvironment();

	device->setWindowCaption(L"ODEClass Test Environment");

	debugdisplay = new DebugDisplayClass(device);
	debugdisplay->setDebugInt("Number of Boxes: ",&numboxes);

	smgr->addCameraSceneNodeFPS(0,100,0.5);
	device->getCursorControl()->setVisible(false);

	//create the physics instance
	Physics = new ODEClass(device);

	Physics->createStaticGeometry("Meshes/Cube.irrmesh");

	scene::ISceneNode *node;

	int i;
	int j;
	int k;
	for(k = 10; k > 0; k--)
	{
		for(i = 10; i > 0; i--)
		{
			for(j = 1; j > 0; j--)
			{
				node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("Meshes/techcon2.3ds"));
				node->getMaterial(0).setFlag(video::EMF_LIGHTING,false);
				node->setPosition(core::vector3df(-10.0+i*1.1,-4+k*1.1,-10.0+j*1.1));
				node->setScale(core::vector3df(0.15,0.15,0.15));
				Physics->setPhysics(node,1.0);

				numboxes += 1;
			}
		}
	}

	//the main loop
	while(device->run())
	{
		 if (device->isWindowActive())
		 {
			Physics->update();

			driver->beginScene(true, true, video::SColor(255,150,180,255));

			//render the scene
			smgr->drawAll();

			guienv->drawAll();
			driver->endScene();

			debugdisplay->updateDebugDisplay();
		  }
	}

	//delete the physics instance
	delete Physics;

	//drop the irrlicht device
	device->drop();
}
