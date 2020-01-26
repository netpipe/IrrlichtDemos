#ifndef __CORE_H_INCLUDED__
#define __CORE_H_INCLUDED__


#include <irrlicht.h>

#include <cstdlib>
#include <cstdio>
#include <string>
#include <ctime>

#include "xeffects/Source/XEffects.h"

#include "corereciever.h"
#include "coreshadercallback.h"

#include "coreshadowlight.h"

#include "config.h"

#include "vehicle.h"

class Core
{
public:

	void begin(const char * winName);

	void end(void);

	bool run(void);

	bool active(void);

	void yield(void);

	void update(void);

	void render(void);

	Core() { }
	~Core() { }

private:

	void build_keyboard(int x, int y, float s);

	void init_device(const char * winName);

	void load_shaders(void);

	void load_lights(void);

	void load_cameras(void);

	CShadowLightSceneNode * addShadowLightSceneNode(irr::scene::ISceneNode * parent, irr::u32 mapRes,
			const irr::core::vector3df& position, irr::video::SColorf color, irr::f32 near, irr::f32 far, irr::s32 id);

	irr::scene::IAnimatedMesh * getMeshIrrlicht(const irr::io::path& filename);

	Vehicle * vehicle;

	irr::core::stringc win_name;

	irr::IrrlichtDevice * device;
	irr::video::IVideoDriver * driver;
	irr::scene::ISceneManager * scene_manager;
	irr::gui::IGUIEnvironment * gui_env;
	CoreEventReciever * event_reciever;

	EffectHandler * effect_handler;

	irr::scene::ICameraSceneNode * camera;

	irr::s32 shader;
	irr::io::path vshader_file;
	irr::io::path fshader_file;
};

void Core::begin(const char * winName)
{
	this->init_device(winName);

	this->load_shaders();

	this->load_lights();

	this->load_cameras();

	this->vehicle = new Vehicle(this->effect_handler, this->device, this->shader);

	this->scene_manager->setShadowColor(irr::video::SColor(150,0,0,0));

	const irr::scene::IGeometryCreator * creator = this->scene_manager->getGeometryCreator();
	//irr::scene::ISceneNode * plane = this->scene_manager->addMeshSceneNode(creator->createPlaneMesh(irr::core::dimension2d<irr::f32>(10.0f, 10.0f)));
	//plane->setMaterialType((irr::video::E_MATERIAL_TYPE)this->shader);

	//this->effect_handler->addShadowToNode(plane, EFT_16PCF, ESM_RECEIVE);
}

void Core::end(void)
{
	if(this->event_reciever)
		this->event_reciever->drop();
	if(this->vehicle)
		this->vehicle->drop();
	if(this->device)
		this->device->drop();
	if(this->effect_handler)
		delete this->effect_handler;
}

bool Core::run(void)
{
	return this->device->run();
}

bool Core::active(void)
{
	return this->device->isWindowActive();
}

void Core::yield(void)
{
	this->device->yield();
}

void Core::update(void)
{
	this->device->setWindowCaption(irr::core::stringw(this->win_name + "  FPS: " + irr::core::stringc(this->driver->getFPS())).c_str());

	irr::core::stringw FPS = irr::core::stringw(this->driver->getFPS());
	this->gui_env->addStaticText(FPS.c_str(), irr::core::rect<irr::s32>(10, 10, FPS.size() * 20, 30), true);

	if(this->event_reciever->keyDown(irr::KEY_ESCAPE)) {
		this->device->closeDevice();
	}
	device->sleep(80);
}

void Core::render(void)
{
	this->driver->beginScene(true, true, irr::video::SColor(255, 5, 5, 250));

	this->effect_handler->update();

	this->gui_env->drawAll();

	this->driver->endScene();
}

void Core::init_device(const char * winName)
{
	irr::SIrrlichtCreationParameters params;
	params.AntiAlias = CORE_ANTI_ALIAS;
	params.DriverType  = irr::video::EDT_OPENGL;
	params.DeviceType = irr::EIDT_BEST;
	params.WindowSize = irr::core::dimension2d<irr::u32>(CORE_WINDOW_WIDTH, CORE_WINDOW_HEIGHT);
	params.Stencilbuffer = CORE_STENCIL_BUFFER;
	params.Fullscreen = 0;//CORE_FULLSCREEN;
	params.Vsync = CORE_VSYNC;

	this->device = irr::createDeviceEx(params);
	if(!device) {
		std::printf("Could not create device!\n");
		return;
	}

	this->device->setWindowCaption(irr::core::stringw(winName).c_str());
	this->win_name = winName;

	this->driver = device->getVideoDriver();
	this->scene_manager = device->getSceneManager();
	this->gui_env = device->getGUIEnvironment();

	this->event_reciever = new CoreEventReciever(this->device);

	this->device->setEventReceiver(this->event_reciever);

	this->effect_handler = new EffectHandler(this->device, this->driver->getScreenSize(), false, false, true);

	this->effect_handler->setClearColour(irr::video::SColor(255, 100, 100, 100));
	this->effect_handler->setAmbientColor(irr::video::SColor(255, 10, 10, 10));
	    device->getFileSystem()->addFileArchive("./egyptians.pk3");
	     device->getFileSystem()->addFileArchive("./media.zip");
}

