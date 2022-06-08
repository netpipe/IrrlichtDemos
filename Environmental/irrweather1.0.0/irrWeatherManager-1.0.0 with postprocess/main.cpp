// based on Example 002 Quake3Map

#include <irr/irrlicht.h>

#include "PostProcessManager.h"
#include "ShaderPostProcess.h"

#include "irrWeatherManager.h"

// That's it. Compile and play around with the program.

using namespace irr;

IrrlichtDevice* device = 0;
scene::ISceneManager* smgr;

class MyEventReceiver : public IEventReceiver
{
public:

virtual bool OnEvent(const SEvent& event)
{
    if (event.EventType == irr::EET_KEY_INPUT_EVENT&& event.KeyInput.PressedDown)
    {
        switch(event.KeyInput.Key)
        {
            case KEY_ESCAPE:{device->closeDevice();}

			default : break;
        }
    }

    return false;
}
};

int main()
{
    MyEventReceiver receiver;
	device = createDevice(video::EDT_OPENGL, core::dimension2du(800, 600),
		32, false, true, false, &receiver);

	device->setEventReceiver(&receiver);
	video::IVideoDriver* driver = device->getVideoDriver();

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	smgr = device->getSceneManager();

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.f, 1.f);
	camera->setPosition(core::vector3df(1280.f, 256.f, 1280.f));
	camera->setTarget(core::vector3df(2560.f, 256.f, 1280.f));

    scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("../../Data/terrain-heightmap.bmp");

	terrain->setScale(core::vector3df(10.f, 1.0f, 10.f));
	terrain->setMaterialFlag(video::EMF_LIGHTING, true);
    terrain->setPosition(core::vector3df(0,0,0));
	terrain->setMaterialTexture(0, driver->getTexture("../../Data/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("../../Data/detailmap3.jpg"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 5.0f);

	device->getCursorControl()->setVisible(false);

    // create the postprocessing framework
	CPostProcessManager* postProcessManager = new CPostProcessManager(device);
	if (postProcessManager == 0)
		return 1;

    irrWeatherManager* atmo = new irrWeatherManager(device, postProcessManager);

	// We have done everything, so lets draw it.
	u32 lastUpdate = 0;

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255, 0, 0, 0));

			atmo->updateWeather();

			E_POSTPROCESS_EFFECT sampleEffect = atmo->getEPPE();

            // the depth is stored in the alpha channel, normals are stored in rgb values
            // call renderDepth() if depth or normal information is used
            // (for example by the depth of field effect)
            postProcessManager->renderDepth();

            // prepare the scene for postprocessing
            // (render to rtt 'auxOut')
            postProcessManager->prepare();

			// render the scene as usual
			smgr->drawAll();

			// now render the post process effect chains
			// in mode 1 apply the sample effect

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

			// finalize post processing
			// (render to the framebuffer)
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
                str += " Effect: ";
                str += postProcessManager->getEffectChain(sampleEffect).getName();
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
