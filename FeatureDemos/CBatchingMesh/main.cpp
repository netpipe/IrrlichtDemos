// Author : Bitplane

#include "irrlicht.h"
#include "CBatchingMesh.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

using namespace irr;
using namespace scene;
using namespace video;

int main()
{
	IrrlichtDevice* device = createDevice( video::EDT_OPENGL, core::dimension2d<u32>(640, 480),
		16, false, false, false);

	if (device == 0)
		return 1;

	ISceneManager *smgr = device->getSceneManager();
	IVideoDriver *driver= device->getVideoDriver();

	IMesh* mesh = smgr->getMesh("media/cube.obj")->getMesh(0);
	mesh->getMeshBuffer(0)->getMaterial().setTexture(0, driver->getTexture("media/grass.png"));
	CBatchingMesh* BatchingMesh = new CBatchingMesh();
	s32 x,y,z;
	// let's add a grassy floor, 20x20
	for (x=-10; x<11; ++x)
		for (z=-10; z<11; ++z)
			if ( abs(x) > 5 || abs(z) > 5)
				BatchingMesh->addMesh(mesh, core::vector3df(x*2,-4,z*2));

	mesh->getMeshBuffer(0)->getMaterial().setTexture(0, driver->getTexture("media/stone.png"));
	// add some walls
	for (y=-1; y<7; ++y)
		for (x=-5; x<6; ++x)
			for (z=-5; z<6; ++z)
				if (((abs(z) == 5 || abs(x) == 5) && (abs(x)>1 || y > 3 )) || y==6)
					BatchingMesh->addMesh(mesh, core::vector3df(x*2,y*2-2,z*2));

	mesh->getMeshBuffer(0)->getMaterial().setTexture(0, driver->getTexture("media/tile.png"));
	// a nice tiled interior
	for (x=-5; x<6; ++x)
		for (z=-5; z<6; ++z)
			BatchingMesh->addMesh(mesh, core::vector3df(x*2,-4,z*2));

	BatchingMesh->update();
	BatchingMesh->setMaterialFlag(EMF_LIGHTING, false);

	ISceneNode *node = smgr->addMeshSceneNode(BatchingMesh);
	node->setScale(core::vector3df(10,10,10));
	node->setDebugDataVisible(EDS_BBOX_BUFFERS);
	BatchingMesh->drop();


	/*
	To be able to look at and move around in this scene,
	we create a first person shooter style camera and make the
	mouse cursor invisible.
	*/
	smgr->addCameraSceneNodeFPS(0, 100.0f, 0.1f);
	device->getCursorControl()->setVisible(false);


	int lastFPS = -1;

	while(device->run())
		if (device->isWindowActive())
	{
		driver->beginScene(true, true, video::SColor(255,113,113,133));
		smgr->drawAll();
		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"CBatchingMesh [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;
			tmp += L", mesh count: ";
			tmp += BatchingMesh->getSourceBufferCount();

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
			device->sleep(5,0);
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();

	return 0;
}

