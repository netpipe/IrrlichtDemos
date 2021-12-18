/* tutorials for snow.*/

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef __EMSCRIPTEN__
#include <emscripten.h>

#endif

#pragma comment(lib, "Irrlicht.lib")

scene::ICameraSceneNode* camera = 0;

	IrrlichtDevice *device;

	video::IVideoDriver* driver;
	scene::ISceneManager* smgr;


void main_loop(){

	device->run();
//	{
	driver->beginScene(true, true, video::SColor(255,200,200,200));
		smgr->drawAll();

		driver->endScene();

		int fps = driver->getFPS();
		wchar_t tmp[1024];
		swprintf(tmp, 1024, L"SpecialFX example - Irrlicht Engine"\
			L"(fps:%d) Triangles:%d", fps,
			driver->getPrimitiveCountDrawn());
		device->setWindowCaption(tmp);
//		lastFPS = fps;
		device->sleep(8,0);
	//}

	}

int main()
{
#ifdef __EMSCRIPTEN__

	device = createDevice(video::EDT_OGLES2,
		core::dimension2d<u32>(640, 480),
		16, false, true );
#else

	device = createDevice(video::EDT_OPENGL,
		core::dimension2d<u32>(640, 480),
		16, false, true );


#endif
	 driver = device->getVideoDriver();
	 smgr = device->getSceneManager();


	scene::ILightSceneNode * lightNode  = smgr->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(1.0f, 0.6f, 1.0f, 0.0f), 800.0f);
		lightNode->setPosition(vector3df(3,50,2));

	scene::ISceneNode* node = 0;

	scene::IAnimatedMesh* mesh = smgr->addHillPlaneMesh("Hill",
		core::dimension2d<f32>(20,20),
		core::dimension2d<u32>(20,20), 0, 20,
		core::dimension2d<f32>(2,2),
		core::dimension2d<f32>(1,1));

	node = smgr->addAnimatedMeshSceneNode(mesh);
	node->setPosition(core::vector3df(0,7,0));

	node->setMaterialTexture(0,	driver->getTexture("media/snow.bmp"));

	node->setMaterialFlag(video::EMF_LIGHTING, true);



	//try snow
	scene::IParticleSystemSceneNode* ps = 0;
	ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition(core::vector3df(-160,170,80));
	ps->setScale(core::vector3df(2,2,2));

	ps->setParticleSize(core::dimension2d<f32>(5.0f, 5.0f));

	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-100,-200,-270,270,300,300),
		core::vector3df(0.01f,-0.02f,0.0f),
		400,800,
		video::SColor(0,255,255,255), video::SColor(0,255,255,255),
		1800,3000);

	ps->setEmitter(em);
	em->drop();

	scene::IParticleAffector* paf =
		ps->createGravityAffector(core::vector3df(-0.05f,-0.03f, 0.0f), 2000);

	ps->addAffector(paf);
	paf->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, true);
	ps->setMaterialTexture(0, driver->getTexture("media/snowparticle.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);


//	mesh = smgr->getMesh("media/snowman_final.3ds");
mesh = smgr->getMesh("media/snowman.3ds");
	scene::IAnimatedMeshSceneNode* anode = 0;

	anode = smgr->addAnimatedMeshSceneNode(mesh);
	anode->setPosition(core::vector3df(-50,45,-60));
	anode->setScale(core::vector3df(-35,35,-36));
		anode->setScale(core::vector3df(-75,75,-76));
		anode->setRotation(vector3df(0,180,0));
//	anode->setMaterialTexture(0, driver->getTexture("media/textura_01.png"));
	anode->setMaterialFlag(video::EMF_LIGHTING, false);


mesh = smgr->getMesh("media/IrrlichtLogo.3ds");
	scene::IAnimatedMeshSceneNode* anodes = 0;

	anodes = smgr->addAnimatedMeshSceneNode(mesh);
	anodes->setPosition(core::vector3df(-50,130,-60));
	anodes->setScale(core::vector3df(-25,25,-26));
		//anodes->setScale(core::vector3df(-75,75,-76));
		anodes->setRotation(vector3df(-90,0,180));
	anodes->setMaterialTexture(0, driver->getTexture("media/IrrlichtLogoTexture.png"));
	anodes->setMaterialFlag(video::EMF_LIGHTING, false);

mesh = smgr->getMesh("media/merrychristmas.3ds");
	scene::IAnimatedMeshSceneNode* anodess = 0;

	anodess = smgr->addAnimatedMeshSceneNode(mesh);
	anodess->setPosition(core::vector3df(0,100,-111));
	anodess->setScale(core::vector3df(-15,15,-16));
		//anodes->setScale(core::vector3df(-75,75,-76));
		anodess->setRotation(vector3df(-90,0,180));
	//anodess->setMaterialTexture(0, driver->getTexture("media/IrrlichtLogoTexture.png"));
	anodess->setMaterialFlag(video::EMF_LIGHTING, false);


	mesh = smgr->getMesh("media/logs.3ds");
	scene::IAnimatedMeshSceneNode* logs = 0;

	logs = smgr->addAnimatedMeshSceneNode(mesh);
	logs->setPosition(core::vector3df(0,10,-100));
	logs->setScale(core::vector3df(-5,5,-5));
		//anodes->setScale(core::vector3df(-75,75,-76));
		logs->setRotation(vector3df(-90,0,180));
	//logs->setMaterialTexture(0, driver->getTexture("media/IrrlichtLogoTexture.png"));
	logs->setMaterialFlag(video::EMF_LIGHTING, true);


	// create sky box
	scene::ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
		driver->getTexture("media/mountain.jpg"),
		driver->getTexture("media/mountain.jpg"),
		driver->getTexture("media/mountainleft.jpg"),
		driver->getTexture("media/mountainright.jpg"),
		driver->getTexture("media/mountainmid.jpg"),
		driver->getTexture("media/mountain.jpg"));

	//camera=smgr->addCameraSceneNode(0, vector3df(-50, 80, -200),vector3df(0, 0, 250));
	camera=smgr->addCameraSceneNodeFPS();


	// create a particle system

	scene::IParticleSystemSceneNode* ps2 =
		smgr->addParticleSystemSceneNode(false);

	if (ps2)
	{
		scene::IParticleEmitter* em = ps2->createBoxEmitter(
			core::aabbox3d<f32>(-7,0,-7,7,1,7), // emitter size
			core::vector3df(0.01f,0.06f,0.0f),   // initial direction
			80,100,                             // emit rate
			video::SColor(0,255,255,255),       // darkest color
			video::SColor(0,255,255,255),       // brightest color
			800,2000,0,                         // min and max age, angle
			core::dimension2df(10.f,10.f),         // min size
			core::dimension2df(20.f,20.f));        // max size

		ps2->setEmitter(em); // this grabs the emitter
		em->drop(); // so we can drop it here without deleting it

		scene::IParticleAffector* paf = ps2->createFadeOutParticleAffector();

		ps2->addAffector(paf); // same goes for the affector
		paf->drop();

		ps2->setPosition(core::vector3df(-20,0,-100));
		ps2->setScale(core::vector3df(2,2,2));
		ps2->setMaterialFlag(video::EMF_LIGHTING, false);
		ps2->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
		ps2->setMaterialTexture(0, driver->getTexture("./media/fire.bmp"));
		ps2->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	}

	/*
	Next we add a volumetric light node, which adds a glowing fake area light to
	the scene. Like with the billboards and particle systems we also assign a
	texture for the desired effect, though this time we'll use a texture animator
	to create the illusion of a magical glowing area effect.
	*/
//	scene::IVolumeLightSceneNode * n = smgr->addVolumeLightSceneNode(0, -1,
//				32,                              // Subdivisions on U axis
//				32,                              // Subdivisions on V axis
//				video::SColor(0, 255, 255, 255), // foot color
//				video::SColor(0, 0, 0, 0));      // tail color

//	scene::ILightSceneNode * lightNode  = smgr->addLightSceneNode(0, core::vector3df(0,0,0),
//		video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
	//n->setPosition(core::vector3df(-20,50,-100));

	int lastFPS = -1;
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
	while(device->run())
	{
driver->beginScene(true, true, video::SColor(255,200,200,200));
		smgr->drawAll();

		driver->endScene();

		int fps = driver->getFPS();
		wchar_t tmp[1024];
		swprintf(tmp, 1024, L"SpecialFX example - Irrlicht Engine"\
			L"(fps:%d) Triangles:%d", fps,
			driver->getPrimitiveCountDrawn());
		device->setWindowCaption(tmp);
		lastFPS = fps;
		device->sleep(8,0);
	}
#endif
	device->drop();

	return 0;
}


