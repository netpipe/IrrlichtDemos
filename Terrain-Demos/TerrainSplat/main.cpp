#include <irrlicht.h>

using namespace irr;
using namespace video;
using namespace core;

class TerrainShaderCallBack : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services,
			s32 userData)
	{
			s32 alpha = 0;
			s32 tex1 = 1;
			s32 tex2 = 2;
			s32 tex3 = 3;
			services->setPixelShaderConstant("Alpha",reinterpret_cast<irr::f32*> (&alpha),1);
			services->setPixelShaderConstant("Tex1",reinterpret_cast<irr::f32*> (&tex1),1);
			services->setPixelShaderConstant("Tex2",reinterpret_cast<irr::f32*> (&tex2),1);
			services->setPixelShaderConstant("Tex3",reinterpret_cast<irr::f32*> (&tex3),1);
	}
};

int main()
{
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(640, 480), 16, false);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, .1f);
	cam->setPosition(core::vector3df(-100,50,100));
	cam->setTarget(core::vector3df(0,0,0));
	cam->setFarValue(42000.0f);

	const c8* vsFileName = "terrain.vert"; // filename for the vertex shader
	const c8* psFileName = "terrain.frag"; // filename for the pixel shader

	if (!driver->queryFeature(video::EVDF_PIXEL_SHADER_1_1) &&
		!driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1))
	{
		device->getLogger()->log("WARNING: Pixel shaders disabled "\
			"because of missing driver/hardware support.");
		psFileName = 0;
	}

	if (!driver->queryFeature(video::EVDF_VERTEX_SHADER_1_1) &&
		!driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1))
	{
		device->getLogger()->log("WARNING: Vertex shaders disabled "\
			"because of missing driver/hardware support.");
		vsFileName = 0;
	}

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	s32 terrainMaterialType = 0;

	if(gpu)
	{
		TerrainShaderCallBack* mc = new TerrainShaderCallBack();

		terrainMaterialType = gpu->addHighLevelShaderMaterialFromFiles(
				vsFileName, "vertexMain", video::EVST_VS_1_1,
				psFileName, "pixelMain", video::EPST_PS_1_1,
				mc, video::EMT_SOLID);

		mc->drop();
	}
	scene::IMeshSceneNode* node = smgr->addMeshSceneNode(smgr->addHillPlaneMesh("Terrain",
dimension2df(3,3),dimension2d<u32>(30,30)));

	node->setPosition(core::vector3df(0,0,0));
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setMaterialTexture(0,driver->getTexture("alpha.png"));
	node->setMaterialTexture(1,driver->getTexture("grass.jpg"));
	node->setMaterialTexture(2,driver->getTexture("stone.jpg"));
	node->setMaterialTexture(3,driver->getTexture("dirt.jpg"));
	node->setMaterialType((video::E_MATERIAL_TYPE)terrainMaterialType);

	int lastFPS = -1;

	while(device->run())
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, 0);

		smgr->drawAll();

device->sleep(1);
		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw str = L"Simple terrain splatting - Irrlicht Engine [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

	device->drop();

	return 0;
}

