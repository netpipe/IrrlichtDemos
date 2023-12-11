#ifndef H_XEFFECTS_CB
#define H_XEFFECTS_CB

#include "effectWrapper.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

class DepthShaderCB : public video::IShaderConstantSetCallBack
{
public:
	DepthShaderCB(effectHandler* effectIn) : effect(effectIn), PassTimer(false) {};
	
	virtual void OnSetMaterial(const irr::video::SMaterial& material)
	{
		PassTimer = ((s32)material.MaterialType) == effect->getEffectMaterialType(EET_MRWIGGLE);
	}

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		
		services->setVertexShaderConstant("MaxD", &FarLink, 1);

		if(PassTimer)
		{
			f32 currtime = (f32)effect->getTime();
			services->setVertexShaderConstant("Timer", reinterpret_cast<f32*>(&currtime), 1);
		}
	}

	effectHandler* effect;
	bool PassTimer;
	f32 FarLink;
	core::matrix4 worldViewProj;
};

class ShadowShaderCB : public video::IShaderConstantSetCallBack
{
public:
	ShadowShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	virtual void OnSetMaterial(const SMaterial& material) {}

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

		matrix4 worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = ProjLink;			
		worldViewProj *= ViewLink;
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj2", worldViewProj.pointer(), 16);

		driver->getTransform(video::ETS_WORLD).getInverse(invWorld);
		vector3df lightPosOS = LightLink;
		invWorld.transformVect(lightPosOS); 
		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS.X), 4);
		
		services->setVertexShaderConstant("MaxD", reinterpret_cast<f32*>(&FarLink), 1);

		services->setPixelShaderConstant("LightColour", reinterpret_cast<f32*>(&LightColour.r), 4);
		
	}

	effectHandler* effect;
	core::matrix4 invWorld;

	video::SColorf LightColour;
	core::matrix4 ProjLink;
	core::matrix4 ViewLink;
	core::vector3df LightLink;
	f32 FarLink;
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

		vector3df lightPosOS;
		
		if(driver->getDynamicLightCount() > 0)
		{
			lightPosOS = driver->getDynamicLight(0).Position;
			invWorld.transformVect(lightPosOS); 
		}

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

		vector3df lightPosOS;
		
		if(driver->getDynamicLightCount() > 0)
		{
			lightPosOS = driver->getDynamicLight(0).Position;
			invWorld.transformVect(lightPosOS); 
		}

		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS), 4);

		f32 currtime = effect->getTime();
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

		vector3df lightPosOS;
		
		if(driver->getDynamicLightCount() > 0)
		{
			lightPosOS = driver->getDynamicLight(0).Position;
			invWorld.transformVect(lightPosOS); 
		}

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

		vector3df lightPosOS;
		
		if(driver->getDynamicLightCount() > 0)
		{
			lightPosOS = driver->getDynamicLight(0).Position;
			invWorld.transformVect(lightPosOS); 
		}

		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS.X), 4);

		vector3df eyePos;
		
		if(effect->getActiveSceneManager()->getActiveCamera())
		{
			eyePos = effect->getActiveSceneManager()->getActiveCamera()->getPosition();
		}

		services->setVertexShaderConstant("CamPos", reinterpret_cast<f32*>(&eyePos.X), 4);
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

		vector3df lightPosOS;
		
		if(driver->getDynamicLightCount() > 0)
		{
			lightPosOS = driver->getDynamicLight(0).Position;
			invWorld.transformVect(lightPosOS);
		}

		services->setVertexShaderConstant("LightPos", reinterpret_cast<f32*>(&lightPosOS.X), 4);
		
		vector3df eyePos;
		
		if(effect->getActiveSceneManager()->getActiveCamera())
		{
			eyePos = effect->getActiveSceneManager()->getActiveCamera()->getPosition();
		}

		services->setVertexShaderConstant("CamPos", reinterpret_cast<f32*>(&eyePos.X), 4);

		
		if(services->getVideoDriver()->getDriverType() == EDT_OPENGL)
		{
			int texVar = 0;
			services->setPixelShaderConstant("hmapSampler", (float*)(&texVar), 1); 

			texVar = 1;
			services->setPixelShaderConstant("nmapSampler", (float*)(&texVar), 1); 
		}
	}
};

#endif