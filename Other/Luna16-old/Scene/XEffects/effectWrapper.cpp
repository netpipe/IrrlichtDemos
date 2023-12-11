#include "effectWrapper.h"

using namespace irr;
using namespace scene;
using namespace gui;
using namespace video;
using namespace core;
using namespace io;


ICameraSceneNode* cam;
ISceneNode* cuby;
core::matrix4 worldViewProj;
core::matrix4 world;
core::matrix4 invWorld;
vector3df lightPosOS;
vector3df cubePos;
core::matrix4 viewi;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
s32 newMaterialType1 = 0;
s32 newMaterialType2 = 0;
s32 newMaterialType3 = 0;
ITexture* myTex;
bool ter = 0;
float mapres = 0;
bool filtering = 0;
float shaddark = -0.6f;
float fadevalue = 1.5f;
bool fading = 0;
float maxdist = 100;
float currtime = 0;


class DepthShaderCB : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		services->setPixelShaderConstant("MaxD", reinterpret_cast<f32*>(&maxdist), 1);

		//services->setPixelShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);
	}
};

class DetailShaderCB : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = cam->getProjectionMatrix();
		worldViewProj *= cam->getViewMatrix();
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj2", worldViewProj.pointer(), 16);

		services->setVertexShaderConstant("ShadDark", reinterpret_cast<f32*>(&shaddark), 1);

		services->setVertexShaderConstant("MaxD", reinterpret_cast<f32*>(&maxdist), 1);

		if(fading)
			services->setPixelShaderConstant("shadfade", reinterpret_cast<f32*>(&fadevalue), 1);

		if(services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		{
			int var0=0;
			services->setPixelShaderConstant("ColoredTextureSampler", (float*)(&var0), 1); //the colormap

			int var1=1;
			services->setPixelShaderConstant("DetailMap", (float*)(&var1), 1); //the colormap

			int var2=2;
			services->setPixelShaderConstant("ShadowMapSampler", (float*)(&var2), 1); //the colormap
		}

	}
};

class DetailShaderFCB : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = cam->getProjectionMatrix();
		worldViewProj *= cam->getViewMatrix();
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj2", worldViewProj.pointer(), 16);

		services->setVertexShaderConstant("MapRes", reinterpret_cast<f32*>(&mapres), 1);

		services->setVertexShaderConstant("MaxD", reinterpret_cast<f32*>(&maxdist), 1);

		services->setVertexShaderConstant("ShadDark", reinterpret_cast<f32*>(&shaddark), 1);

		//services->setPixelShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);

		if(fading)
			services->setPixelShaderConstant("shadfade", reinterpret_cast<f32*>(&fadevalue), 1);

		if(services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		{
			int var0=0;
			services->setPixelShaderConstant("ColoredTextureSampler", (float*)(&var0), 1); //the colormap

			int var1=1;
			services->setPixelShaderConstant("DetailMap", (float*)(&var1), 1); //the colormap

			int var2=2;
			services->setPixelShaderConstant("ShadowMapSampler", (float*)(&var2), 1); //the colormap
		}

	}
};

class SolidShaderCB : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = cam->getProjectionMatrix();
		worldViewProj *= cam->getViewMatrix();
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj2", worldViewProj.pointer(), 16);

		cubePos = (vector3df)cam->getPosition();
		//cubePos.invert();
		invWorld.transformVect(lightPosOS,cubePos);
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&cubePos), 4);

		services->setVertexShaderConstant("MaxD", reinterpret_cast<f32*>(&maxdist), 1);

		services->setVertexShaderConstant("ShadDark", reinterpret_cast<f32*>(&shaddark), 1);

		if(services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		{
			int var0=0;
			services->setPixelShaderConstant("ColoredTextureSampler", (float*)(&var0), 1); //the colormap

			int var1=1;
			services->setPixelShaderConstant("ShadowMapSampler", (float*)(&var1), 1); //the colormap
		}

	}
};

