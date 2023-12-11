#include "effectWrapper.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace irr;
using namespace scene;
using namespace gui;
using namespace video;
using namespace core;
using namespace io;

class DepthShaderCB : public video::IShaderConstantSetCallBack
{
public:
	DepthShaderCB(effectHandler* effectIn) : effect(effectIn), FarValue(new f32(0)) {};
	~DepthShaderCB() {delete FarValue;};


	effectHandler* effect;
	f32* FarValue;
	core::matrix4 worldViewProj;

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		
		services->setPixelShaderConstant("MaxD", FarValue, 1);
	}
};

class ShadowShaderCB : public video::IShaderConstantSetCallBack
{
public:
	ShadowShaderCB(effectHandler* effectIn) : effect(effectIn), Tex2(false), EnableLighting(0) {};

	effectHandler* effect;
	core::matrix4 worldViewProj;
	core::matrix4 invWorld;
	bool Tex2;
	float EnableLighting;

	core::matrix4 ProjLink;
	core::matrix4 ViewLink;

	virtual void OnSetMaterial(const SMaterial& material) 
	{ 
		Tex2 = (material.getTexture(2) == effect->getShadowMapTexture());

		EnableLighting = material.Lighting ? 0.0f : 1.0f;
	}

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = ProjLink;			
		worldViewProj *= ViewLink;
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj2", worldViewProj.pointer(), 16);

		if(EnableLighting > 0.5f)
		{
			driver->getTransform(video::ETS_WORLD).getInverse(invWorld);

			vector3df lightPosOS = effect->getLightCamera()->getPosition();
			invWorld.transformVect(lightPosOS,lightPosOS); 
			services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);
		}

		f32 shaddark = effect->getShadowDarkness();
		services->setVertexShaderConstant("ShadDark", reinterpret_cast<f32*>(&shaddark), 1);

		f32 maxdist = (f32)effect->getMaxShadowDistanceFromLight();
		services->setVertexShaderConstant("MaxD", reinterpret_cast<f32*>(&maxdist), 1);

		services->setVertexShaderConstant("EnableLighting", reinterpret_cast<f32*>(&EnableLighting), 1);

		if(services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		{
			int var0=0;
			services->setPixelShaderConstant("ColoredTextureSampler", (float*)(&var0), 1); 

			if(Tex2)
			{
				int var1=1;
				services->setPixelShaderConstant("DetailMap", (float*)(&var1), 1); 

				int var2=2;
				services->setPixelShaderConstant("ShadowMapSampler", (float*)(&var2), 1); 

				//int var3=3;
				//services->setPixelShaderConstant("RandMapSampler", (float*)(&var3), 1); 
			}
			else
			{
				int var1=1;
				services->setPixelShaderConstant("ShadowMapSampler", (float*)(&var1), 1); 

				//int var2=2;
				//services->setPixelShaderConstant("RandMapSampler", (float*)(&var2), 1); 
			}
		}
	}
};

class LightShaderCB : public video::IShaderConstantSetCallBack
{
public:
	LightShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	core::matrix4 worldViewProj;
	core::matrix4 invWorld;

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
	
		driver->getTransform(video::ETS_WORLD).getInverse(invWorld);

		vector3df lightPosOS = effect->getLightCamera()->getPosition();
		invWorld.transformVect(lightPosOS,lightPosOS); 
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);
	}
};

class WiggleShaderCB : public video::IShaderConstantSetCallBack
{
public:
	WiggleShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	core::matrix4 worldViewProj;
	core::matrix4 invWorld;

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

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

		driver->getTransform(video::ETS_WORLD).getInverse(invWorld);

		vector3df lightPosOS = effect->getLightCamera()->getPosition();
		invWorld.transformVect(lightPosOS,lightPosOS); 
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);

		f32 currtime = effect->getTime();
		services->setVertexShaderConstant("Timer", reinterpret_cast<f32*>(&currtime), 1);
	}
};


