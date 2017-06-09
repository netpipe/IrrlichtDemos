//#include "stdafx.h"
#include <irrlicht.h>
#include <iostream>
#include "shader_lightmap_specular.h"
#include "shader_glass.h"
#include <vector>

using namespace irr;
using namespace irr::scene;
#pragma comment(lib, "Irrlicht.lib")

int main()
{
    gui::IGUIFont* font;
	bool no_specular = false;
	video::E_DRIVER_TYPE driverType= video::EDT_OPENGL;

	// create device and exit if creation failed
	IrrlichtDevice *device =
		createDevice(driverType, core::dimension2d<s32>(640, 480),
		16, false, false);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	font = device->getGUIEnvironment()->getBuiltInFont();


    video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	// create offscreen render target
	ITexture *tex_offscr = driver->createRenderTargetTexture(core::dimension2d<s32>(256,256));


	// glass shader
	Shader_Glass_callback *cb_glass = new Shader_Glass_callback();
	cb_glass->smgr = smgr;
	cb_glass->device = device;
	cb_glass->driver = driver;
    s32 material_glass = gpu->addHighLevelShaderMaterial(
	 vertGlassShader.c_str(), "main", video::EVST_VS_2_a,
	 fragGlassShader.c_str(), "main", video::EPST_PS_2_b,
	 cb_glass, video::EMT_SOLID);



	// normalmap + specular shader
	ShaderLightmapSpecularCallback *cb = new ShaderLightmapSpecularCallback();
    cb->fSpecularStrength=0.6f;
    cb->fSpecularPower=10;
    cb->fvAmbient=SColorf(0.02,0.02,0.02);

    s32 material = gpu->addHighLevelShaderMaterial(
	 vertBumpShaderLS.c_str(), "main", video::EVST_VS_2_a,
	 fragBumpShaderLS.c_str(), "main", video::EPST_PS_2_b,
	 cb, video::EMT_SOLID);

	IAnimatedMesh*mesh = smgr->getMesh("map/test.b3d");


    std::vector<int>refr_submeshes;


	IMesh *m = mesh->getMesh(0);
    int refr_i = -1;
	for (int mb = 0; mb < m->getMeshBufferCount(); mb++)
	{
      IMeshBuffer *imb = m->getMeshBuffer(mb);
	  char sbuf[255];
	  strcpy(sbuf, imb->getMaterial().getTexture(0)->getName().c_str());
	  char C[2];
	  C[1] = NULL;
	  char pure_fn[255];
	  strcpy(pure_fn,"");
	  for (int c = 0; c < strlen(sbuf); c++)
	  {
	    C[0] = sbuf[c];
		if ((C[0] == '\\') || (C[0] == '/'))
		{
		  strcpy(pure_fn,"");
		  continue;
		}

		strcat(pure_fn,C);
	  }

	  if (!strcmp(pure_fn,"refraction.jpg"))
	  {
	    imb->getMaterial().MaterialType = (video::E_MATERIAL_TYPE)material_glass;
		imb->getMaterial().setFlag(video::EMF_LIGHTING, false);
		imb->getMaterial().setFlag(video::EMF_ZWRITE_ENABLE, false);
		imb->getMaterial().setTexture(1,tex_offscr);
		imb->getMaterial().setTexture(2,driver->getTexture("refr_diffuse.png"));
        refr_submeshes.push_back(mb);
		continue;
	  }


	  strcpy(sbuf,imb->getMaterial().getTexture(0)->getName().c_str());
	  imb->getMaterial().setFlag(video::EMF_LIGHTING , false);
	  if (no_specular) continue;
	  imb->getMaterial().MaterialType = (video::E_MATERIAL_TYPE)material;

	  char sbuf_n[255];
	  strcpy(sbuf_n,"");

	  C[1] = NULL;
	  int dotPos;
	  for (int n = 0; n < strlen(sbuf); n++)
	  {
	    if (sbuf[n] == '.') dotPos = n;
	  }
	  for (int n = 0; n < strlen(sbuf); n++)
	  {
		C[0] = sbuf[n];
	    if (n == dotPos)
		{
		  strcat(sbuf_n,"_n");
		}
        strcat(sbuf_n,C);
	  }
	  imb->getMaterial().setTexture(2, driver->getTexture(sbuf_n));
	}


	scene::IAnimatedMeshSceneNode* anode = 0;
	anode = smgr->addAnimatedMeshSceneNode(mesh);
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(core::vector3df(-50,50,-150));

    device->getCursorControl()->setVisible(false);	// disable mouse cursor


    while(device->run())
	if (device->isWindowActive())
	{
      IAnimatedMesh *iam = anode->getMesh();
	  for (int sm = 0; sm < refr_submeshes.size(); sm++)
	    iam->getMeshBuffer(refr_submeshes[sm])->getMaterial().setFlag(::video::EMF_POINTCLOUD, true);
	  anode->setMesh(iam);

	  driver->beginScene(true, true, 0);
  	  // render scene without glass
	  driver->setRenderTarget(tex_offscr, false, true, video::SColor(0,0,0,0));
	  smgr->drawAll();

	  // render final scene
  	  driver->setRenderTarget(0);
      iam = anode->getMesh();
	  for (int sm = 0; sm < refr_submeshes.size(); sm++)
	    iam->getMeshBuffer(refr_submeshes[sm])->getMaterial().setFlag(::video::EMF_POINTCLOUD, false);
	  anode->setMesh(iam);
	  smgr->drawAll();

	  int fps = driver->getFPS();
	  wchar_t *lbl = new wchar_t[255];
	  swprintf(lbl,255,L"fps: %i", fps);

  	  font->draw(lbl,
				core::rect<s32>(0,0,1024,50),
				video::SColor(255,255,255,255));

	  driver->endScene();
	}

	device->drop();

	return 0;
}

