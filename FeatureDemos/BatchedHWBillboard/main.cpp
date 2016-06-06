/** Example 008 SpecialFX

*/

#include <irrlicht.h>
#include <iostream>
#include "driverChoice.h"
#include "CHardwareBillboardSceneNode.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


int main()
{
	char i;
	printf("Please press 'a' if you want to use hardware accelerated billboards or 'b' if you want to use batched billboards.\n");

	std::cin >> i;

	const bool hardware = (i == 'a');
	const bool batched = hardware||(i == 'b');

	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;


	/*
	Create device and exit if creation failed. We make the stencil flag
	optional to avoid slow screen modes for runs without shadows.
	*/

	IrrlichtDevice *device =
		createDevice(driverType, core::dimension2d<u32>(640, 480),
		24, false, false);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();


	/*
	For our environment, we load a .3ds file. It is a small room I modelled
	with Anim8or and exported into the 3ds format because the Irrlicht
	Engine does not support the .an8 format. I am a very bad 3d graphic
	artist, and so the texture mapping is not very nice in this model.
	Luckily I am a better programmer than artist, and so the Irrlicht
	Engine is able to create a cool texture mapping for me: Just use the
	mesh manipulator and create a planar texture mapping for the mesh. If
	you want to see the mapping I made with Anim8or, uncomment this line. I
	also did not figure out how to set the material right in Anim8or, it
	has a specular light color which I don't really like. I'll switch it
	off too with this code.
	*/

	scene::IAnimatedMesh* mesh = smgr->getMesh("../../media/room.3ds");

	smgr->getMeshManipulator()->makePlanarTextureMapping(mesh->getMesh(0), 0.004f);

	scene::ISceneNode* node = 0;

	node = smgr->addAnimatedMeshSceneNode(mesh);
	node->setMaterialTexture(0, driver->getTexture("../../media/wall.jpg"));
	node->getMaterial(0).SpecularColor.set(0,0,0,0);
    node->setMaterialFlag(video::EMF_LIGHTING, false);


    f32 scale = 12.f;
    if (hardware) {
        scene::IBatchedBillboardSceneNode* batchedNode = new scene::CHardwareBillboardSceneNode(smgr->getRootSceneNode(),smgr);
        for (u32 i=0;i<32;i++) {
            for (u32 j=0;j<16;j++) {
                for (u32 k=0;k<32;k++) {
                    node = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(50, 50));
                    node->setMaterialFlag(video::EMF_LIGHTING, false);
                    node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
                    node->setMaterialTexture(0, driver->getTexture("../../media/particlewhite.bmp"));
                    node->setPosition(core::vector3df(i*scale,j*scale,k*scale)-core::vector3df(8,0,8)*scale);
                    node->setScale(core::vector3df(1+rand()%32)*0.09);
                    batchedNode->addBillboard((scene::IBillboardSceneNode*)node);
                }
            }
        }
        printf("Billboards added :   %u \n",batchedNode->getCount()*batchedNode->isVisible());
    }
    else if (batched) {
        scene::IBatchedBillboardSceneNode* batchedNode = new scene::CBatchedBillboardSceneNode(smgr->getRootSceneNode(),smgr);
        for (u32 i=0;i<32;i++) {
            for (u32 j=0;j<16;j++) {
                for (u32 k=0;k<32;k++) {
                    node = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(50, 50));
                    node->setMaterialFlag(video::EMF_LIGHTING, false);
                    node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
                    node->setMaterialTexture(0, driver->getTexture("../../media/particlewhite.bmp"));
                    node->setPosition(core::vector3df(i*scale,j*scale,k*scale)-core::vector3df(8,0,8)*scale);
                    node->setScale(core::vector3df(1+rand()%32)*0.09);
                    batchedNode->addBillboard((scene::IBillboardSceneNode*)node);
                }
            }
        }
        batchedNode->setLoDAngle(0.f);
        //scene::ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(core::vector3df(64.f,32.f,-64.f),64.f,2.4f,core::vector3df(1.f));
        //node->addAnimator(anim);
        printf("Billboards added :   %u \n",batchedNode->getCount()*batchedNode->isVisible());
    }
    else {
        for (u32 i=0;i<32;i++) {
            for (u32 j=0;j<16;j++) {
                for (u32 k=0;k<32;k++) {
                    node = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(50, 50));
                    node->setMaterialFlag(video::EMF_LIGHTING, false);
                    node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
                    node->setMaterialTexture(0, driver->getTexture("../../media/particlewhite.bmp"));
                    node->setPosition(core::vector3df(i*scale,j*scale,k*scale)-core::vector3df(8,0,8)*scale);
                    node->setScale(core::vector3df(1+rand()%32)*0.09);
                }
            }
        }
    }


	/*
	Finally we simply have to draw everything, that's all.
	*/

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(core::vector3df(-50,50,-150));

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	s32 lastFPS = -1;

	while(device->run())
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, 0);

		smgr->drawAll();

		driver->endScene();

		const s32 fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw str = L"Irrlicht Engine - Batched Hardware Billboards example [";
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

/*
**/