class SolidShaderFCB : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = cam->getProjectionMatrix();
		worldViewProj *= cam->getViewMatrix();
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj2", worldViewProj.pointer(), 16);

		cubePos = (vector3df)cam->getPosition();

		invWorld.transformVect(lightPosOS,cubePos);
		//lightPosOS.invert();
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);

		services->setVertexShaderConstant("MapRes", reinterpret_cast<f32*>(&mapres), 1);

		services->setVertexShaderConstant("MaxD", reinterpret_cast<f32*>(&maxdist), 1);

		services->setVertexShaderConstant("ShadDark", reinterpret_cast<f32*>(&shaddark), 1);

		if(services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		{
			int var0=0;
			services->setPixelShaderConstant("ColoredTextureSampler", (float*)(&var0), 1); //the colormap

			int var1=1;
			services->setPixelShaderConstant("ShadowMapSampler", (float*)(&var1), 1); //the colormap
		}
	}
};

class LightShaderCB : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		cubePos = (vector3df)cam->getPosition();
		cubePos.invert();
		invWorld.transformVect(lightPosOS,cubePos);
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);
	}
};

class WiggleShaderCB : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("World", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(video::ETS_WORLD);
		worldViewProj.getInverse(worldViewProj);
		worldViewProj.getTransposed(worldViewProj);
		services->setVertexShaderConstant("mInvWorld", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(video::ETS_VIEW);
		worldViewProj.getInverse(worldViewProj);
		services->setVertexShaderConstant("ViewI", worldViewProj.pointer(), 16);

		cubePos = (vector3df)cam->getPosition();
		cubePos.invert();
		invWorld.transformVect(lightPosOS,cubePos);
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);


		services->setVertexShaderConstant("Timer", reinterpret_cast<f32*>(&currtime), 1);
	}
};


class DepthWiggleShaderCB : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);


		services->setVertexShaderConstant("MaxD", reinterpret_cast<f32*>(&maxdist), 1);

		services->setVertexShaderConstant("Timer", reinterpret_cast<f32*>(&currtime), 1);
	}
};






class AnisoShaderCB : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("World", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(video::ETS_VIEW);
		worldViewProj.getInverse(worldViewProj);
		services->setVertexShaderConstant("ViewI", worldViewProj.pointer(), 16);

		cubePos = (vector3df)cam->getPosition();
		cubePos.invert();
		invWorld.transformVect(lightPosOS,cubePos);
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);


	}
};


class PhongShaderCB : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("World", worldViewProj.pointer(), 16);



		cubePos = (vector3df)cam->getPosition();
		cubePos.invert();
		invWorld.transformVect(lightPosOS,cubePos);
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);



		cubePos = (vector3df)smgr->getActiveCamera()->getPosition();
		cubePos.invert();
		invWorld.transformVect(lightPosOS,cubePos);
		services->setVertexShaderConstant("CamPos", reinterpret_cast<f32*>(&lightPosOS), 4);


	}
};


