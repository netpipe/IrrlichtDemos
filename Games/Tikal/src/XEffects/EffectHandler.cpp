#include "EffectHandler.h"
#include "EffectCB.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;


EffectHandler::EffectHandler(IrrlichtDevice* dev, const irr::core::stringc& shaderFolder,
	const irr::core::dimension2du& screenRTTSize, const bool useRoundSpotLights)
: device(dev), smgr(dev->getSceneManager()), driver(dev->getVideoDriver()),
ScreenRTTSize(screenRTTSize.getArea() == 0 ? dev->getVideoDriver()->getScreenSize() : screenRTTSize),
ClearColour(0x0), shadowsUnsupported(false), DepthRTT(0), DepthPass(false), depthMC(0), shadowMC(0),
AmbientColour(0x0)
{
	CShaderPreprocessor sPP(driver);

	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	ScreenRTT = driver->addRenderTargetTexture(ScreenRTTSize);
	ScreenQuad.rt[0] = driver->addRenderTargetTexture(ScreenRTTSize);
	ScreenQuad.rt[1] = driver->addRenderTargetTexture(ScreenRTTSize);

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);

	stringc platformExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";

	const stringc shaderDirectory = shaderFolder +
		((shaderFolder.size() > 0 && shaderFolder[shaderFolder.size() - 1] != '/')
		? stringc("/") : stringc(""));

	// For shadows
	stringc DFNV = shaderDirectory + stringc("ShadowPass1V") + platformExt;
	stringc DFNP = shaderDirectory + stringc("ShadowPass1P") + platformExt;
	stringc DFNPT = shaderDirectory + stringc("ShadowPass1PT") + platformExt;
	stringc WFNP = shaderDirectory + stringc("WhiteWashP") + platformExt;

	stringc SDFNV = shaderDirectory + stringc("ShadowPass2V") + platformExt;
	stringc SDFNP = shaderDirectory + stringc("ShadowPass2P") + platformExt;

	stringc GOOCHV = shaderDirectory + stringc("ShaderGoochPtV") + platformExt;
	stringc WIGGLEV = shaderDirectory + stringc("ShaderMrWiggleV") + platformExt;
	stringc DEPTHWIGGLEV = shaderDirectory + stringc("ShaderDepthWiggleV") + platformExt;
	stringc ANISOV = shaderDirectory + stringc("ShaderAnisoV") + platformExt;
	stringc PHONGV = shaderDirectory + stringc("ShaderPhongV") + platformExt;
	stringc BRDFV = shaderDirectory + stringc("ShaderBRDFV") + platformExt;

	stringc GOOCHP = shaderDirectory + stringc("ShaderGoochPtP") + platformExt;
	stringc WIGGLEP = shaderDirectory + stringc("ShaderMrWiggleP") + platformExt;
	stringc ANISOP = shaderDirectory + stringc("ShaderAnisoP") + platformExt;
	stringc PHONGP = shaderDirectory + stringc("ShaderPhongP") + platformExt;
	stringc BRDFP = shaderDirectory + stringc("ShaderBRDFP") + platformExt;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	if(gpu && ((driver->getDriverType() == EDT_OPENGL && driver->queryFeature(EVDF_ARB_GLSL)) ||
			   (driver->getDriverType() == EDT_DIRECT3D9 && driver->queryFeature(EVDF_PIXEL_SHADER_1_4))))
	{
			BRDFShaderCB* mc4 = new BRDFShaderCB(this);
			LightShaderCB* mc5 = new LightShaderCB(this);
			WiggleShaderCB* mc6 = new WiggleShaderCB(this);
			AnisoShaderCB* mc8 = new AnisoShaderCB(this);
			PhongShaderCB* mc9 = new PhongShaderCB(this);

			Effects[EET_BRDF] = gpu->addHighLevelShaderMaterialFromFiles(
				BRDFV.c_str(), "vertexMain", video::EVST_VS_1_1,
				BRDFP.c_str(), "pixelMain", video::EPST_PS_1_4,
				mc4, video::EMT_SOLID);

			Effects[EET_GOOCH] = gpu->addHighLevelShaderMaterialFromFiles(
				GOOCHV.c_str(),	"vertexMain", video::EVST_VS_1_1,
				GOOCHP.c_str(), "pixelMain", video::EPST_PS_1_4,
				mc5, video::EMT_SOLID);

			Effects[EET_MRWIGGLE] = gpu->addHighLevelShaderMaterialFromFiles(
				WIGGLEV.c_str(), "vertexMain", video::EVST_VS_1_1,
				WIGGLEP.c_str(), "pixelMain", video::EPST_PS_1_4,
				mc6, video::EMT_SOLID);

			Effects[EET_ANISO] = gpu->addHighLevelShaderMaterialFromFiles(
				ANISOV.c_str(),	"vertexMain", video::EVST_VS_1_1,
				ANISOP.c_str(), "pixelMain", video::EPST_PS_1_4,
				mc8, video::EMT_SOLID);

			Effects[EET_PHONG] = gpu->addHighLevelShaderMaterialFromFiles(
				PHONGV.c_str(),	"vertexMain", video::EVST_VS_1_1,
				PHONGP.c_str(), "pixelMain", video::EPST_PS_1_4,
				mc9, video::EMT_SOLID);

			if(driver->queryFeature(EVDF_PIXEL_SHADER_2_0) || driver->getDriverType() == EDT_OPENGL)
			{
				depthMC = new DepthShaderCB(this);
				shadowMC = new ShadowShaderCB(this);

				Depth = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(DFNV).c_str(), "vertexMain", video::EVST_VS_2_0,
					sPP.ppShaderFF(DFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					depthMC, video::EMT_SOLID);

				DepthT = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(DFNV).c_str(), "vertexMain", video::EVST_VS_2_0,
					sPP.ppShaderFF(DFNPT).c_str(), "pixelMain", video::EPST_PS_2_0,
					depthMC, video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

				DepthWiggle = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(DEPTHWIGGLEV).c_str(), "vertexMain", video::EVST_VS_2_0,
					sPP.ppShaderFF(DFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					depthMC, video::EMT_SOLID);

				sPP.addShaderDefine("LENGTH_BASED_DEPTH");
				DepthScreen = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(DFNV).c_str(), "vertexMain", video::EVST_VS_2_0,
					sPP.ppShaderFF(DFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					depthMC, video::EMT_SOLID);

				WhiteWash = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(DFNV).c_str(), "vertexMain", video::EVST_VS_2_0,
					sPP.ppShaderFF(WFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					depthMC, video::EMT_SOLID);

				WhiteWashT = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(DFNV).c_str(), "vertexMain", video::EVST_VS_2_0,
					sPP.ppShaderFF(WFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					depthMC, video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

				if(useRoundSpotLights) sPP.addShaderDefine("ROUND_SPOTLIGHTS");

				sPP.addShaderDefine("SAMPLE_AMOUNT", core::stringc(1));
				Solid[EFT_NONE] = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_2_0,
					sPP.ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					shadowMC, video::EMT_SOLID);

				sPP.addShaderDefine("SAMPLE_AMOUNT", core::stringc(4));
				Solid[EFT_4PCF] = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_2_0,
					sPP.ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					shadowMC, video::EMT_SOLID);

				// If we are using round spot lights make this 7 sample, for backwards compatibility with SM 2.0.
				sPP.addShaderDefine("SAMPLE_AMOUNT", core::stringc(useRoundSpotLights ? 7 : 8));
				Solid[EFT_8PCF] = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_2_0,
					sPP.ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_2_0,
					shadowMC, video::EMT_SOLID);

				sPP.addShaderDefine("SAMPLE_AMOUNT", core::stringc(12));
				if(driver->queryFeature(EVDF_PIXEL_SHADER_3_0) || driver->getDriverType() == EDT_OPENGL)
				Solid[EFT_12PCF] = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_3_0,
					sPP.ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_3_0,
					shadowMC, video::EMT_SOLID);
				else
					Solid[EFT_12PCF] = Solid[EFT_8PCF];

				sPP.addShaderDefine("SAMPLE_AMOUNT", core::stringc(16));
				if(driver->queryFeature(EVDF_PIXEL_SHADER_3_0) || driver->getDriverType() == EDT_OPENGL)
				Solid[EFT_16PCF] = gpu->addHighLevelShaderMaterial(
					sPP.ppShaderFF(SDFNV).c_str(),	"vertexMain", video::EVST_VS_3_0,
					sPP.ppShaderFF(SDFNP).c_str(), "pixelMain", video::EPST_PS_3_0,
					shadowMC, video::EMT_SOLID);
				else
					Solid[EFT_16PCF] = Solid[EFT_8PCF];

				// Light modulate.
				LightModulate = obtainScreenQuadMaterialFromFile(
					shaderDirectory + stringc("LightModulate") + platformExt);

				// Simple present.
				Simple = obtainScreenQuadMaterialFromFile(
					shaderDirectory + stringc("SimpleP") + platformExt, EMT_TRANSPARENT_ADD_COLOR);
			}
			else
			{
				Depth = EMT_SOLID;
				DepthT = EMT_SOLID;
				DepthWiggle = EMT_SOLID;
				DepthScreen = EMT_SOLID;
				WhiteWash = EMT_SOLID;
				WhiteWashT = EMT_SOLID;

				for(u32 i = 0;i < EFT_COUNT;++i)
					Solid[i] = EMT_SOLID;

				device->getLogger()->log("XEffects: Shadow maps not supported on this system.");
				shadowsUnsupported = true;
			}

			mc4->drop();
			mc5->drop();
			mc6->drop();
			mc8->drop();
			mc9->drop();
	}
	else
	{
		Depth = EMT_SOLID;
		DepthWiggle = EMT_SOLID;
		WhiteWash = EMT_SOLID;

		for(u32 i = 0;i < EFT_COUNT;++i)
			Solid[i] = EMT_SOLID;

		for(u32 i = 0;i < EET_COUNT;++i)
			Effects[i] = EMT_SOLID;

		device->getLogger()->log("XEffects: Shadow maps not supported on this system.");
		device->getLogger()->log("XEffects: Effects not supported on this system.");
	}
}


