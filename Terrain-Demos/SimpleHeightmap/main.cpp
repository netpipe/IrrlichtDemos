//TODO: fix normals (including smoothing with adjascent terrains)
//fix texture (well, just check UV coords are OK)


#include <irrlicht.h>
#include <stdio.h>
#pragma warning( disable : 4996 )

#include "SimpleHeightMap.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif




video::IVideoDriver* driver;
scene::ISceneManager* smgr;
IrrlichtDevice *device;



void SetTextures(video::SMaterial &material)
{
	video::ITexture *alpha1=driver->getTexture("Map/a100-00.bmp");
	video::ITexture *alpha2=driver->getTexture("Map/a200-00.bmp");

	material.TextureLayer[0].Texture = alpha1;
	material.TextureLayer[0].TextureWrapU = video::ETC_MIRROR;//So we won't get artifacts with borders
	material.TextureLayer[0].TextureWrapV = video::ETC_MIRROR;
	material.TextureLayer[0].BilinearFilter=true;
	material.TextureLayer[0].TrilinearFilter=true;

	material.TextureLayer[1].Texture = alpha2;
	material.TextureLayer[1].TextureWrapU = video::ETC_MIRROR;
	material.TextureLayer[1].TextureWrapV = video::ETC_MIRROR;
	material.TextureLayer[1].BilinearFilter=true;
	material.TextureLayer[1].TrilinearFilter=true;

	material.TextureLayer[ 2].Texture = driver->getTexture("Map/Base.jpg");
	material.TextureLayer[ 3].Texture = driver->getTexture("Map/FloorsPortuguese0001_7_S.jpg");
	material.TextureLayer[ 4].Texture = driver->getTexture("Map/CrackedEarth.jpg");
	material.TextureLayer[ 5].Texture = driver->getTexture("Map/Sand.jpg");
	material.TextureLayer[ 6].Texture = driver->getTexture("Map/RedSand.jpg");
	material.TextureLayer[ 7].Texture = driver->getTexture("Map/Grass.jpg");
}




class Terrain12SplatCallBack : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services,s32 userData)
	{

		// Vertex Shader
		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);


		core::matrix4 world;
		world *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorld", world.pointer(), 16);

		float Rotation=45;
		float r=-Rotation*3.14159265f/180.0f;
		services->setVertexShaderConstant("YRotation",&r,1);

		float fogstart=30000.0f;
		float fogstop=100000.0f;


		services->setPixelShaderConstant("eyePosition",(float*)&device->getSceneManager()->getActiveCamera()->getAbsolutePosition(),3);
		services->setPixelShaderConstant("fogstartdistance",(float*)&fogstart,3);
		services->setPixelShaderConstant("fogenddistance",(float*)&fogstop,3);


		core::vector3df m_AmbientColor=core::vector3df(1.0f,1.0f,1.0f);
		core::vector3df m_SunColor=core::vector3df(0.7f,0.7f,0.55f);
		core::vector3df m_SunDirection=core::vector3df(0,0.707f,0.707f);
		core::vector3df m_FogColor=core::vector3df(71.0f/255.0f, 80.0f/255.0f, 92.0f/255.0f);


		//services->setPixelShaderConstant("ambientcolor",(float*)&m_AmbientColor,3);
		//services->setPixelShaderConstant("sunlightcolor",(float*)&m_SunColor,3);
		//services->setPixelShaderConstant("dirlight",(float*)&m_SunDirection,3);
		//services->setPixelShaderConstant("fogcolor",(float*)&m_FogColor,3);
	}
};





int main()
{
	printf("0=old Terrain\n");
	printf("1=new Terrain\n");
	char choice=-1;
	while((choice!='0')&&(choice!='1'))
		choice=getchar();



	// ask user for driver
	video::E_DRIVER_TYPE driverType=video::EDT_OPENGL;

	// create device
	device = createDevice(driverType,
			core::dimension2d<u32>(640, 480), 16, false);

	if (device == 0)
		return 1; // could not create selected driver.

	// create engine and camera
	device->setWindowCaption(L"Simple Terrain Node Demo");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();

	scene::ICameraSceneNode *camera=smgr->addCameraSceneNodeFPS(0,150.0f,10.0f,-1,0,0,false);//0, core::vector3df(400,400,400), core::vector3df(0,0,0));
	camera->setPosition(core::vector3df(40,900,40));
	//camera->setTarget(core::vector3df(0,0,0));

	camera->setNearValue(50.0f);
	camera->setFarValue(200000.0f);


	SimpleHeightMap *SimpleTerrain=NULL;
	scene::ITerrainSceneNode* Terrain=NULL;


	//create the material:
	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	video::IShaderConstantSetCallBack *CallBack;
	CallBack = new Terrain12SplatCallBack();
	int ShaderNumber = gpu->addHighLevelShaderMaterialFromFiles(
		"Shader/terrainsplatting.hlsl","main_v", video::EVST_VS_3_0,
		"Shader/terrainsplatting.hlsl","main_f", video::EPST_PS_3_0,CallBack, video::EMT_SOLID,0);
	printf("ShaderNumber=%i\n",ShaderNumber);

	//todo shader needs to be ported to glsl

	if(choice=='1')
	{
		//use the new terrain:

		SimpleTerrain =
			new SimpleHeightMap(smgr, smgr->getRootSceneNode(), 0, "heightmap32bit00-00.raw",core::vector3df(0,0,0),core::vector3df(0,0,0),
			core::vector3df(800.0f,800.0f,800.0f));

		SimpleTerrain->setMaterialType((video::E_MATERIAL_TYPE)ShaderNumber);

		SetTextures(SimpleTerrain->getMaterial(0));
	}
	else
	{
		//use the original Terrain class:

		Terrain=smgr->addTerrainSceneNode(
			(const char*)0,
			0,										// parent node
			0,										// node id
			core::vector3df(0, 0.f, 0),			// position
			core::vector3df(0.f, 0.f, -2560),			// rotation
			core::vector3df(800.f, 800.0f, 800.f),		// scale
			video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
			8,										// maxLOD
			scene::ETPS_129,							// patchSize
			0,										// smoothFactor
			true			//add even if file is empty
			);

		io::IFileSystem* FileSystem=device->getFileSystem();

		io::IReadFile* file = FileSystem->createAndOpenFile("heightmap32bit00-00.raw");

		if(!file)
		{
			printf("Could not load terrain, file not found\n");
			getchar();
			return(-1);
		}

		Terrain->loadHeightMapRAW(file,32,true,true); // use this in terrain = smgr->addTerrainSceneNode( .. .. ... core::vector3df(100.f, 100.0f, 100.f),		// scale

		file->drop();

		Terrain->setMaterialType((video::E_MATERIAL_TYPE)ShaderNumber);

		SetTextures(Terrain->getMaterial(0));
	}



	//Draw loop:

	u32 frames=0;
	while(device->run())
	{
		driver->beginScene(true, true, video::SColor(0,100,100,100));

		smgr->drawAll();

		driver->endScene();
		if (++frames==100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();
			str+=L" primitivecount=";
			str+=(s32)driver->getPrimitiveCountDrawn();

			device->setWindowCaption(str.c_str());
			frames=0;
		}
	}

	device->drop();

	return 0;
}