effectHandler::effectHandler(IrrlichtDevice* dev, dimension2du mapSize, bool useAccurate)
{
	device = dev;
	smgr = dev->getSceneManager();
	driver = dev->getVideoDriver();
	mapRes = mapSize;
	mapres = mapSize.Height;

	ShadowMapTex = driver->addRenderTargetTexture(mapSize);
	//ShadowMapBuffer = driver->createRenderTargetTexture(mapSize);

	cameraBuffer = smgr->getActiveCamera();
	lightCam = smgr->addCameraSceneNode();
	lightCam->setFarValue(200);
	cam = lightCam;


	smgr->setActiveCamera(cameraBuffer);
	lightPosOS = vector3df(0,0,0);

	// For shadows
	c8* DFN = "shaders/xeffects/ShadowPass1.hlsl";
	c8* SDFN = "shaders/xeffects/ShadowPass2SolidDiff.hlsl";
	c8* SDF4FN = "shaders/xeffects/ShadowPass2SolidDiffF4.hlsl";
	c8* DNDNFFN = "shaders/xeffects/ShadowPass2DetailNoDiff.hlsl";
	c8* DNDF4FN = "shaders/xeffects/ShadowPass2DetailNoDiffF4.hlsl";
	c8* DNDF8FN = "shaders/xeffects/ShadowPass2DetailNoDiffF8.hlsl";
	c8* DNDF12FN = "shaders/xeffects/ShadowPass2DetailNoDiffF12.hlsl";

	// Other effects
	c8* GOOCH = "shaders/xeffects/ShaderGoochPt.hlsl";
	c8* WIGGLE = "shaders/xeffects/ShaderMrWiggle.hlsl";
	c8* DEPTHWIGGLE = "shaders/xeffects/ShaderDepthWiggle.hlsl";
	c8* ANISO = "shaders/xeffects/ShaderAniso.hlsl";
	c8* PHONG = "shaders/xeffects/ShaderPhong.hlsl";
	c8* BRDF = "shaders/xeffects/ShaderBRDF.hlsl";


	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

		if(gpu && driver->getDriverType() == EDT_OPENGL && driver->queryFeature(EVDF_ARB_GLSL))
		{
			DepthShaderCB* mc = new DepthShaderCB();
			SolidShaderCB* mc1 = new SolidShaderCB();
			DetailShaderCB* mc2 = new DetailShaderCB();
			DetailShaderFCB* mc3 = new DetailShaderFCB();
			SolidShaderFCB* mc4 = new SolidShaderFCB();

				Depth = gpu->addHighLevelShaderMaterialFromFiles(
					"shaders/xeffects/ShadowPass1v.glsl",	"vertexMain", video::EVST_VS_1_1,
					"shaders/xeffects/ShadowPass1p.glsl", "pixelMain", video::EPST_PS_2_0,
					mc, video::EMT_SOLID);

				DepthWiggle = gpu->addHighLevelShaderMaterialFromFiles(
					"shaders/xeffects/ShadowPass1v.glsl",	"vertexMain", video::EVST_VS_1_1,
					"shaders/xeffects/ShadowPass1p.glsl", "pixelMain", video::EPST_PS_2_0,
					mc, video::EMT_SOLID);

				Solid = gpu->addHighLevelShaderMaterialFromFiles(
					"shaders/xeffects/ShadowPass2SolidAV.glsl",	"vertexMain", video::EVST_VS_1_1,
					"shaders/xeffects/ShadowPass2SolidAP.glsl", "pixelMain", video::EPST_PS_2_0,
					mc1, video::EMT_SOLID);

				SolidPCF4 = gpu->addHighLevelShaderMaterialFromFiles(
					"shaders/xeffects/ShadowPass2Solid4AV.glsl",	"vertexMain", video::EVST_VS_1_1,
					"shaders/xeffects/ShadowPass2Solid4AP.glsl", "pixelMain", video::EPST_PS_2_0,
					mc1, video::EMT_SOLID);

				Detail = gpu->addHighLevelShaderMaterialFromFiles(
					"shaders/xeffects/ShadowPass2DetailAV.glsl",	"vertexMain", video::EVST_VS_1_1,
					"shaders/xeffects/ShadowPass2DetailAP.glsl", "pixelMain", video::EPST_PS_2_0,
					mc2, video::EMT_DETAIL_MAP);

				DetailPCF4 = gpu->addHighLevelShaderMaterialFromFiles(
					"shaders/xeffects/ShadowPass2Detail4AV.glsl",	"vertexMain", video::EVST_VS_1_1,
					"shaders/xeffects/ShadowPass2Detail4AP.glsl", "pixelMain", video::EPST_PS_2_0,
					mc3, video::EMT_DETAIL_MAP);


				DetailPCF8 = gpu->addHighLevelShaderMaterialFromFiles(
					"shaders/xeffects/ShadowPass2Detail8AV.glsl",	"vertexMain", video::EVST_VS_1_1,
					"shaders/xeffects/ShadowPass2Detail8AP.glsl", "pixelMain", video::EPST_PS_2_0,
					mc3, video::EMT_DETAIL_MAP);

			Brdf = EMT_SOLID;
			Aniso = EMT_SOLID;
			Phong = EMT_SOLID;
			Wiggle = EMT_SOLID;
			Gooch = EMT_SOLID;
			DetailPCF12 = EMT_DETAIL_MAP;

			mc->drop();
			mc1->drop();
			mc2->drop();
			mc3->drop();
			mc4->drop();

			printf("\n OpenGL ShadowMaps initialized.");

		}
		else
		{
			Brdf = EMT_SOLID;
			Aniso = EMT_SOLID;
			Phong = EMT_SOLID;
			Wiggle = EMT_SOLID;
			Gooch = EMT_SOLID;
			Depth = EMT_SOLID;
			Solid = EMT_SOLID;
			SolidPCF4 = EMT_SOLID;
			Detail = EMT_DETAIL_MAP;
			DetailPCF4 = EMT_DETAIL_MAP;
			DetailPCF8 = EMT_DETAIL_MAP;
			DetailPCF12 = EMT_DETAIL_MAP;

			printf("\n ShadowMaps: Shadow maps not supported on this system. \n");
		}


	DetailStore = Detail;
	DetailPCF4Store = DetailPCF4;
	DetailPCF8Store = DetailPCF8;
	DetailPCF12Store = DetailPCF12;
};

