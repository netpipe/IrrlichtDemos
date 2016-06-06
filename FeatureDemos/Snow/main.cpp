/* tutorials for snow.*/

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#pragma comment(lib, "Irrlicht.lib")

scene::ICameraSceneNode* camera = 0;


int main()
{

	IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
		core::dimension2d<s32>(640, 480),
		16, false, true );

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	scene::ISceneNode* node = 0;

	scene::IAnimatedMesh* mesh = smgr->addHillPlaneMesh("Hill",
		core::dimension2d<f32>(20,20),
		core::dimension2d<u32>(20,20), 0, 20,
		core::dimension2d<f32>(2,2),
		core::dimension2d<f32>(1,1));

	node = smgr->addAnimatedMeshSceneNode(mesh);
	node->setPosition(core::vector3df(0,7,0));

	node->setMaterialTexture(0,	driver->getTexture("media/snow.bmp"));

	node->setMaterialFlag(video::EMF_LIGHTING, false);


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

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, driver->getTexture("media/snowparticle.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);


	mesh = smgr->getMesh("media/snowman.3ds");
	scene::IAnimatedMeshSceneNode* anode = 0;

	anode = smgr->addAnimatedMeshSceneNode(mesh);
	anode->setPosition(core::vector3df(-50,45,-60));
	anode->setMaterialTexture(0, driver->getTexture("media/snowman.jpg"));
	anode->setMaterialFlag(video::EMF_LIGHTING, false);


	// create sky box
	scene::ISceneNode* skyboxNode = smgr->addSkyBoxSceneNode(
		driver->getTexture("media/mountain.jpg"),
		driver->getTexture("media/mountain.jpg"),
		driver->getTexture("media/mountainleft.jpg"),
		driver->getTexture("media/mountainright.jpg"),
		driver->getTexture("media/mountainmid.jpg"),
		driver->getTexture("media/mountain.jpg"));

	camera=smgr->addCameraSceneNode(0, vector3df(-50, 80, -200),vector3df(0, 0, 250));


	int lastFPS = -1;

	while(device->run())
	{
		driver->beginScene(true, true, 0);

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

	device->drop();

	return 0;
}