class DepthWiggleShaderCB : public video::IShaderConstantSetCallBack
{
public:
	DepthWiggleShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	core::matrix4 worldViewProj;

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		f32 maxdist = (f32)effect->getMaxShadowDistanceFromLight();
		services->setVertexShaderConstant("MaxD", reinterpret_cast<f32*>(&maxdist), 1);

		f32 currtime = (f32)effect->getTime();
		services->setVertexShaderConstant("Timer", reinterpret_cast<f32*>(&currtime), 1);
	}
};






class AnisoShaderCB : public video::IShaderConstantSetCallBack
{
public:
	AnisoShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	core::matrix4 worldViewProj;
	core::matrix4 invWorld;

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("World", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(video::ETS_VIEW);
		worldViewProj.getInverse(worldViewProj);
		services->setVertexShaderConstant("ViewI", worldViewProj.pointer(), 16);

		driver->getTransform(video::ETS_WORLD).getInverse(invWorld);

		vector3df lightPosOS = effect->getLightCamera()->getPosition();
		invWorld.transformVect(lightPosOS,lightPosOS); 
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);


	}
};

class PhongShaderCB : public video::IShaderConstantSetCallBack
{
public:
	PhongShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	core::matrix4 worldViewProj;
	core::matrix4 invWorld;

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("World", worldViewProj.pointer(), 16);

		driver->getTransform(video::ETS_WORLD).getInverse(invWorld);

		vector3df lightPosOS = effect->getLightCamera()->getPosition();
		invWorld.transformVect(lightPosOS,lightPosOS); 
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);
		services->setVertexShaderConstant("CamPos", reinterpret_cast<f32*>(&lightPosOS), 4);
	}
};


class BRDFShaderCB : public video::IShaderConstantSetCallBack
{
public:
	BRDFShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	core::matrix4 worldViewProj;
	core::matrix4 invWorld;

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("World", worldViewProj.pointer(), 16);

		driver->getTransform(video::ETS_WORLD).getInverse(invWorld);

		vector3df lightPosOS = effect->getLightCamera()->getPosition();
		invWorld.transformVect(lightPosOS,lightPosOS); 
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);
		services->setVertexShaderConstant("CamPos", reinterpret_cast<f32*>(&lightPosOS), 4);

		
		if(services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		{
			int var0=0;
			services->setPixelShaderConstant("hmapSampler", (float*)(&var0), 1); 

			int var1=1;
			services->setPixelShaderConstant("nmapSampler", (float*)(&var1), 1); 
		}
	}
};