EffectHandler::~EffectHandler()
{
	removeAllShadowMapTextures();

	if(ScreenRTT)
		driver->removeTexture(ScreenRTT);

	if(ScreenQuad.rt[0])
		driver->removeTexture(ScreenQuad.rt[0]);

	if(ScreenQuad.rt[1])
		driver->removeTexture(ScreenQuad.rt[1]);

	if(DepthRTT)
		driver->removeTexture(DepthRTT);
}


void EffectHandler::setScreenRenderTargetResolution(const irr::core::dimension2du& resolution)
{
	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	if(ScreenRTT)
		driver->removeTexture(ScreenRTT);

	ScreenRTT = driver->addRenderTargetTexture(resolution);

	if(ScreenQuad.rt[0])
		driver->removeTexture(ScreenQuad.rt[0]);

	ScreenQuad.rt[0] = driver->addRenderTargetTexture(resolution);

	if(ScreenQuad.rt[1])
		driver->removeTexture(ScreenQuad.rt[1]);

	ScreenQuad.rt[1] = driver->addRenderTargetTexture(resolution);

	if(DepthRTT != 0)
	{
		driver->removeTexture(DepthRTT);
		DepthRTT = driver->addRenderTargetTexture(resolution);
	}

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);
}


void EffectHandler::enableDepthPass(bool enableDepthPass)
{
	DepthPass = enableDepthPass;

	if(DepthPass && DepthRTT == 0)
	{
		bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);
		DepthRTT = driver->addRenderTargetTexture(ScreenRTTSize);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);
	}
}


