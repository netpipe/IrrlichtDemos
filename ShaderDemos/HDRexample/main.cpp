#include <iostream>
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace gui;

#pragma comment (lib, "Irrlicht.lib")

f32 Exposure;
f32 Gamma;

class HDRShaderCallBack : public IShaderConstantSetCallBack
{
	public:
		virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData)
		{
			float exp = Exposure;
			services->setPixelShaderConstant("Exposure",&exp,1);
			services->setPixelShaderConstant("Gamma",&Gamma,1);

			float var = 0;
			services->setPixelShaderConstant("Texture",&var,1);
		}
};

class Receiver : public IEventReceiver
{
	public:

		virtual bool OnEvent(const SEvent &event)
		{
			if (event.EventType == EET_GUI_EVENT)
			{
				IGUIElement* element = event.GUIEvent.Caller;
				s32 id = element->getID();

				if (id == 10)
					Exposure = ((IGUIScrollBar*)element)->getPos()/100.0f;
				else if (id == 11)
					Gamma = ((IGUIScrollBar*)element)->getPos()/100.0f;
			}
			return false;
		}
};

int main ()
{
	video::E_DRIVER_TYPE driverType;

	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) OpenGL 1.5\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;

	switch(i)
	{
		// D3D not yet implemented
		//case 'a': driverType = video::EDT_DIRECT3D9;break;
		case 'b': driverType = video::EDT_OPENGL;   break;
		default: return 1;
	}

	IrrlichtDevice* device = createDevice(driverType, core::dimension2du(800,600),32);

	IEventReceiver* receiver = new Receiver();
	device->setEventReceiver(receiver);

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guiEnv = device->getGUIEnvironment();

	IGUIWindow* window = guiEnv->addWindow(rect<s32>(5,5,230,60));
	IGUIScrollBar* expBar = guiEnv->addScrollBar(true,rect<s32>(5,25,220,35),window,10);
	IGUIScrollBar* gamBar = guiEnv->addScrollBar(true,rect<s32>(5,40,220,50),window,11);

	// the value will be devided by 100
	expBar->setMin(0);
	expBar->setMax(3000);
	expBar->setPos(100);
	gamBar->setMin(0);
	gamBar->setMax(1200);
	gamBar->setPos(200);

	c8* vertexShaderFile = 0;
	c8* pixelShaderFile = 0;

	if (driverType == EDT_OPENGL)
	{
		vertexShaderFile = "HDR.vert";
		pixelShaderFile = "HDR.frag";
	}

	// create materials
	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	s32 HDRMaterial = 0;

	if (gpu)
	{
		HDRShaderCallBack* shaderCB = new HDRShaderCallBack();

		if (shaderCB)
		{
			HDRMaterial = gpu->addHighLevelShaderMaterialFromFiles(
				vertexShaderFile,"main",EVST_VS_2_0,
				pixelShaderFile,"main",EPST_PS_2_0, shaderCB);

			shaderCB->drop();
		}
	}

	Exposure = 1.0f;
	Gamma = 2.0f;

	// Don't know about license for the picture
	// Find here : http://www.anyhere.com/gward/hdrenc/pages/originals.html
	ITexture* skyTexture = driver->getTexture("./SpheronNice.hdr");

	ISceneNode* skyDome = smgr->addSkyDomeSceneNode(skyTexture,64,32,0.9f,1.20f);
	skyDome->setMaterialTexture(0,skyTexture);
	skyDome->setMaterialType((E_MATERIAL_TYPE)HDRMaterial);

	smgr->addCameraSceneNodeMaya();

	ISceneNode* floor = smgr->addCubeSceneNode(1000);
	floor->setScale(vector3df(1.0f,0.0001f,1.0f));
	floor->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	//floor->setMaterialFlag(EMF_LIGHTING,false);

	stringw caption;

	while (device->run()) if (device->isWindowActive())
	{
		caption = "HDR Example - Exposure :";
		caption += Exposure;
		caption += " Gamma :";
		caption += Gamma;

		device->setWindowCaption(caption.c_str());

		driver->beginScene(true,true,SColor(255,120,120,120));

		smgr->drawAll();
		guiEnv->drawAll();

		driver->endScene();
	}

	device->closeDevice();
	device->drop();

	return 0;
}