effectHandler::effectHandler(IrrlichtDevice* dev, dimension2d<s32> mapSize, core::stringc shaderFolder, irr::core::dimension2d<irr::s32> screenRTTSize)
{
	device = dev;
	smgr = dev->getSceneManager();
	driver = dev->getVideoDriver();

	CShaderPreprocessor* sPP = new CShaderPreprocessor(driver);
	sPP->addShaderDefine("MAPRES", mapSize.Height);

	ScreenRTTSize = (screenRTTSize == dimension2d<s32>(0,0)) ? driver->getScreenSize() : screenRTTSize;
	
	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

	ShadowMapTex = driver->createRenderTargetTexture(mapSize);
	
	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);

	ICameraSceneNode* cameraBuffer = smgr->getActiveCamera();
	lightCam = smgr->addCameraSceneNode();
	smgr->setActiveCamera(cameraBuffer);

	lightCam->setFarValue(200);
	maxdist = 200;
	ClearColour = SColor(0,0,0,0);
	shadowsUnsupported = false;
	ScreenQuad = 0;
	DepthRTT = 0;
	depthLink = 0;
	DepthPass = false;
	shaddark = 1.0f;

	stringc platformExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";

	// For shadows
	stringc DFNV = shaderFolder + stringc("shaders/ShadowPass1V") + platformExt;
	stringc DFNP = shaderFolder + stringc("shaders/ShadowPass1P") + platformExt;

	stringc SDFNV = shaderFolder + stringc("shaders/ShadowPass2V") + platformExt;
	stringc SDFNP = shaderFolder + stringc("shaders/ShadowPass2P") + platformExt;
	
	stringc GOOCHV = shaderFolder + stringc("shaders/ShaderGoochPtV") + platformExt;
	stringc WIGGLEV = shaderFolder + stringc("shaders/ShaderMrWiggleV") + platformExt;
	stringc DEPTHWIGGLEV = shaderFolder + stringc("shaders/ShaderDepthWiggleV") + platformExt;
	stringc ANISOV = shaderFolder + stringc("shaders/ShaderAnisoV") + platformExt;
	stringc PHONGV = shaderFolder + stringc("shaders/ShaderPhongV") + platformExt;
	stringc BRDFV = shaderFolder + stringc("shaders/ShaderBRDFV") + platformExt;

	stringc GOOCHP = shaderFolder + stringc("shaders/ShaderGoochPtP") + platformExt;
	stringc WIGGLEP = shaderFolder + stringc("shaders/ShaderMrWiggleP") + platformExt;
	stringc DEPTHWIGGLEP = shaderFolder + stringc("shaders/ShaderDepthWiggleP") + platformExt;
	stringc ANISOP = shaderFolder + stringc("shaders/ShaderAnisoP") + platformExt;
	stringc PHONGP = shaderFolder + stringc("shaders/ShaderPhongP") + platformExt;
	stringc BRDFP = shaderFolder + stringc("shaders/ShaderBRDFP") + platformExt;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	
	if(gpu && ((driver->getDriverType() == EDT_OPENGL && driver->queryFeature(EVDF_ARB_GLSL)) ||
			   (driver->getDriverType() == EDT_DIRECT3D9 && driver->queryFeature(EVDF_PIXEL_SHADER_1_4))))
	{
			DepthShaderCB* mc = new DepthShaderCB(this);
			depthLink = mc->FarValue;
			*depthLink = 200;

			ShadowShaderCB* mc1 = new ShadowShaderCB(this);
			ProjLink = &mc1->ProjLink;
			ViewLink = &mc1->ViewLink;

			BRDFShaderCB* mc4 = new BRDFShaderCB(this);
			LightShaderCB* mc5 = new LightShaderCB(this);
			WiggleShaderCB* mc6 = new WiggleShaderCB(this);
			DepthWiggleShaderCB* mc7 = new DepthWiggleShaderCB(this);
			AnisoShaderCB* mc8 = new AnisoShaderCB(this);
			PhongShaderCB* mc9 = new PhongShaderCB(this);

			Brdf = gpu->addHighLevelShaderMaterialFromFiles(
				BRDFV.c_str(),	"vertexMain", video::EVST_VS_1_1,
				BRDFP.c_str(), "pixelMain", video::EPST_PS_1_4,
				mc4, video::EMT_SOLID);
			Gooch = gpu->addHighLevelShaderMaterialFromFiles(
				GOOCHV.c_str(),	"vertexMain", video::EVST_VS_1_1,
				GOOCHP.c_str(), "pixelMain", video::EPST_PS_1_4,
				mc5, video::EMT_SOLID);

			Wiggle = gpu->addHighLevelShaderMaterialFromFiles(
				WIGGLEV.c_str(),	"vertexMain", video::EVST_VS_1_1,
				WIGGLEP.c_str(), "pixelMain", video::EPST_PS_1_4,
				mc6, video::EMT_SOLID);

			Aniso = gpu->addHighLevelShaderMaterialFromFiles(
				ANISOV.c_str(),	"vertexMain", video::EVST_VS_1_1,
				ANISOP.c_str(), "pixelMain", video::EPST_PS_1_4,
				mc8, video::EMT_SOLID);

			Phong = gpu->addHighLevelShaderMaterialFromFiles(
				PHONGV.c_str(),	"vertexMain", video::EVST_VS_1_1,
				PHONGP.c_str(), "pixelMain", video::EPST_PS_1_4,
				mc9, video::EMT_SOLID);

			if(driver->queryFeature(EVDF_PIXEL_SHADER_2_0) || driver->getDriverType() == EDT_OPENGL)
			{
				Depth = gpu->addHighLevelShaderMaterialFromFiles(
					DFNV.c_str(),	"vertexMain", video::EVST_VS_2_0,
					DFNP.c_str(), "pixelMain", video::EPST_PS_2_0,
					mc, video::EMT_SOLID);
				
				DepthWiggle = gpu->addHighLevelShaderMaterialFromFiles(
					DEPTHWIGGLEV.c_str(),	"vertexMain", video::EVST_VS_2_0,
					DEPTHWIGGLEP.c_str(), "pixelMain", video::EPST_PS_2_0,
					mc7, video::EMT_SOLID);

				sPP->addShaderDefine("SAMPLE_AMOUNT",1);
				Solid = gpu->addHighLevelShaderMaterial(
					sPP->ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_2_0,
					sPP->ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					mc1, video::EMT_SOLID);

				sPP->addShaderDefine("SAMPLE_AMOUNT",4);
				SolidPCF4 = gpu->addHighLevelShaderMaterial(
					sPP->ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_2_0,
					sPP->ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					mc1, video::EMT_SOLID);

				sPP->addShaderDefine("SAMPLE_AMOUNT",7);
				SolidPCF8 = gpu->addHighLevelShaderMaterial(
					sPP->ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_2_0,
					sPP->ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					mc1, video::EMT_SOLID);

				sPP->addShaderDefine("SAMPLE_AMOUNT",12);
				if(driver->queryFeature(EVDF_PIXEL_SHADER_3_0) || driver->getDriverType() == EDT_OPENGL)
				SolidPCF12 = gpu->addHighLevelShaderMaterial(
					sPP->ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_3_0,
					sPP->ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_3_0,
					mc1, video::EMT_SOLID);
				else
					SolidPCF12 = SolidPCF8;

				sPP->addShaderDefine("SAMPLE_AMOUNT",16);
				if(driver->queryFeature(EVDF_PIXEL_SHADER_3_0) || driver->getDriverType() == EDT_OPENGL)
				SolidPCF16 = gpu->addHighLevelShaderMaterial(
					sPP->ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_3_0,
					sPP->ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_3_0,
					mc1, video::EMT_SOLID);
				else
					SolidPCF16 = SolidPCF8;

				// Enable 2 texture layers.
				sPP->addShaderDefine("2_LAYER");

				sPP->addShaderDefine("SAMPLE_AMOUNT",1);
				Detail = gpu->addHighLevelShaderMaterial(
					sPP->ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_2_0,
					sPP->ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					mc1, video::EMT_DETAIL_MAP);
 
				sPP->addShaderDefine("SAMPLE_AMOUNT",4);
				DetailPCF4 = gpu->addHighLevelShaderMaterial(
					sPP->ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_2_0,
					sPP->ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					mc1, video::EMT_DETAIL_MAP);

				sPP->addShaderDefine("SAMPLE_AMOUNT",7);
				DetailPCF8 = gpu->addHighLevelShaderMaterial(
					sPP->ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_2_0,
					sPP->ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					mc1, video::EMT_DETAIL_MAP);

				sPP->addShaderDefine("SAMPLE_AMOUNT",12);
				if(driver->queryFeature(EVDF_PIXEL_SHADER_3_0) || driver->getDriverType() == EDT_OPENGL)
					DetailPCF12 = gpu->addHighLevelShaderMaterial(
					sPP->ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_3_0,
					sPP->ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_3_0,
					mc1, video::EMT_DETAIL_MAP);
				else
					DetailPCF12 = DetailPCF8;

				sPP->addShaderDefine("SAMPLE_AMOUNT",16);
				if(driver->queryFeature(EVDF_PIXEL_SHADER_3_0) || driver->getDriverType() == EDT_OPENGL)
					DetailPCF16 = gpu->addHighLevelShaderMaterial(
					sPP->ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_3_0,
					sPP->ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_3_0,
					mc1, video::EMT_DETAIL_MAP);
				else
					DetailPCF16 = DetailPCF8;
			}
			else
			{
				Depth = EMT_SOLID;
				DepthWiggle = EMT_SOLID;
				Solid = EMT_SOLID;
				SolidPCF4 = EMT_SOLID;
				SolidPCF8 = EMT_SOLID;
				SolidPCF12 = EMT_SOLID;
				SolidPCF16 = EMT_SOLID;
				Detail = EMT_DETAIL_MAP;
				DetailPCF4 = EMT_DETAIL_MAP;
				DetailPCF8 = EMT_DETAIL_MAP;
				DetailPCF12 = EMT_DETAIL_MAP;
				DetailPCF16 = EMT_DETAIL_MAP;

				device->getLogger()->log("ShadowMaps: Shadow maps not supported on this system.");
				shadowsUnsupported = true;
			}

			mc->drop();
			mc1->drop();
			mc4->drop();
			mc5->drop();
			mc6->drop();
			mc7->drop();
			mc8->drop();
			mc9->drop();
	
	}
	else
	{
		Depth = EMT_SOLID;
		Solid = EMT_SOLID;
		SolidPCF4 = EMT_SOLID;
		SolidPCF8 = EMT_SOLID;
		SolidPCF12 = EMT_SOLID;
		SolidPCF16 = EMT_SOLID;
		Detail = EMT_DETAIL_MAP;
		DetailPCF4 = EMT_DETAIL_MAP;
		DetailPCF8 = EMT_DETAIL_MAP;
		DetailPCF12 = EMT_DETAIL_MAP;
		DetailPCF16 = EMT_DETAIL_MAP;
		Brdf = EMT_SOLID;
		Aniso = EMT_SOLID;
		Phong = EMT_SOLID;
		Wiggle = EMT_SOLID;
		Gooch = EMT_SOLID;

		device->getLogger()->log("ShadowMaps: Shadow maps not supported on this system.");
		device->getLogger()->log("Effects: Effects not supported on this system.");
	}
	
	delete sPP;
}

