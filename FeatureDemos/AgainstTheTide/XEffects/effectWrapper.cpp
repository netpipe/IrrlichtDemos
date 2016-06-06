#include "effectWrapper.h"
#include "effectWrapperCB.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

effectHandler::effectHandler(IrrlichtDevice* dev, dimension2d<s32> mapSize, core::stringc shaderFolder, irr::core::dimension2d<irr::s32> screenRTTSize)
: device(dev), smgr(dev->getSceneManager()), driver(dev->getVideoDriver()),
ScreenRTTSize((screenRTTSize == dimension2d<s32>(0,0)) ? dev->getVideoDriver()->getScreenSize() : screenRTTSize),
ClearColour(0x0), shadowsUnsupported(false), DepthRTT(0), DepthPass(false), depthMC(0), shadowMC(0),
AmbientColour(0x0), layerToExclude(-1)
{
	CShaderPreprocessor sPP(driver);
	sPP.addShaderDefine("MAPRES", core::stringc(mapSize.Height));
	
	bool tempTexFlagMipMaps = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool tempTexFlag32 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);

	ShadowMapTex = driver->createRenderTargetTexture(mapSize);
	ScreenRTT = driver->createRenderTargetTexture(ScreenRTTSize);
	ScreenQuad.rt[0] = driver->createRenderTargetTexture(ScreenRTTSize);
	ScreenQuad.rt[1] = driver->createRenderTargetTexture(ScreenRTTSize);

	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, tempTexFlagMipMaps);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, tempTexFlag32);

	stringc platformExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";

	if(shaderFolder.size() > 0 && shaderFolder[shaderFolder.size() - 1] != '/')
		shaderFolder += "/";

	// For shadows
	stringc DFNV = shaderFolder + stringc("ShadowPass1V") + platformExt;
	stringc DFNP = shaderFolder + stringc("ShadowPass1P") + platformExt;
	stringc WFNP = shaderFolder + stringc("WhiteWashP") + platformExt;

	stringc SDFNV = shaderFolder + stringc("ShadowPass2V") + platformExt;
	stringc SDFNP = shaderFolder + stringc("ShadowPass2P") + platformExt;
	
	stringc GOOCHV = shaderFolder + stringc("ShaderGoochPtV") + platformExt;
	stringc WIGGLEV = shaderFolder + stringc("ShaderMrWiggleV") + platformExt;
	stringc DEPTHWIGGLEV = shaderFolder + stringc("ShaderDepthWiggleV") + platformExt;
	stringc ANISOV = shaderFolder + stringc("ShaderAnisoV") + platformExt;
	stringc PHONGV = shaderFolder + stringc("ShaderPhongV") + platformExt;
	stringc BRDFV = shaderFolder + stringc("ShaderBRDFV") + platformExt;

	stringc GOOCHP = shaderFolder + stringc("ShaderGoochPtP") + platformExt;
	stringc WIGGLEP = shaderFolder + stringc("ShaderMrWiggleP") + platformExt;
	stringc ANISOP = shaderFolder + stringc("ShaderAnisoP") + platformExt;
	stringc PHONGP = shaderFolder + stringc("ShaderPhongP") + platformExt;
	stringc BRDFP = shaderFolder + stringc("ShaderBRDFP") + platformExt;

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

				Depth = gpu->addHighLevelShaderMaterialFromFiles(
					DFNV.c_str(), "vertexMain", video::EVST_VS_2_0,
					DFNP.c_str(), "pixelMain", video::EPST_PS_2_0,
					depthMC, video::EMT_SOLID);
				
				DepthWiggle = gpu->addHighLevelShaderMaterialFromFiles(
					DEPTHWIGGLEV.c_str(), "vertexMain", video::EVST_VS_2_0,
					DFNP.c_str(), "pixelMain", video::EPST_PS_2_0,
					depthMC, video::EMT_SOLID);

				WhiteWash = gpu->addHighLevelShaderMaterialFromFiles(
					DFNV.c_str(), "vertexMain", video::EVST_VS_2_0,
					WFNP.c_str(), "pixelMain", video::EPST_PS_2_0,
					depthMC, video::EMT_SOLID);

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

				sPP.addShaderDefine("SAMPLE_AMOUNT", core::stringc(8));
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
					shaderFolder + stringc("LightModulate") + platformExt);

				// Simple present.
				Simple = obtainScreenQuadMaterialFromFile(
					shaderFolder + stringc("SimpleP") + platformExt, EMT_TRANSPARENT_ADD_COLOR);
			}
			else
			{
				Depth = EMT_SOLID;
				DepthWiggle = EMT_SOLID;
				WhiteWash = EMT_SOLID;

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
	PostProcessingRoutines.push_back(MaterialType);
}