void effectHandler::addShadowToNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType)
{
	shadowNode snode;
	snode.node = node;
	snode.filterType = filterType;
	snode.materialType = node->getMaterial(0).MaterialType;
	snode.depthOnly = false;
	snode.exclude = false;


	if(node->getMaterial(0).MaterialType == EMT_DETAIL_MAP || node->getMaterial(0).MaterialType == EMT_LIGHTMAP)
		node->setMaterialTexture(2,ShadowMapTex);

	if(node->getMaterial(0).MaterialType == EMT_SOLID)
		node->setMaterialTexture(1,ShadowMapTex);

	shadowNodeArray.push_back(snode);
}


void effectHandler::addNodeToDepthPass(irr::scene::ISceneNode *node)
{
	shadowNode snode;
	snode.node = node;
	snode.materialType = node->getMaterial(0).MaterialType;
	snode.depthOnly = true;
	snode.exclude = false;

	shadowNodeArray.push_back(snode);
}

void effectHandler::excludeNodeFromDepthPass(irr::scene::ISceneNode *node)
{
	shadowNode snode;
	snode.node = node;
	snode.exclude = true;

	shadowNodeArray.push_back(snode);
}

void effectHandler::removeShadowFromNode(irr::scene::ISceneNode *node)
{
	int i = 0;
	while(shadowNodeArray[i].node != node && i < shadowNodeArray.size())
	{
		i++;
	}
	shadowNodeArray.erase(i);
}

void effectHandler::enableFading(float fadeval)
{


	if(driver->queryFeature(EVDF_PIXEL_SHADER_2_0) && driver->getDriverType() == EDT_DIRECT3D9)
	{
		video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

		DepthShaderCB* mc = new DepthShaderCB();
		SolidShaderCB* mc1 = new SolidShaderCB();
		DetailShaderCB* mc2 = new DetailShaderCB();
		DetailShaderFCB* mc3 = new DetailShaderFCB();

		DetailStore = Detail;
		DetailPCF4Store = DetailPCF4;
		DetailPCF8Store = DetailPCF8;
		DetailPCF12Store = DetailPCF12;

		c8* DNDNFFN = "shaders/xeffects/ShadowPass2DetailNoDiffAFade.hlsl";
		c8* DNDF4FN = "shaders/xeffects/ShadowPass2DetailNoDiffF4AFade.hlsl";
		c8* DNDF8FN = "shaders/xeffects/ShadowPass2DetailNoDiffF8AFade.hlsl";
		c8* DNDF12FN = "shaders/xeffects/ShadowPass2DetailNoDiffF12AFade.hlsl";


		Detail = gpu->addHighLevelShaderMaterialFromFiles(
			DNDNFFN,	"vertexMain", video::EVST_VS_1_1,
			DNDNFFN, "pixelMain", video::EPST_PS_2_0,
			mc2, video::EMT_DETAIL_MAP);

		DetailPCF4 = gpu->addHighLevelShaderMaterialFromFiles(
			DNDF4FN,	"vertexMain", video::EVST_VS_1_1,
			DNDF4FN, "pixelMain", video::EPST_PS_2_0,
			mc3, video::EMT_DETAIL_MAP);


		DetailPCF8 = gpu->addHighLevelShaderMaterialFromFiles(
			DNDF8FN,	"vertexMain", video::EVST_VS_1_1,
			DNDF8FN, "pixelMain", video::EPST_PS_2_0,
			mc3, video::EMT_DETAIL_MAP);

		if(driver->queryFeature(EVDF_PIXEL_SHADER_3_0))
			DetailPCF12 = gpu->addHighLevelShaderMaterialFromFiles(
			DNDF12FN,	"vertexMain", video::EVST_VS_1_1,
			DNDF12FN, "pixelMain", video::EPST_PS_3_0,
			mc3, video::EMT_DETAIL_MAP);

		fading = true;
		fadevalue = fadeval;
	}
	else
	{
		printf("\n ShadowMaps: Shadow fading not supported on this system.");
	}

}