void Core::load_shaders(void)
{
	this->vshader_file = "shader.vs";
	this->fshader_file = "shader.frag";

	if (!this->driver->queryFeature(irr::video::EVDF_PIXEL_SHADER_1_1) &&
            !this->driver->queryFeature(irr::video::EVDF_ARB_FRAGMENT_PROGRAM_1))
        {
        	this->device->getLogger()->log("WARNING: Pixel shaders disabled "\
           	 "because of missing driver/hardware support.");
        	fshader_file = "";
        }

        if (!this->driver->queryFeature(irr::video::EVDF_VERTEX_SHADER_1_1) &&
            !this->driver->queryFeature(irr::video::EVDF_ARB_VERTEX_PROGRAM_1))
    	{
        	this->device->getLogger()->log("WARNING: Vertex shaders disabled "\
           	 "because of missing driver/hardware support.");
        	vshader_file = "";
        }

	if (!this->driver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
	{
		this->device->getLogger()->log("WARNING: Render to texture disabled "\
		 "because of missing driver/harware support.");
	}

	irr::video::IGPUProgrammingServices * gpu = this->driver->getGPUProgrammingServices();

	this->shader = 0;

	CoreShaderCallback * scallback = new CoreShaderCallback(this->device);

	this->shader = gpu->addHighLevelShaderMaterialFromFiles(
			vshader_file, "main", irr::video::EVST_VS_1_1,
			fshader_file, "main", irr::video::EPST_PS_1_1,
			scallback, irr::video::EMT_SOLID);

	if(this->shader < 0) {
		std::printf("Could not add shader!\n");
		this->device->closeDevice();
	}

	scallback->drop();
}

void Core::load_lights(void)
{
	irr::scene::ILightSceneNode * light0 = this->addShadowLightSceneNode(0, 512, irr::core::vector3df(0, -3, 0), irr::video::SColorf(1.0f, 1.0f, 1.0f), 0.3f, 10.0f, -1);
	if(light0) {
		light0->getLightData().AmbientColor = irr::video::SColorf(0.4f, 0.4f, 0.4f);
		light0->getLightData().SpecularColor = irr::video::SColorf(1.0f, 1.0f, 1.0f);
		//light0->setLightType(irr::video::ELT_DIRECTIONAL);
		//light0->getLightData().Direction = irr::core::vector3df(1.0f, -1.0f, 1.0f);
		light0->setName("light0");

		irr::scene::ISceneNodeAnimator * lightAnimator = this->scene_manager->createFlyCircleAnimator(irr::core::vector3df(0, 1, 0), 1, 0.008);
		if(lightAnimator) {
			light0->addAnimator(lightAnimator);
			lightAnimator->drop();
		}
	}

	//this->scene_manager->setAmbientLight(irr::video::SColor(200, 20, 20, 20));
}

CShadowLightSceneNode * Core::addShadowLightSceneNode(irr::scene::ISceneNode * parent, irr::u32 mapRes,
			const irr::core::vector3df& position, irr::video::SColorf color, irr::f32 near, irr::f32 far, irr::s32 id)
{
	if(!parent)
		parent = this->scene_manager->getRootSceneNode();

	CShadowLightSceneNode * node = new CShadowLightSceneNode(this->effect_handler, mapRes, parent, this->scene_manager,
								id, position, color, near, far);
	node->drop();

	return node;
}

void Core::load_cameras(void)
{
	this->camera = this->scene_manager->addCameraSceneNodeFPS(0, 100.0f, 0.005f);
	if(this->camera) {
		this->camera->setFarValue(50000.0f);
		this->camera->setNearValue(0.5f);
		this->camera->setFOV(1.0f);
		this->camera->setPosition(irr::core::vector3df(10.0f, 10.0f, 10.0f));
		this->camera->setTarget(irr::core::vector3df(0, 0, 0));
	}

	this->device->getCursorControl()->setVisible(false);
}

irr::scene::IAnimatedMesh * Core::getMeshIrrlicht(const irr::io::path& filename)
{
	irr::scene::IAnimatedMesh * mesh = this->scene_manager->getMesh(filename);
	if(!mesh) {
		std::printf("Could not load mesh with Irrlicht!");
	}

	return mesh;
}

#endif