void EffectHandler::addPostProcessingEffect(irr::s32 MaterialType)
{
	PostProcessingRoutines.push_back(MaterialType);
}


void EffectHandler::addShadowToNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType, E_SHADOW_MODE shadowMode)
{
	SShadowNode snode = {node, shadowMode, filterType};
	ShadowNodeArray.push_back(snode);
}


void EffectHandler::addNodeToDepthPass(irr::scene::ISceneNode *node)
{
	if(DepthPassArray.binary_search(node) == -1)
		DepthPassArray.push_back(node);
}


void EffectHandler::removeNodeFromDepthPass(irr::scene::ISceneNode *node)
{
	s32 i = DepthPassArray.binary_search(node);

	if(i != -1)
		DepthPassArray.erase(i);
}


void EffectHandler::addEffectToNode(ISceneNode* node, E_EFFECT_TYPE etype)
{
	if(etype < EET_COUNT)
		node->setMaterialType((E_MATERIAL_TYPE)Effects[etype]);
}


s32 EffectHandler::getEffectMaterialType(E_EFFECT_TYPE etype) const
{
	if(etype < EET_COUNT)
		return Effects[etype];

	return 0;
}


void EffectHandler::update(irr::video::ITexture* outputTarget)
{
	if(shadowsUnsupported || smgr->getActiveCamera() == 0)
		return;

	if(!ShadowNodeArray.empty() && !LightList.empty())
	{
		driver->setRenderTarget(ScreenQuad.rt[0], true, true, AmbientColour);

		ICameraSceneNode* activeCam = smgr->getActiveCamera();
		activeCam->OnAnimate(device->getTimer()->getTime());
		activeCam->OnRegisterSceneNode();
		activeCam->render();

		const u32 LightListSize = LightList.size();
		for(u32 l = 0;l < LightListSize;++l)
		{
			// Set max distance constant for depth shader.
			depthMC->FarLink = LightList[l].getFarValue();

			driver->setTransform(ETS_VIEW, LightList[l].getViewMatrix());
			driver->setTransform(ETS_PROJECTION, LightList[l].getProjectionMatrix());

			ITexture* currentShadowMapTexture = getShadowMapTexture(LightList[l].getShadowMapResolution());
			driver->setRenderTarget(currentShadowMapTexture, true, true, SColor(0xffffffff));

			const u32 ShadowNodeArraySize = ShadowNodeArray.size();
			for(u32 i = 0;i < ShadowNodeArraySize;++i)
			{
				if(ShadowNodeArray[i].shadowMode == ESM_RECEIVE)
					continue;

				const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
				core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
				BufferMaterialList.set_used(0);

				for(u32 m = 0;m < CurrentMaterialCount;++m)
				{
					BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
					ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)
						(BufferMaterialList[m] == Effects[EET_MRWIGGLE]
						? DepthWiggle :
						(BufferMaterialList[m] == video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF ? DepthT : Depth));
				}

				ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
				ShadowNodeArray[i].node->render();

				const u32 BufferMaterialListSize = BufferMaterialList.size();
				for(u32 m = 0;m < BufferMaterialListSize;++m)
					ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
			}

			driver->setTransform(ETS_VIEW, activeCam->getViewMatrix());
			driver->setTransform(ETS_PROJECTION, activeCam->getProjectionMatrix());

			shadowMC->LightColour = LightList[l].getLightColor();
			shadowMC->LightLink = LightList[l].getPosition();
			shadowMC->FarLink = LightList[l].getFarValue();
			shadowMC->ViewLink = LightList[l].getViewMatrix();
			shadowMC->ProjLink = LightList[l].getProjectionMatrix();
			shadowMC->MapRes = (f32)LightList[l].getShadowMapResolution();

			driver->setRenderTarget(ScreenQuad.rt[1], true, true, SColor(0xffffffff));

			for(u32 i = 0;i < ShadowNodeArraySize;++i)
			{
				const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();

				core::array<irr::s32> BufferMaterialList(CurrentMaterialCount);
				core::array<irr::video::ITexture*> BufferTextureList(CurrentMaterialCount);
				core::array<bool> BilinearBuffer(CurrentMaterialCount);
				BufferMaterialList.set_used(0);
				BufferTextureList.set_used(0);
				BilinearBuffer.set_used(0);

				for(u32 m = 0;m < CurrentMaterialCount;++m)
				{
					BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
					BufferTextureList.push_back(ShadowNodeArray[i].node->getMaterial(m).getTexture(0));
					BilinearBuffer.push_back(ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].BilinearFilter);

					if(ShadowNodeArray[i].shadowMode != ESM_CAST)
					{
						ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)Solid[ShadowNodeArray[i].filterType];
						ShadowNodeArray[i].node->getMaterial(m).setTexture(0, currentShadowMapTexture);
						ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].BilinearFilter = false;
					}
					else
					{
						ShadowNodeArray[i].node->getMaterial(m).MaterialType =
							BufferMaterialList[m] == video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF ?
							(E_MATERIAL_TYPE)WhiteWashT : (E_MATERIAL_TYPE)WhiteWash;
					}
				}

				ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
				ShadowNodeArray[i].node->render();

				for(u32 m = 0;m < CurrentMaterialCount;++m)
				{
					ShadowNodeArray[i].node->getMaterial(m).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[m];
					ShadowNodeArray[i].node->getMaterial(m).setTexture(0, BufferTextureList[m]);
					ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].BilinearFilter = BilinearBuffer[m];
				}
			}

			driver->setRenderTarget(ScreenQuad.rt[0], false, false, SColor(0x0));
			ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
			ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)Simple;

			ScreenQuad.render(driver);
		}
	}
	else
	{
		driver->setRenderTarget(ScreenQuad.rt[0], true, true, SColor(0xffffffff));
	}

	driver->setRenderTarget(ScreenQuad.rt[1], true, true, ClearColour);
	smgr->drawAll();

	const u32 PostProcessingRoutinesSize = PostProcessingRoutines.size();

	driver->setRenderTarget(PostProcessingRoutinesSize
		? ScreenRTT : outputTarget, true, true, SColor(0x0));

	ScreenQuad.getMaterial().setTexture(0, ScreenQuad.rt[1]);
	ScreenQuad.getMaterial().setTexture(1, ScreenQuad.rt[0]);

	ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)LightModulate;
	ScreenQuad.render(driver);

	// Perform depth pass after rendering, to ensure animations stay up to date.
	if(DepthPass)
	{
		driver->setRenderTarget(DepthRTT, true, true, SColor(0xffffffff));

		// Set max distance constant for depth shader.
		depthMC->FarLink = smgr->getActiveCamera()->getFarValue();

		for(u32 i = 0;i < DepthPassArray.size();++i)
		{
			core::array<irr::s32> BufferMaterialList(DepthPassArray[i]->getMaterialCount());
			BufferMaterialList.set_used(0);

			for(u32 g = 0;g < DepthPassArray[i]->getMaterialCount();++g)
				BufferMaterialList.push_back(DepthPassArray[i]->getMaterial(g).MaterialType);

			DepthPassArray[i]->setMaterialType((E_MATERIAL_TYPE)DepthScreen);
			DepthPassArray[i]->OnAnimate(device->getTimer()->getTime());
			DepthPassArray[i]->render();

			for(u32 g = 0;g < DepthPassArray[i]->getMaterialCount();++g)
				DepthPassArray[i]->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[g];
		}

		driver->setRenderTarget(0, false, false);
	}

	if(PostProcessingRoutinesSize)
	{
		bool Alter = 0;
		ScreenQuad.getMaterial().setTexture(1, ScreenRTT);
		ScreenQuad.getMaterial().setTexture(2, DepthRTT);
		for(u32 i = 0;i < PostProcessingRoutinesSize;++i)
		{
			ScreenQuad.getMaterial().MaterialType = (E_MATERIAL_TYPE)PostProcessingRoutines[i];

			Alter = !Alter;
			ScreenQuad.getMaterial().setTexture(0, i == 0 ? ScreenRTT : ScreenQuad.rt[int(!Alter)]);
			driver->setRenderTarget(i >= PostProcessingRoutinesSize - 1 ?
				outputTarget : ScreenQuad.rt[int(Alter)], true, true, ClearColour);
			ScreenQuad.render(driver);
		}
	}
}