void effectHandler::disableFading()
{
	Detail = DetailStore;
	DetailPCF4 = DetailPCF4Store;
	DetailPCF8 = DetailPCF8Store;
	DetailPCF12 = DetailPCF12Store;
	fading = false;
}


void effectHandler::addEffectToNode(ISceneNode* node, E_EFFECT_TYPE etype)
{
	switch(etype)
	{
	case EET_GOOCH:
		node->setMaterialType((E_MATERIAL_TYPE)Gooch);
		return;
	case EET_MRWIGGLE:
		node->setMaterialType((E_MATERIAL_TYPE)Wiggle);
		return;
	case EET_ANISO:
		node->setMaterialType((E_MATERIAL_TYPE)Aniso);
		return;
	case EET_PHONG:
		node->setMaterialType((E_MATERIAL_TYPE)Phong);
		return;
	case EET_BRDF:
		node->setMaterialType((E_MATERIAL_TYPE)Brdf);
		return;
	}
}


void effectHandler::update()
{
	currtime = device->getTimer()->getTime() / 25;

	cameraBuffer = smgr->getActiveCamera();


	smgr->setActiveCamera(lightCam);

	/*
	// get pointer to texture data which in this case is basicly array of u32 values, you need to cast since function returns void pointer, however you already know color format and thus what to cast to
	u32* p = (u32*)ShadowMapTex->lock();
	u32* j = (u32*)ShadowMapBuffer->lock();

	// now use that pointer to acces array
	for(u32 i=0; i < ShadowMapTex->getSize().Width * ShadowMapTex->getSize().Height; i++)
	{
	p[i] = j[i];
	}

	// now unlock texture
	ShadowMapTex->unlock();
	ShadowMapBuffer->unlock();

	ShadowMapTex->regenerateMipMapLevels();
	ShadowMapBuffer->regenerateMipMapLevels();
	*/

	driver->setRenderTarget(ShadowMapTex);

	for(int i = 0;i < shadowNodeArray.size();i++)
	{
		if(shadowNodeArray[i].exclude)
		{
			shadowNodeArray[i].node->setVisible(false);
		}
		else
		{
			if(shadowNodeArray[i].materialType == Wiggle)
				shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)DepthWiggle);
			else
				shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)Depth);
		}
	}

	smgr->drawAll();

	driver->setRenderTarget(0);
	smgr->setActiveCamera(cameraBuffer);

	lightPosOS = lightCam->getPosition();

	for(int i = 0;i < shadowNodeArray.size();i++)
	{
		if(shadowNodeArray[i].exclude)
		{
			shadowNodeArray[i].node->setVisible(true);
		}
		else
		{
			if(shadowNodeArray[i].depthOnly)
			{
				shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)shadowNodeArray[i].materialType);
			}
			else
			{



				if(shadowNodeArray[i].materialType == EMT_SOLID)
				{
					if(shadowNodeArray[i].filterType == EFT_NONE)
						shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)Solid);
					if(shadowNodeArray[i].filterType == EFT_4PCF)
						shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)SolidPCF4);
				}

				if(shadowNodeArray[i].materialType == EMT_DETAIL_MAP || shadowNodeArray[i].materialType == EMT_LIGHTMAP)
				{
					if(shadowNodeArray[i].filterType == EFT_NONE)
						shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)Detail);

					if(shadowNodeArray[i].filterType == EFT_4PCF)
						shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)DetailPCF4);

					if(shadowNodeArray[i].filterType == EFT_8PCF)
						shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)DetailPCF8);

					if(shadowNodeArray[i].filterType == EFT_12PCF)
						shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)DetailPCF12);
				}


			}


		}
	}
}

void effectHandler::setMaxShadowDistanceFromLight(int distance)
{
	cam->setFarValue(distance);
	maxdist = distance / 2;
}

void effectHandler::setShadowDarkness(float shadalpha)
{
	shaddark = shadalpha;
}

// This code is covered by a zlib-style, please find enclosed "License.txt"
// Copyright (C) Ahmed Hilali 2007
