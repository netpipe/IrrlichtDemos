#include <irrlicht.h>
#include "BoltSceneNode.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif
int main()
{
	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<s32>(640, 480), 16,
			false, false, false, 0);

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation ( scene::EMAT_PAIN_A);
		node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
		node->setPosition(vector3df(10,0,10));
	}
	ISceneNode* cube = smgr->addCubeSceneNode(500);
	cube->setPosition(vector3df(0,-30,0));
	cube->setScale(vector3df(1,0.01f,1));
	cube->setMaterialFlag(EMF_LIGHTING,false);
	cube->setMaterialTexture(0,driver->getTexture("../../media/terrain-texture.jpg"));

	ISceneNode* sphere = smgr->addSphereSceneNode(10);
	sphere->setPosition(vector3df(50,50,50));
	sphere->setMaterialFlag(EMF_LIGHTING,false);
	sphere->setMaterialTexture(0,driver->getTexture("water006.jpg"));
	///////////////////////////////
	irr::scene::CBoltSceneNode* beam = new irr::scene::CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1,"light01_1.bmp");
   beam->setLine(irr::core::vector3df(50,50,50), irr::core::vector3df(0,0,0), 100, 5,10,3, false,10.0f, irr::video::SColor(255,0,0,255));
   beam->drop();
	////////////////////////////////
   ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();
   cam->setPosition(core::vector3df(30,0,-60));
   cam->setTarget(core::vector3df(0,0,0));
	int lastFPS = -1;
	stringw caption;
	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,32,32,32));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
		int fps = driver->getFPS();
		if (fps!=lastFPS)
		{
			lastFPS = fps;
			caption = L"Driver [";
			caption+= driver->getName();
			caption+=L"] FPS : ";
			caption+=fps;
			device->setWindowCaption(caption.c_str());
		}
		device->sleep(1,0);
	}
	device->drop();
	return 0;
}

