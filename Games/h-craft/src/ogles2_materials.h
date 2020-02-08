// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef OGLES_MATERIALS_H
#define OGLES_MATERIALS_H

#include "compile_config.h"

#ifdef HC1_OGLES

#include <irrlicht.h>

// NOTE: This stuff is basically copied from Irrlicht's COGLES2FixedPipelineRenderer (I just wanted minor modifications).
// This is also why it uses the Irrlicht coding style here (easier to merge fixes keeping that)
// Just put in another namespace to avoid conflicts.

namespace hc1
{

#define HC1_MAX_SHADER_LIGHTS 	4	// must be the same as MAX_LIGHTS define in the shader

class COGLES2MaterialBaseCB : public irr::video::IShaderConstantSetCallBack
{
public:
	COGLES2MaterialBaseCB();

	virtual void OnSetMaterial(const irr::video::SMaterial& material);
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

	// Can disable dynamic light in the shader even if the material enables it.
	// Does not work the other way round - if the material disables it then there is no dynamic light.
	// Default is enabled.
	void SetUseDynamicLight(bool enable);
	bool GetUseDynamicLight() const;

protected:
	bool UseDynamicLight;

	bool FirstUpdateBase;

	irr::s32 WVPMatrixID;
	irr::s32 WVMatrixID;
	irr::s32 NMatrixID;

	irr::s32 GlobalAmbientID;
	irr::s32 MaterialAmbientID;
	irr::s32 MaterialDiffuseID;
	irr::s32 MaterialEmissiveID;
	irr::s32 MaterialSpecularID;
	irr::s32 MaterialShininessID;

	irr::s32 LightCountID;
	irr::s32 LightPositionID;
	irr::s32 LightAttenuationID;
	irr::s32 LightDiffuseID;
	irr::s32 LightSpecularID;

	irr::s32 ThicknessID;

	bool LightEnable;
	irr::video::SColorf GlobalAmbient;
	irr::video::SColorf MaterialAmbient;
	irr::video::SColorf MaterialDiffuse;
	irr::video::SColorf MaterialEmissive;
	irr::video::SColorf MaterialSpecular;
	irr::f32 MaterialShininess;

	irr::core::vector3df LightPosition[HC1_MAX_SHADER_LIGHTS];
	irr::f32 LightAttenuation[HC1_MAX_SHADER_LIGHTS];
	irr::video::SColorf LightDiffuse[HC1_MAX_SHADER_LIGHTS];
	irr::video::SColorf LightSpecular[HC1_MAX_SHADER_LIGHTS];

	irr::f32 Thickness;
};

class COGLES2MaterialSolidCB : public COGLES2MaterialBaseCB
{
public:
	COGLES2MaterialSolidCB();

	virtual void OnSetMaterial(const irr::video::SMaterial& material);
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

protected:
	bool FirstUpdate;

	irr::s32 TMatrix0ID;
	irr::s32 AlphaRefID;
	irr::s32 TextureUnit0ID;

	irr::f32 AlphaRef;
	irr::s32 TextureUnit0;
};

class COGLES2MaterialLightmapCB : public COGLES2MaterialBaseCB
{
public:
	COGLES2MaterialLightmapCB();

	virtual void OnSetMaterial(const irr::video::SMaterial& material);
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

protected:
	bool FirstUpdate;

	irr::s32 TextureUnit0ID;
	irr::s32 TextureUnit1ID;

	irr::s32 TextureUnit0;
	irr::s32 TextureUnit1;
};

class COGLES2MaterialTransparentAlphaChannelCB : public COGLES2MaterialSolidCB
{
public:
	COGLES2MaterialTransparentAlphaChannelCB();

	virtual void OnSetMaterial(const irr::video::SMaterial& material);
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

protected:
};

class COGLES2MaterialTransparentAddColorCB: public COGLES2MaterialSolidCB
{
public:
	COGLES2MaterialTransparentAddColorCB();

	virtual void OnSetMaterial(const irr::video::SMaterial& material);
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

protected:
};

class COGLES2MaterialTransparentVertexAlphaCB: public COGLES2MaterialSolidCB
{
public:
	COGLES2MaterialTransparentVertexAlphaCB();

	virtual void OnSetMaterial(const irr::video::SMaterial& material);
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

protected:
};


}; // namespace hc1

#endif // HC1_OGLES

#endif // OGLES_MATERIALS_H
