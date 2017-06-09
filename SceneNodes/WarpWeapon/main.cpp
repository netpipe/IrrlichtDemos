#include "stdafx.h"
#include <irrlicht.h>
#include <iostream>
#include "shader_warp.h"
using namespace irr;
using namespace irr::scene;

#pragma comment(lib, "Irrlicht.lib")

bool mouseLeftDown = false;

class MyEventReceiver : public IEventReceiver
{
public:

  virtual bool OnEvent(const SEvent& event)
  {
   if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
      if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
	  {
	    mouseLeftDown = true;
	  }
	  if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
	  {
	    mouseLeftDown = false;
	  }

	}

    return false;
  }
};


int main()
{
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;


	MyEventReceiver receiver;

	// create device
	IrrlichtDevice *device =
		createDevice(driverType, core::dimension2d<u32>(640, 480), 16, false, false, false,&receiver);

	if (device == 0)
		return 1; // could not create selected driver.


	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	ICameraSceneNode *cam = smgr->addCameraSceneNodeFPS(0,100,1000);


	smgr->addSkyBoxSceneNode(driver->getTexture("mars_fr.png"),
							 driver->getTexture("mars_bk.png"),
							 driver->getTexture("mars_rt.png"),
							 driver->getTexture("mars_dn.png"),
							 driver->getTexture("mars_up.png"),
							 driver->getTexture("mars_lf.png"));


	// create offscreen render target
	ITexture *tex_offscr = driver->addRenderTargetTexture(core::dimension2d<u32>(512,512));
    video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();


	// instantiate callback
	Shader_Warp_callback *callback = new Shader_Warp_callback();
	callback->device = device;
	callback->driver = driver;
	callback->smgr = smgr;

	// create material, based on shader
	s32 material_warp = gpu->addHighLevelShaderMaterial(
	  vertWarpShader.c_str(), "main", video::EVST_VS_2_a,
	  fragWarpShader.c_str(), "main", video::EPST_PS_2_b,
	  callback, video::EMT_TRANSPARENT_ALPHA_CHANNEL);


	// create billboard for bullet
	ISceneNode *node_warp = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(90, 90));
	node_warp->setMaterialFlag(video::EMF_LIGHTING, false);
	node_warp->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	node_warp->setMaterialTexture(0, tex_offscr);
	node_warp->setMaterialTexture(1, driver->getTexture("normalmap.bmp"));
	node_warp->setMaterialTexture(2, driver->getTexture("diffusemap.bmp"));
    node_warp->getMaterial(0).MaterialTypeParam = 0.01f;
	node_warp->setMaterialType((video::E_MATERIAL_TYPE) material_warp);




	int index_warptex = 0;
	float dx,dy,dz;
	u32 frames=0;
	while(device->run())
	{
		if (mouseLeftDown)
		{
		  float ang = (3.1434*cam->getRotation().Y)/180.0f;
		  float ang_updown = (3.1434*cam->getRotation().X)/180.0f;
	      dx = sinf(ang);
	      dy = -tanf(ang_updown);
	      dz = cosf(ang);
		  node_warp->setPosition(cam->getPosition());
		}

		static unsigned long tmr = device->getTimer()->getRealTime();
		if ( device->getTimer()->getRealTime() - tmr > 20)
		{
		  float amp = 3; // amplitude of direction vector (determines speed)
		  node_warp->setPosition(node_warp->getPosition() + vector3df(dx,dy,dz)*amp);
		  tmr =  device->getTimer()->getRealTime();
		}


		driver->beginScene(true, true, video::SColor(0,255,0,255));

		// render scene without billboard
		driver->setRenderTarget(tex_offscr, false, true, video::SColor(0,0,0,0));
	    node_warp->setVisible(false);
		smgr->drawAll();

		// render final scene
		driver->setRenderTarget(0, true, true, video::SColor(0,0,0,0));
		node_warp->setVisible(true);
		smgr->drawAll();

		driver->endScene();

		if (++frames==100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();
			str += " Click with mouse to release bullet...";

			device->setWindowCaption(str.c_str());
			frames=0;
		}
		device->sleep(15,0);
	}

	device->drop();

	return 0;
}
