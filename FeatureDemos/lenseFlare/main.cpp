// based on Example 002 Quake3Map

#include <irrlicht.h>
#include <iostream>

#include "LensFlareSceneNode.h"
#include "SceneNodeAnimatorFollowCamera.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{
	// ask user for driver
	//video::E_DRIVER_TYPE driverType;
//
//	printf("Please select the driver you want for this example:\n"\
//		" (a) OpenGL 1.5\n (b) Direct3D 9.0c\n (otherKey) exit\n\n");

//	char i;
//	std::cin >> i;
//
//	switch(i)
//	{
//		case 'a': driverType = video::EDT_OPENGL;   break;
//		case 'b': driverType = video::EDT_DIRECT3D9;break;
//		default: return 1;
//	}

	// create device and exit if creation failed
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1024, 768));
	if (device == 0)
		return 1;

	// Get a pointer to the video driver and the SceneManager so that
	// we do not always have to call irr::IrrlichtDevice::getVideoDriver() and
	// irr::IrrlichtDevice::getSceneManager().
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// create sky box
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	scene::ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
		driver->getTexture("media/skybox/glacier_up.png"),
		driver->getTexture("media/skybox/glacier_dn.png"),
		driver->getTexture("media/skybox/glacier_lf.png"),
		driver->getTexture("media/skybox/glacier_rt.png"),
		driver->getTexture("media/skybox/glacier_ft.png"),
		driver->getTexture("media/skybox/glacier_bk.png"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	// add the Quake 3 map
	device->getFileSystem()->addFileArchive("media/map/map-20kdm2.pk3");

	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;

	// add octree
	if (mesh)
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);

	// because the level was not modelled around the origin (0,0,0), we
	// translate the whole level a little bit.
	if (node)
		node->setPosition(core::vector3df(-1300,-144,-1249));

	// create the sun node as a sphere scene node.
	scene::IMeshSceneNode* sunMeshNode = smgr->addSphereSceneNode(1, 16, smgr->getRootSceneNode());
	sunMeshNode->setMaterialTexture(0, driver->getTexture("media/sun/mesh.png"));
	sunMeshNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	sunMeshNode->setMaterialFlag(video::EMF_LIGHTING, false);
	sunMeshNode->setScale(core::vector3d<f32>(600, 600, 600));

	// add a billboard to the sun node
	scene::IBillboardSceneNode* sunBillboardNode = smgr->addBillboardSceneNode(sunMeshNode);
	sunBillboardNode->setMaterialTexture(0, driver->getTexture("media/sun/sun.png"));
	sunBillboardNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	sunBillboardNode->setMaterialFlag(video::EMF_LIGHTING, false);
	sunBillboardNode->setSize(core::dimension2d<f32>(4000, 4000));

	// add the lensflare to the sun
	scene::CLensFlareSceneNode* lensFlareNode = new scene::CLensFlareSceneNode(sunMeshNode, smgr);
	lensFlareNode->setMaterialTexture(0, driver->getTexture("media/lensflare/flare.png"));

	// create a follow camara animator for the sun
	scene::CSceneNodeAnimatorFollowCamera* sunAnim = new scene::CSceneNodeAnimatorFollowCamera(core::vector3df(-8000, 4000, 750));
	sunMeshNode->addAnimator(sunAnim);
	sunAnim->drop();

	// we need a camera to look at the Quake 3 map.
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	if (camera)
	{
		camera->setPosition(core::vector3df(-400,400,-900));
		camera->setFarValue(20000);
	}

	// add the occlusion query
	driver->addOcclusionQuery(sunMeshNode, sunMeshNode->getMesh());

	// The mouse cursor needs not be visible, so we hide it via the
	// irr::IrrlichtDevice::ICursorControl.
	device->getCursorControl()->setVisible(false);

	// We have done everything, so lets draw it.
	static u32 lastUpdate = 0;

	while(device->run())
	{
		if (device->isWindowActive())
		{
			// begin the scene
			driver->beginScene(true, true, video::SColor(255,0,0,0));

			// render the scene as usual
			smgr->drawAll();

			// run occlusion query
			driver->runAllOcclusionQueries(false);
			driver->updateAllOcclusionQueries(false);
			u32 occlusionQueryResult = driver->getOcclusionQueryResult(sunMeshNode);

			// update strength of the lensflare
			if(occlusionQueryResult!= 0xffffffff)
				lensFlareNode->setStrength(f32(occlusionQueryResult)/8000.f);

			// finish scene rendering as usual
			driver->endScene();

			// update Info every 1000 ticks
			u32 now = device->getTimer()->getRealTime();
			if (now > lastUpdate+1000)
			{
				int fps = driver->getFPS();
				core::stringw str = L"LensFlare example [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;
				str += " OccQuery:";
				str += occlusionQueryResult;
				device->setWindowCaption(str.c_str());

				//save the last query time
				lastUpdate = now;
			}
		}
		else
			device->yield();
	}

	// In the end, delete the Irrlicht device.
	device->drop();
	return 0;
}

// That's it. Compile and play around with the program.