void effectHandler::addShadowToNode(irr::scene::ISceneNode *node, E_FILTER_TYPE filterType, E_SHADOW_MODE shadowMode)
{
	SShadowNode snode = {node, shadowMode, filterType};
	ShadowNodeArray.push_back(snode);
}

void effectHandler::addNodeToDepthPass(irr::scene::ISceneNode *node)
{
	if(DepthPassArray.binary_search(node) == -1)
		DepthPassArray.push_back(node);
}

void effectHandler::removeNodeFromDepthPass(irr::scene::ISceneNode *node)
{
	s32 i = DepthPassArray.binary_search(node);
	
	if(i != -1) 
		DepthPassArray.erase(i);
}

void effectHandler::removeShadowFromNode(irr::scene::ISceneNode *node)
{
	u32 i = 0;
	while(ShadowNodeArray[i].node != node && i < ShadowNodeArray.size())
		i++;
	
	if(i < ShadowNodeArray.size()) 
		ShadowNodeArray.erase(i);
}

void effectHandler::addEffectToNode(ISceneNode* node, E_EFFECT_TYPE etype)
{
	if(etype < EET_COUNT)
		node->setMaterialType((E_MATERIAL_TYPE)Effects[etype]);
}

s32 effectHandler::getEffectMaterialType(E_EFFECT_TYPE etype) const
{
	if(etype < EET_COUNT)
		return Effects[etype];

	return 0;
}