irr::video::ITexture* EffectHandler::getShadowMapTexture(const irr::u32 resolution)
{
	irr::core::map<irr::u32, irr::video::ITexture*>::Node* shadowMapNode = ShadowMaps.find(resolution);

	if(shadowMapNode)
	{
		return shadowMapNode->getValue();
	}
	else
	{
		bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
		bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

		ITexture* shadowMapTexture = driver->addRenderTargetTexture(dimension2du(resolution, resolution));
		ShadowMaps.set(resolution, shadowMapTexture);

		driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
		driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);

		return shadowMapTexture;
	}
}

irr::video::ITexture* EffectHandler::generateRandomVectorTexture(const irr::core::dimension2du& dimensions,
	const irr::core::stringc& name)
{
	IImage* tmpImage = driver->createImage(irr::video::ECF_A8R8G8B8, dimensions);

	srand(device->getTimer()->getRealTime());

	for(u32 x = 0;x < dimensions.Width;++x)
	{
		for(u32 y = 0;y < dimensions.Height;++y)
		{
			vector3df randVec;

			// Reject vectors outside the unit sphere to get a uniform distribution.
			do {randVec = vector3df((rand()%255) / 255.0f, (rand()%255) / 255.0f, (rand()%255) / 255.0f);}
			while(randVec.getLengthSQ() > 1.0f);

			SColorf randCol(randVec.X, randVec.Y, randVec.Z);
			tmpImage->setPixel(x, y, randCol.toSColor());
		}
	}

	ITexture* randTexture = driver->addTexture(name.c_str(), tmpImage);

	tmpImage->drop();

	return randTexture;
}


