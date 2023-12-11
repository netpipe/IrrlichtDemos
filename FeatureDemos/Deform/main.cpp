
/*
  The coolest mesh deformation contest
  ------------------------------------

  In this example we'll squash a mesh flat over a period of time

  I'm lazy, so I start by stealing Niko's water surface scene node, copying the CPP and H files
  and replacing the word CWaterSurfaceSceneNode with CSquashingMeshSceneNode.

  Next, because it inherits CMeshSceneNode1 and that is not visible from outside the engine,
  I'll steal a copy of that too so that we don't have to recompile Irrlicht and add IMeshSceneNode

  So the files are-
       main.cpp                    // a copy paste job from tutorials 1 and 4
       CSquashingMeshSceneNode.cpp // copied, changed and commented water surface node
       CSquashingMeshSceneNode.h   // copied and changed water surface node
       CMeshSceneNode1.cpp          // copied directly from the engine
       CMeshSceneNode1.h            // copied directly from the engine

  Okay, read through the CSquashingMeshSceneNode.cpp and change it to do your
  own deformation. Good luck!

*/

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include "CSquashingMeshSceneNode.h"

int main()
{
    // we'll use the OGL driver
	IrrlichtDevice *device =
		createDevice(EDT_OPENGL, dimension2du(640, 480), 16,
			false, false, false, 0);

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

    // load sydney
	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
    // squash her!
	CSquashingMeshSceneNode* node = new CSquashingMeshSceneNode( mesh->getMesh(0), 0, smgr, 0 );


	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setPosition( vector3df(0,0,50));
		node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
	}

	smgr->addCameraSceneNodeFPS();

    int lastFPS = -1;

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

        node->render();
        node->OnPostRender(device->getTimer()->getTime());

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();

		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Irrlicht Mesh Deformation Contest [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}


	}

	device->drop();

	return 0;
}