void effectHandler::enableDepthPass(bool enableDepthPass)
{
	DepthPass = enableDepthPass;

	if(DepthPass && DepthRTT == 0)
	{
		bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
	
		DepthRTT = driver->createRenderTargetTexture(ScreenRTTSize);

		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);
	}
};

void effectHandler::addPostProcessingEffect(irr::s32 MaterialType)
{
	if(ScreenQuad == 0)
	{
		// Init screen quad.
		ScreenQuad = new CBaseFilter(smgr->getRootSceneNode(), smgr, -1);
		ScreenQuad->drop();

		bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
		driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

		ScreenRTT = driver->createRenderTargetTexture(ScreenRTTSize);
		ScreenQuad->rt[0] = driver->createRenderTargetTexture(ScreenRTTSize);
		ScreenQuad->rt[1] = driver->createRenderTargetTexture(ScreenRTTSize);
		
		driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	}

	PostProcessingRoutines.push_back(MaterialType);
}

void effectHandler::addShadowToNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType, E_SHADOW_MODE shadowMode)
{
	if(shadowMode != ESM_CAST)
	{
		if(node->getMaterial(0).MaterialType == EMT_DETAIL_MAP || node->getMaterial(0).MaterialType == EMT_LIGHTMAP)
			node->setMaterialTexture(2,ShadowMapTex);
		else
			node->setMaterialTexture(1,ShadowMapTex);

		// Need to disable bilinear for 16-bit depth precision.
		for(u32 i = 0;i < node->getMaterialCount();++i)
			node->getMaterial(i).TextureLayer[2].BilinearFilter = false;
	}
	
	shadowNode snode;
	snode.node = node;
	snode.filterType = filterType;

	for(u32 i = 0;i < node->getMaterialCount();++i)
		snode.materialTypes.push_back(node->getMaterial(i).MaterialType);

	snode.shadowMode = shadowMode;

	shadowNodeArray.push_back(snode);
}