void EffectHandler::removeAllShadowMapTextures()
{
	irr::core::map<irr::u32, irr::video::ITexture*>::Iterator mapIter = ShadowMaps.getIterator();

	for(;!mapIter.atEnd();mapIter++) driver->removeTexture((*mapIter).getValue());

	ShadowMaps.clear();
}


const c8* ScreenQuadVGLSL =
"uniform float screenX, screenY; \n"
"uniform vec3 LineStarts0, LineStarts1, LineStarts2, LineStarts3; \n"
"uniform vec3 LineEnds0, LineEnds1, LineEnds2, LineEnds3; \n"
"void main() \n"
"{\n"
"	gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0); \n"
"	vec2 tCoords; \n"
"	tCoords.x = 0.5 * (1.0 + gl_Vertex.x - (1.0 / float(screenX))); \n"
"	tCoords.y = 0.5 * (1.0 + gl_Vertex.y - (1.0 / float(screenY))); \n"
"	gl_TexCoord[0].xy = tCoords; \n"
"	vec3 tLStart = mix(LineStarts0, LineStarts1, tCoords.x); \n"
"	vec3 bLStart = mix(LineStarts2, LineStarts3, tCoords.x); \n"
"	gl_TexCoord[1].xyz = mix(tLStart, bLStart, tCoords.y); \n"
"	vec3 tLEnd = mix(LineEnds0, LineEnds1, tCoords.x); \n"
"	vec3 bLEnd = mix(LineEnds2, LineEnds3, tCoords.x); \n"
"	gl_TexCoord[2].xyz = mix(tLEnd, bLEnd, tCoords.y); \n"
"} \n";


