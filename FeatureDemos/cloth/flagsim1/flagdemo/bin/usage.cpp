// how it maybe used.......
// all setting as per the demo

// only running opengl and assuming check for suitable shader features
// load the shaders

// add your path stuff here



	#include <irrlicht.h>

	using namespace irr;

	using namespace core;
	using namespace scene;
	using namespace video;
	using namespace io;
	using namespace gui;

	IVideoDriver	*driver = 0;
	ISceneManager	*smgr	= 0;
	IGUIEnvironment	*guienv = 0;
	IrrlichtDevice	*device = 0;


// flag scene node
	#include "CFlagSceneNode.h"
	CFlagSceneNode	*irrFlagNode = 0;

// Generic Input Event Reciever from Tutorial 04
	class MyEventReceiver : public IEventReceiver
	{
	public:
		virtual bool OnEvent(const SEvent& event)
		{
			if (event.EventType == irr::EET_KEY_INPUT_EVENT) KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
			return false;
		}
		virtual bool IsKeyDown(EKEY_CODE keyCode) const{ return KeyIsDown[keyCode]; }
		MyEventReceiver(){ for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i) KeyIsDown[i] = false; }

	private:
		bool KeyIsDown[KEY_KEY_CODES_COUNT];
	};


int main()
{
	// create an Input event Reciever
		MyEventReceiver 	receiver;

	// create irrlicht device
		device = createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600), 32, false, false, false, &receiver);
		if (!device)return 1;

		device->setWindowCaption(L"Flag SceneNode Demo by FMX");

		driver = device->getVideoDriver();
		smgr = device->getSceneManager();
		guienv = device->getGUIEnvironment();

	// create a camera
    smgr->addCameraSceneNodeFPS();

io::path vsFileName = "flag_shader.vert";
io::path psFileName = "flag_shader.frag";

IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
int materialType = 0;
FlagShaderCallBack* cb = new FlagShaderCallBack();
if(gpu)
{
// add your own light settings here
	cb->setAmbient(SColorf(0.1f,0.1f,0.1f,1.0f));
	cb->setDiffuse(SColorf(1.0f,1.0f,1.0f,1.0f));
	cb->setTranslucency(SColorf(0.0f,0.0f,0.0f,0.0f));
	cb->setLight(0);	// use the first light
	cb->setSampler(0);

	materialType = gpu->addHighLevelShaderMaterialFromFiles(vsFileName, "main", EVST_VS_1_1,
											psFileName, "main", EPST_PS_1_1,cb, EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
	cb->drop();
}

// create a couple of perlin generators
Perlin1D* pgen1 = new Perlin1D(128,5,0.05f,2.0f,rand());
Perlin1D* pgen2 = new Perlin1D(128,6,0.04f,2.0f,rand());

// create a wind generator, there maybe issues/bugs with the direction handling
CWindGen* wind = new CWindGen(vector3df(1.0f,0.0f,0.0f),30.0,pgen1,0.5f,1.5f,pgen2);

// create the flag
CFlagSceneNode* flag = new CFlagSceneNode(smgr->getRootSceneNode(),smgr,345,wind,15.0f,16,12,5.0f,0.99f,0.05f,vector3df(0.0f,-0.06f,0.0f));flag->getMaterial(0).setTexture(0, driver->getTexture("usa.jpg"));
//flag->getMaterial(0).setTexture(0, driver->getTexture("japan.tga"));
//flag->getMaterial(0).setTexture(0, driver->getTexture("uk.tga"));

flag->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)materialType;
flag->setPosition(vector3df(0.0f,0.0f,0.0f));
smgr->isCulled(flag);//)AutomaticCulling(EAC_BOX);

	// render loop
	while(device->run())
	{
        driver->beginScene(true, true, SColor(255,100,101,140));
        smgr->drawAll();
        guienv->drawAll();
		driver->endScene();
		device->sleep(1,0);
	}

	flag->drop();	// remove flag scenenode

	device->drop();

	return 0;
}