void effectHandler::addNodeToDepthPass(irr::scene::ISceneNode *node)
{
	if(DepthPassArray.linear_search(node) == -1)
		DepthPassArray.push_back(node);
}

void effectHandler::removeNodeFromDepthPass(irr::scene::ISceneNode *node)
{
	s32 i = DepthPassArray.linear_search(node);
	
	if(i != -1) 
		DepthPassArray.erase(i);
}

void effectHandler::removeShadowFromNode(irr::scene::ISceneNode *node)
{
	u32 i = 0;
	while(shadowNodeArray[i].node != node && i < shadowNodeArray.size())
		i++;
	
	if(i < shadowNodeArray.size()) 
	{
		for(u32 g = 0;g < shadowNodeArray[i].node->getMaterialCount();++g)
			shadowNodeArray[i].node->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)shadowNodeArray[i].materialTypes[g];	
			
		shadowNodeArray.erase(i);
	}
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

void effectHandler::update(irr::video::ITexture* outputTarget)
{
	if(shadowsUnsupported)
		return;

	if(!shadowNodeArray.empty())
	{
	driver->setTransform(ETS_VIEW, lightCam->getViewMatrix());
	driver->setTransform(ETS_PROJECTION, lightCam->getProjectionMatrix());
	*ProjLink = lightCam->getProjectionMatrix();
	*ViewLink = lightCam->getViewMatrix();

	driver->setRenderTarget(ShadowMapTex,true,true,SColor(255,255,255,255));
	
	// Set max distance constant for depth shader.
	*depthLink = lightCam->getFarValue();

	for(u32 i = 0;i < shadowNodeArray.size();++i)
	{
		if(shadowNodeArray[i].shadowMode != ESM_RECEIVE)
		{
			if(shadowNodeArray[i].materialTypes[0] == Wiggle)
				shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)DepthWiggle);
			else
				shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)Depth);

			shadowNodeArray[i].node->render();
		}
	}
	
	for(u32 i = 0;i < shadowNodeArray.size();++i)
	{
		if(shadowNodeArray[i].shadowMode == ESM_CAST)
		{
				for(u32 g = 0;g < shadowNodeArray[i].node->getMaterialCount();++g)
					shadowNodeArray[i].node->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)shadowNodeArray[i].materialTypes[g];	
		}
		else
		{
			if(shadowNodeArray[i].materialTypes[0] == EMT_DETAIL_MAP || shadowNodeArray[i].materialTypes[0] == EMT_LIGHTMAP)
			{
				switch(shadowNodeArray[i].filterType)
				{
				case EFT_NONE:
					shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)Detail);
					break;
				case EFT_4PCF:
					shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)DetailPCF4);
					break;
				case EFT_8PCF:
					shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)DetailPCF8);
					break;
				case EFT_12PCF:
					shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)DetailPCF12);
					break;
				case EFT_16PCF:
					shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)DetailPCF16);
					break;
				}
			}
			else
			{
				switch(shadowNodeArray[i].filterType)
				{
				case EFT_NONE:
					shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)Solid);
					break;
				case EFT_4PCF:
					shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)SolidPCF4);
					break;
				case EFT_8PCF:
					shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)SolidPCF8);
					break;
				case EFT_12PCF:
					shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)SolidPCF12);
					break;
				case EFT_16PCF:
					shadowNodeArray[i].node->setMaterialType((E_MATERIAL_TYPE)SolidPCF16);
					break;
				}
			}
		}
	}
	}

	
	if(!PostProcessingRoutines.empty())
	{
		bool Alter = 0;

		driver->setRenderTarget(ScreenRTT,true,true,ClearColour);
		smgr->drawAll();
		ScreenQuad->getMaterial(0).setTexture(1,ScreenRTT);

		// Perform depth pass after rendering, to ensure animations stay up to date.
		if(DepthPass)
		{
			driver->setRenderTarget(DepthRTT,true,true,SColor(255,255,255,255));
			
			// Set max distance constant for depth shader.
			*depthLink = smgr->getActiveCamera()->getFarValue();

			for(u32 i = 0;i < DepthPassArray.size();++i)
			{
				BufferMaterialList.clear();

				for(u32 g = 0;g < DepthPassArray[i]->getMaterialCount();++g)
					BufferMaterialList.push_back(DepthPassArray[i]->getMaterial(g).MaterialType);

				DepthPassArray[i]->setMaterialType((E_MATERIAL_TYPE)Depth);

				DepthPassArray[i]->render();

				for(u32 g = 0;g < DepthPassArray[i]->getMaterialCount();++g)
					DepthPassArray[i]->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[g];	
			
			}
		}

		if(DepthRTT != 0)
			ScreenQuad->getMaterial(0).setTexture(2,DepthRTT);

		for(u32 i = 0;i < PostProcessingRoutines.size();++i)
		{
			ScreenQuad->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)PostProcessingRoutines[i];
			Alter = !Alter;

			if(i == 0)
				ScreenQuad->getMaterial(0).setTexture(0,ScreenRTT);
			else
				ScreenQuad->getMaterial(0).setTexture(0,ScreenQuad->rt[int(!Alter)]);

			if(i == PostProcessingRoutines.size() - 1)
				driver->setRenderTarget(outputTarget,true,true,ClearColour);
			else
				driver->setRenderTarget(ScreenQuad->rt[int(Alter)],true,true,ClearColour);

			ScreenQuad->render();
		}
	}
	else
	{
		driver->setRenderTarget(outputTarget,true,true,ClearColour);
		smgr->drawAll();
	}

	for(u32 i = 0;i < shadowNodeArray.size();++i)
		for(u32 g = 0;g < shadowNodeArray[i].node->getMaterialCount();++g)
			shadowNodeArray[i].node->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)shadowNodeArray[i].materialTypes[g];	
}