const c8* ScreenQuadVHLSL =
"float screenX, screenY; \n"
"float3 LineStarts0, LineStarts1, LineStarts2, LineStarts3; \n"
"float3 LineEnds0, LineEnds1, LineEnds2, LineEnds3; \n"
"struct VS_OUTPUT \n"
"{"
"	float4 Position		: POSITION0;"
"	float2 TexCoords	: TEXCOORD0;"
"	float3 LStart		: TEXCOORD1;"
"	float3 LEnd			: TEXCOORD2;"
"}; \n"
"VS_OUTPUT vertexMain(float3 Position : POSITION0) \n"
"{ \n"
"	VS_OUTPUT OUT; \n"
"   OUT.Position = float4(Position.x,Position.y, 0.0, 1.0); \n"
"	OUT.TexCoords.x = 0.5 * (1.0 + Position.x - (1 / screenX)); \n"
"	OUT.TexCoords.y = 1.0 - 0.5 * (1.0 + Position.y - (1 / screenY)); \n"
"	float3 tLStart = lerp(LineStarts0, LineStarts1, OUT.TexCoords.x); \n"
"	float3 bLStart = lerp(LineStarts2, LineStarts3, OUT.TexCoords.x); \n"
"	OUT.LStart = lerp(tLStart, bLStart, OUT.TexCoords.y); \n"
"	float3 tLEnd = lerp(LineEnds0, LineEnds1, OUT.TexCoords.x); \n"
"	float3 bLEnd = lerp(LineEnds2, LineEnds3, OUT.TexCoords.x); \n"
"	OUT.LEnd = lerp(tLEnd, bLEnd, OUT.TexCoords.y); \n"
"	return(OUT); \n"
"} \n";


irr::s32 EffectHandler::obtainScreenQuadMaterialFromFile(const irr::core::stringc& filename,
	irr::video::E_MATERIAL_TYPE baseMaterial)
{
	CShaderPreprocessor sPP(driver);

	sPP.addShaderDefine("SCREENX", core::stringc(ScreenRTTSize.Width));
	sPP.addShaderDefine("SCREENY", core::stringc(ScreenRTTSize.Height));



	video::E_VERTEX_SHADER_TYPE VertexLevel = driver->queryFeature(video::EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;
	video::E_PIXEL_SHADER_TYPE PixelLevel = driver->queryFeature(video::EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

	const c8* VShad = (driver->getDriverType() == EDT_DIRECT3D9) ? ScreenQuadVHLSL : ScreenQuadVGLSL;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	const stringc shaderString = sPP.ppShaderFF(filename.c_str());

	ScreenQuadCB* SQCB = new ScreenQuadCB(this, true, shaderString.find("mViewProj") != -1);

	s32 PostMat = gpu->addHighLevelShaderMaterial(
	sPP.ppShader(VShad).c_str(), "vertexMain", VertexLevel,
	shaderString.c_str(), "pixelMain", PixelLevel,
	SQCB, baseMaterial);

	SQCB->drop();

	return PostMat;
}


s32 EffectHandler::addPostProcessingEffectFromFile(const irr::core::stringc& filename)
{
	const s32 PostMat = obtainScreenQuadMaterialFromFile(filename);
	this->addPostProcessingEffect(PostMat);

	return PostMat;
}

// Copyright (C) 2007-2009 Ahmed Hilali
