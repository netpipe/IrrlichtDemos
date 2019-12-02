#include <irrlicht.h>
#include <iostream>

#include"irrAn8Loader.h" // Anim8or file loader

using namespace irr;

int main()
{
#ifdef WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Look for Memory leak !
	//_CrtSetBreakAlloc(2840); // To use to find the memory leak source
#endif


	video::E_DRIVER_TYPE driverType;

	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
		" (d) Software Renderer\n (e) Burning's Software Renderer\n"\
		" (f) NullDevice\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;

	switch(i)
	{
		case 'a': driverType = video::EDT_DIRECT3D9;break;
//		case 'b': driverType = video::EDT_DIRECT3D8;break;
		case 'c': driverType = video::EDT_OPENGL;   break;
		case 'd': driverType = video::EDT_SOFTWARE; break;
		case 'e': driverType = video::EDT_BURNINGSVIDEO;break;
		case 'f': driverType = video::EDT_NULL;     break;
		default: return 0;
	}

	// create device
	IrrlichtDevice *device = createDevice(driverType,
			core::dimension2d<u32>(640, 480), 16, false);

	if (device == 0)
		return 1; // could not create selected driver.

	// create engine and camera
	device->setWindowCaption(L"Irrlicht Anim8or File Loader");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	device->getCursorControl ()-> setVisible (true);
	//smgr->addCameraSceneNode(0, core::vector3df(0,-200,0), core::vector3df(0,0,0));
	scene::ICameraSceneNode* camera =
		smgr->addCameraSceneNodeFPS(
		0, // parent
		100.0f, //rotateSpeed
		.3f, //moveSpeed
		-1, 0, 0,
		true, // vertical mouvement
		3.f); // jumpspeed
	camera->setPosition(core::vector3df(-100,15,-150));

	// Add an8 loader
	scene::irrAn8Loader loaderAn8(smgr,device->getFileSystem());
	smgr->addExternalMeshLoader(&loaderAn8);

	scene::IAnimatedMesh* model = smgr->getMesh("test2.an8");
    scene::IAnimatedMeshSceneNode* Nmodele = smgr->addAnimatedMeshSceneNode(model);

	// Add a light
	scene::ILightSceneNode* nodeLight = smgr->addLightSceneNode(0, core::vector3df(100, 100, 100),
		video::SColorf(1.0f,1.0f,1.0f,1.0f),
		20000.0f);
	video::SLight light;
	//light.Direction = core::vector3df(100, 100, 0);
	//light.Type = video::ELT_DIRECTIONAL;
	light.AmbientColor = video::SColorf(0.3f,0.3f,0.3f,1);
	light.SpecularColor= video::SColorf(0.4f,0.4f,0.4f,1);
	light.DiffuseColor = video::SColorf(1.0f,1.0f,1.0f,1);
	light.CastShadows = false;
	nodeLight->setLightData(light);

	while(device->run())
	{
		driver->beginScene(true,true,video::SColor(0,200,200,255));
        smgr->drawAll ();
        driver->endScene ();
	}
	device->drop();
	return 0;
}