void effectHandler::setMaxShadowDistanceFromLight(irr::f32 distance)
{
	lightCam->setFarValue((f32)distance);
	maxdist = distance;
}

void effectHandler::setShadowDarkness(irr::f32 shadalpha)
{
	shaddark = shadalpha;
}

const c8* ScreenQuadVGLSL = 
"void main() \n" 
"{\n" 
"	gl_Position = vec4(gl_Vertex.xy,0.0,1.0); \n" 
"	gl_TexCoord[0].x = 0.5 * (1.0 + gl_Vertex.x - (1.0 / float(SCREENX))); \n" 
"	gl_TexCoord[0].y = 0.5 * (1.0 + gl_Vertex.y - (1.0 / float(SCREENY))); \n" 
"} \n";

const c8* ScreenQuadVHLSL = 
"struct VS_OUTPUT"
"{"
"	float4 Position				: POSITION0;"
"	float2 TexCoords            : TEXCOORD0;"
"};"
"VS_OUTPUT vertexMain( "
"  	float3 Position	: POSITION0"
"  )"
"{"
"	VS_OUTPUT  OUT;"
"   OUT.Position = float4(Position.x,Position.y,0.0,1.0);"
"	OUT.TexCoords.x = 0.5 * (1 + Position.x - (1 / SCREENX)); \n" 
"	OUT.TexCoords.y = -0.5 * (1 + Position.y - (1 / SCREENY)); \n" 
"	return(OUT); \n"
"} \n";


void effectHandler::addPostProcessingEffectFromFile(irr::core::stringc filename)
{
	CShaderPreprocessor sPP(driver);

	sPP.addShaderDefine("SCREENX",ScreenRTTSize.Width);
	sPP.addShaderDefine("SCREENY",ScreenRTTSize.Height);

	ScreenQuadCB* SQCB = new ScreenQuadCB();
	
	video::E_VERTEX_SHADER_TYPE VertexLevel = driver->queryFeature(video::EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;
	video::E_PIXEL_SHADER_TYPE PixelLevel = driver->queryFeature(video::EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

	const c8* VShad = (driver->getDriverType() == EDT_DIRECT3D9) ? ScreenQuadVHLSL : ScreenQuadVGLSL;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	s32 PostMat = gpu->addHighLevelShaderMaterial(
	sPP.ppShader(VShad).c_str(),	"vertexMain", VertexLevel,
	sPP.ppShaderFF(filename.c_str()).c_str(), "pixelMain", PixelLevel,
	SQCB, video::EMT_SOLID);

	this->addPostProcessingEffect(PostMat);

	SQCB->drop();
}

// Copyright (C) Ahmed Hilali 2008