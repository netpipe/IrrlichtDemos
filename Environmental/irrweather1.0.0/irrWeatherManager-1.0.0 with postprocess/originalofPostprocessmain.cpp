// based on Example 002 Quake3Map

#include <irr/irrlicht.h>
#include <iostream>

#include "PostProcessManager.h"
#include "ShaderPostProcess.h"

using namespace irr;

// this variable is used to switch through the effects in mode 1
E_POSTPROCESS_EFFECT sampleEffect = EPPE_NO_EFFECT;

// mode
// 0 = no postprocess
// 1 = use sample effect
// 2 = use combination of effects
int postProcessMode = 0;

class MyEventReceiver : public IEventReceiver
{
public:
	bool OnEvent(const SEvent& event)
	{
		// check if user presses the key 'p'
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_KEY_P:
				// switch to the next postprocess effect
				sampleEffect = (E_POSTPROCESS_EFFECT) ((sampleEffect+1) % EPPE_COUNT);
				return true;
			case irr::KEY_KEY_M:
				// switch to the next postprocess mode
				postProcessMode = ((postProcessMode+1) % 3);
				return true;
			case irr::KEY_KEY_S:
				// switch to the next postprocess mode
				if (postProcessMode)
				postProcessMode = ((postProcessMode+1) % 3);
				return true;
			default:
				break;
			}
		}

		return false;
	}
};

int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	// create device and exit if creation failed
	IrrlichtDevice *device = createDevice(driverType, core::dimension2d<u32>(1024, 768));
	if (device == 0)
		return 1;

	// Get a pointer to the video driver and the SceneManager so that
	// we do not always have to call irr::IrrlichtDevice::getVideoDriver() and
	// irr::IrrlichtDevice::getSceneManager().
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// create the postprocessing framework
	CPostProcessManager* postProcessManager= new CPostProcessManager(device);
	if (postProcessManager == 0)
		return 1;

	// add the Quake 3 map
	device->getFileSystem()->addFileArchive("../../media/postprocess/media/map/chiropteraDM.pk3");

	scene::IAnimatedMesh* mesh = smgr->getMesh("chiropteraDM.bsp");
	scene::ISceneNode* node = 0;

	// create sky box
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	smgr->addSkyBoxSceneNode(
		driver->getTexture("../../media/postprocess/media/skybox/glacier_up.png"),
		driver->getTexture("../../media/postprocess/media/skybox/glacier_dn.png"),
		driver->getTexture("../../media/postprocess/media/skybox/glacier_lf.png"),
		driver->getTexture("../../media/postprocess/media/skybox/glacier_rt.png"),
		driver->getTexture("../../media/postprocess/media/skybox/glacier_ft.png"),
		driver->getTexture("../../media/postprocess/media/skybox/glacier_bk.png"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	if (mesh)
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);

	// Because the level was not modelled around the origin (0,0,0), we
	// translate the whole level a little bit.
	if (node)
		node->setPosition(core::vector3df(-1300,-144,-1249));

	// add the quake node to the depth pass (used for the depth of field effect)
	if(node)
		postProcessManager->addNodeToDepthPass(node);

	// we need a camera to look at the Quake 3 map.
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	if (camera)
	{
		camera->setPosition(core::vector3df(-400,400,-900));
		camera->setFarValue(20000);
	}

	// The mouse cursor needs not be visible, so we hide it via the
	// irr::IrrlichtDevice::ICursorControl.
	device->getCursorControl()->setVisible(false);

	// create event receiver
	MyEventReceiver receiver;
	device->setEventReceiver(&receiver);

	// We have done everything, so lets draw it.
	u32 lastUpdate = 0;

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255,200,200,200));


			// prepare scene for post processing
			if (postProcessMode)
			{
				// the depth is stored in the alpha channel, normals are stored in rgb values
				// call renderDepth() if depth or normal information is used
				// (for example by the depth of field effect)
				postProcessManager->renderDepth();

				// prepare the scene for postprocessing
				// (render to rtt 'auxOut')
				postProcessManager->prepare();
			}

			// render the scene as usual
			smgr->drawAll();

			// now render the post process effect chains
			// in mode 1 apply the sample effect
			if (postProcessMode == 1)
			{
				// set some vars needed for the underwater shader
				video::SColorf waterColor = video::SColorf(0.05f, 0.05f, 0.2f);
				f32 blendFactor = 0.4f;
				if(sampleEffect == EPPE_UNDER_WATER)
				{
					// set the underwater vars
					CShaderPostProcess* underWaterPass = (CShaderPostProcess*) postProcessManager->getEffectChain(EPPE_UNDER_WATER)[0];
					underWaterPass->getShaderMaterial()->setPixelShaderConstant("WaterColor", &waterColor.r, 4);
					underWaterPass->getShaderMaterial()->setPixelShaderConstant("BlendFactor", &blendFactor, 1);
				}
				postProcessManager->render(sampleEffect);
			}

			// in mode 2 apply 3 cascading effect chains
			if (postProcessMode == 2)
			{
				postProcessManager->render(EPPE_ADAPTIVE_BLOOM);
				postProcessManager->render(EPPE_DEPTH_OF_FIELD);
				postProcessManager->render(EPPE_VIGNETTE);
			}

			// finalize post processing
			// (render to the framebuffer)
			if (postProcessMode)
				postProcessManager->update();

			// render the scene as usual
			driver->endScene();

			u32 now = device->getTimer()->getRealTime();

			// update Info every 500 ticks
			if (now > lastUpdate+500)
			{
				s32 fps = driver->getFPS();
				core::stringw str = L"PostProcess example [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;
				str += " Mode:";
				str += core::stringw(postProcessMode);
				if(postProcessMode==1)
				{
					str += " Effect: ";
					str += postProcessManager->getEffectChain(sampleEffect).getName();
				}
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

