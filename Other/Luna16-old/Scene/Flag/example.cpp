
// only running opengl and assuming check for suitable shader features
// load the shaders
// Understood by : Klunk
// add your path stuff here

#ifdef _MSC_VER
	#pragma comment(lib, "Irrlicht.lib")
#endif

#pragma warning(disable : 4244)	// disable bitchin' about loss of data

#include <irrlicht.h>
#include <stdlib.h>
#include <time.h>
#include "CFlagSceneNode.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == irr::EET_KEY_INPUT_EVENT) KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		return false;
	}
	virtual bool IsKeyDown(EKEY_CODE keyCode) const{ return KeyIsDown[keyCode]; }
	EventReceiver(){ for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i) KeyIsDown[i] = false; }

private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

//********************************************************************************************

int main()
{
	srand(time(NULL));

	EventReceiver 	receiver;

// create irrlicht device
	IrrlichtDevice* device = createDevice( EDT_OPENGL, dimension2d<u32>(800, 600), 32, false, false, false, &receiver);
	if(!device)
		return 1;

	device->setWindowCaption(L"Flag Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	ILogger* output = device->getLogger();

	path vsFileName = "flag_shader.vert";
	path psFileName = "flag_shader.frag";

	if(!driver->queryFeature(EVDF_PIXEL_SHADER_1_1) && !driver->queryFeature(EVDF_ARB_FRAGMENT_PROGRAM_1))
	{
		output->log("WARNING: Pixel shaders disabled because of missing driver/hardware support.");
		psFileName = "";
	}

	if(!driver->queryFeature(EVDF_VERTEX_SHADER_1_1) && !driver->queryFeature(EVDF_ARB_VERTEX_PROGRAM_1))
	{
		output->log("WARNING: Vertex shaders disabled because of missing driver/hardware support.");
		vsFileName = "";
	}

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

	IImage* vertweightmap = driver->createImageFromFile("weightmap.tga");

// create a couple of perlin generators
	Perlin1D* pgen1 = new Perlin1D(128,5,0.05f,2.0f,rand());
	Perlin1D* pgen2 = new Perlin1D(128,6,0.04f,2.0f,rand());

// create a wind generator, there maybe issues/bugs with the direction handling
	CWindGen* wind = new CWindGen(vector3df(0.0f,0.0f,1.0f),30.0,pgen1,0.5f,1.5f,pgen2);

// create the flag
	CFlagSceneNode* flag = new CFlagSceneNode(smgr->getRootSceneNode(),smgr,345,wind,20.0f,16,12,20.0f,0.994f,
						0.05f,vector3df(0.0f,-0.08f,0.0f),vertweightmap);
	flag->getMaterial(0).setTexture(0, driver->getTexture("japan.tga"));
	flag->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)materialType;
	flag->setPosition(vector3df(0.0f,0.0f,0.0f));

// drop the weight map

	vertweightmap->drop();

	ILightSceneNode* lightnode = smgr->addLightSceneNode();
	lightnode->setLightType(ELT_DIRECTIONAL);
	lightnode->setRotation(vector3df(30.0f, -25.0f, 0.0f));
	lightnode->setPosition(vector3df(6.7f, 32.0f,-11.0f));

	ICameraSceneNode* cam = smgr->addCameraSceneNodeMaya();
	cam->setPosition(vector3df(150,0,0));
	cam->setTarget(vector3df(0,0,0));




	while(device->run())
	{
        driver->beginScene(true, true, SColor(255,100,101,140));
        smgr->drawAll();
		driver->endScene();
		device->sleep(1,0);
	}

	flag->drop();	// remove flag scenenode

	device->drop();

	return 0;
}