void effectHandler::update(irr::video::ITexture* outputTarget)
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
			
			driver->setRenderTarget(ShadowMapTex, true, true, SColor(0xffffffff));
			
			const u32 ShadowNodeArraySize = ShadowNodeArray.size();
			for(u32 i = 0;i < ShadowNodeArraySize;++i)
			{
				if(ShadowNodeArray[i].shadowMode == ESM_RECEIVE)
					continue;

				BufferMaterialList.set_used(0);

				const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();
				for(u32 m = 0;m < CurrentMaterialCount;++m)
				{
					BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
					ShadowNodeArray[i].node->getMaterial(m).MaterialType =
						ShadowNodeArray[i].node->getMaterial(m).MaterialType == Effects[EET_MRWIGGLE]
						? (E_MATERIAL_TYPE)DepthWiggle : (E_MATERIAL_TYPE)Depth;
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
			
			driver->setRenderTarget(ScreenQuad.rt[1], true, true, SColor(0xffffffff));

			for(u32 i = 0;i < ShadowNodeArraySize;++i)
			{
				const u32 CurrentMaterialCount = ShadowNodeArray[i].node->getMaterialCount();

				BufferMaterialList.reallocate(CurrentMaterialCount);
				BufferTextureList.reallocate(CurrentMaterialCount);
				core::array<bool> BilinearBuffer(CurrentMaterialCount);
				BufferMaterialList.set_used(0);
				BufferTextureList.set_used(0);
				BilinearBuffer.set_used(0);
				
				for(u32 m = 0;m < CurrentMaterialCount;++m)
				{
					BufferMaterialList.push_back(ShadowNodeArray[i].node->getMaterial(m).MaterialType);
					BufferTextureList.push_back(ShadowNodeArray[i].node->getMaterial(m).getTexture(0));
					BilinearBuffer.push_back(ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].BilinearFilter);
				
					if(m == layerToExclude) continue;

					ShadowNodeArray[i].node->getMaterial(m).MaterialType = 
						(ShadowNodeArray[i].shadowMode == ESM_CAST) ? (E_MATERIAL_TYPE)WhiteWash
						: (E_MATERIAL_TYPE)Solid[ShadowNodeArray[i].filterType];

					ShadowNodeArray[i].node->getMaterial(m).setTexture(0, ShadowMapTex);
					ShadowNodeArray[i].node->getMaterial(m).TextureLayer[0].BilinearFilter = false;
				}

				ShadowNodeArray[i].node->OnAnimate(device->getTimer()->getTime());
				ShadowNodeArray[i].node->render();

				for(u32 m = 0;m < CurrentMaterialCount;++m)
				{
					if(m == layerToExclude) continue;

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

	// Perform depth pass after rendering, to ensure animations stay up to date.
	if(DepthPass)
	{
		driver->setRenderTarget(DepthRTT, true, true, SColor(0xffffffff));

		// Set max distance constant for depth shader.
		depthMC->FarLink = smgr->getActiveCamera()->getFarValue();

		for(u32 i = 0;i < DepthPassArray.size();++i)
		{
			BufferMaterialList.set_used(0);

			for(u32 g = 0;g < DepthPassArray[i]->getMaterialCount();++g)
				BufferMaterialList.push_back(DepthPassArray[i]->getMaterial(g).MaterialType);

			DepthPassArray[i]->setMaterialType((E_MATERIAL_TYPE)Depth);
			DepthPassArray[i]->OnAnimate(device->getTimer()->getTime());
			DepthPassArray[i]->render();

			for(u32 g = 0;g < DepthPassArray[i]->getMaterialCount();++g)
				DepthPassArray[i]->getMaterial(g).MaterialType = (E_MATERIAL_TYPE)BufferMaterialList[g];
		}
	}
}

const c8* ScreenQuadVGLSL = 
"void main() \n" 
"{\n" 
"	gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0); \n" 
"	gl_TexCoord[0].x = 0.5 * (1.0 + gl_Vertex.x - (1.0 / float(SCREENX))); \n" 
"	gl_TexCoord[0].y = 0.5 * (1.0 + gl_Vertex.y - (1.0 / float(SCREENY))); \n" 
"} \n";

const c8* ScreenQuadVHLSL = 
"struct VS_OUTPUT"
"{"
"	float4 Position		: POSITION0;"
"	float2 TexCoords	: TEXCOORD0;"
"};"
"VS_OUTPUT vertexMain(float3 Position : POSITION0)"
"{"
"	VS_OUTPUT  OUT;"
"   OUT.Position = float4(Position.x,Position.y, 0.0, 1.0);"
"	OUT.TexCoords.x = 0.5 * (1 + Position.x - (1 / SCREENX)); \n" 
"	OUT.TexCoords.y = -0.5 * (1 + Position.y - (1 / SCREENY)); \n" 
"	return(OUT); \n"
"} \n";


irr::s32 effectHandler::obtainScreenQuadMaterialFromFile(const irr::core::stringc& filename, 
	irr::video::E_MATERIAL_TYPE baseMaterial)
{
	CShaderPreprocessor sPP(driver);

	sPP.addShaderDefine("SCREENX", core::stringc(ScreenRTTSize.Width));
	sPP.addShaderDefine("SCREENY", core::stringc(ScreenRTTSize.Height));

	ScreenQuadCB* SQCB = new ScreenQuadCB();
	
	video::E_VERTEX_SHADER_TYPE VertexLevel = driver->queryFeature(video::EVDF_VERTEX_SHADER_3_0) ? EVST_VS_3_0 : EVST_VS_2_0;
	video::E_PIXEL_SHADER_TYPE PixelLevel = driver->queryFeature(video::EVDF_PIXEL_SHADER_3_0) ? EPST_PS_3_0 : EPST_PS_2_0;

	const c8* VShad = (driver->getDriverType() == EDT_DIRECT3D9) ? ScreenQuadVHLSL : ScreenQuadVGLSL;

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	s32 PostMat = gpu->addHighLevelShaderMaterial(
	sPP.ppShader(VShad).c_str(), "vertexMain", VertexLevel,
	sPP.ppShaderFF(filename.c_str()).c_str(), "pixelMain", PixelLevel,
	SQCB, baseMaterial);
	
	SQCB->drop();

	return PostMat;
}

void effectHandler::addPostProcessingEffectFromFile(const irr::core::stringc& filename)
{
	s32 PostMat = obtainScreenQuadMaterialFromFile(filename);	
	this->addPostProcessingEffect(PostMat);
}

// Copyright (C) Ahmed Hilali 2008