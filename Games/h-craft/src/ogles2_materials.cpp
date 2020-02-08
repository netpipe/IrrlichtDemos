// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "ogles2_materials.h"

#ifdef HC1_OGLES

#include <cassert>

using namespace irr;
using namespace video;

namespace hc1
{

COGLES2MaterialBaseCB::COGLES2MaterialBaseCB() :
	UseDynamicLight(true),
	FirstUpdateBase(true), WVPMatrixID(-1), WVMatrixID(-1), NMatrixID(-1), GlobalAmbientID(-1), MaterialAmbientID(-1), MaterialDiffuseID(-1), MaterialEmissiveID(-1), MaterialSpecularID(-1), MaterialShininessID(-1), LightCountID(-1),
	LightPositionID(-1), LightAttenuationID(-1), LightDiffuseID(-1), LightSpecularID(-1),
	ThicknessID(-1), LightEnable(false), MaterialAmbient(SColorf(0.f, 0.f, 0.f)), MaterialDiffuse(SColorf(0.f, 0.f, 0.f)), MaterialEmissive(SColorf(0.f, 0.f, 0.f)), MaterialSpecular(SColorf(0.f, 0.f, 0.f)),
	MaterialShininess(0.f), Thickness(1.f)
{
	for (u32 i = 0; i < HC1_MAX_SHADER_LIGHTS; ++i)
	{
		LightPosition[i] = core::vector3df(0.f, 0.f, 0.f);
		LightAttenuation[i] = 1.f;
		LightDiffuse[i] = SColorf(0.f, 0.f, 0.f);
		LightSpecular[i] = SColorf(0.f, 0.f, 0.f);
	}
}

void COGLES2MaterialBaseCB::OnSetMaterial(const SMaterial& material)
{
	LightEnable = material.Lighting && UseDynamicLight;
	MaterialAmbient = SColorf(material.AmbientColor);
	MaterialDiffuse = SColorf(material.DiffuseColor);
	MaterialEmissive = SColorf(material.EmissiveColor);
	MaterialSpecular = SColorf(material.SpecularColor);
	MaterialShininess = material.Shininess;

	Thickness = (material.Thickness > 0.f) ? material.Thickness : 1.f;
}

void COGLES2MaterialBaseCB::OnSetConstants(IMaterialRendererServices* services, s32 userData)
{
	IVideoDriver* driver = services->getVideoDriver();

	if (FirstUpdateBase)
	{
		WVPMatrixID = services->getVertexShaderConstantID("uWVPMatrix");
		WVMatrixID = services->getVertexShaderConstantID("uWVMatrix");
		NMatrixID = services->getVertexShaderConstantID("uNMatrix");
		GlobalAmbientID = services->getVertexShaderConstantID("uGlobalAmbient");
		MaterialAmbientID = services->getVertexShaderConstantID("uMaterialAmbient");
		MaterialDiffuseID = services->getVertexShaderConstantID("uMaterialDiffuse");
		MaterialEmissiveID = services->getVertexShaderConstantID("uMaterialEmissive");
		MaterialSpecularID = services->getVertexShaderConstantID("uMaterialSpecular");
		MaterialShininessID = services->getVertexShaderConstantID("uMaterialShininess");
		LightCountID = services->getVertexShaderConstantID("uLightCount");
		LightPositionID = services->getVertexShaderConstantID("uLightPosition");
		LightAttenuationID = services->getVertexShaderConstantID("uLightAttenuation");
		LightDiffuseID = services->getVertexShaderConstantID("uLightDiffuse");
		LightSpecularID = services->getVertexShaderConstantID("uLightSpecular");
		//ThicknessID = services->getVertexShaderConstantID("uThickness");	// no need currently

		FirstUpdateBase = false;
	}

	const core::matrix4 W = driver->getTransform(ETS_WORLD);
	const core::matrix4 V = driver->getTransform(ETS_VIEW);
	const core::matrix4 P = driver->getTransform(ETS_PROJECTION);

	core::matrix4 Matrix = P * V * W;
	services->setPixelShaderConstant(WVPMatrixID, Matrix.pointer(), 16);

	Matrix = V * W;
	services->setPixelShaderConstant(WVMatrixID, Matrix.pointer(), 16);

	Matrix.makeInverse();
	services->setPixelShaderConstant(NMatrixID, Matrix.getTransposed().pointer(), 16);

	s32 LightCount = LightEnable ? driver->getDynamicLightCount() : 0;
	services->setPixelShaderConstant(LightCountID, &LightCount, 1);

	if (LightCount > 0)
	{
		if (LightCount > HC1_MAX_SHADER_LIGHTS)
			LightCount = HC1_MAX_SHADER_LIGHTS;

		video::SColorf globalAmbient(driver->getAmbientLight());
		services->setVertexShaderConstant(GlobalAmbientID, reinterpret_cast<f32*>(&globalAmbient), 4);

		services->setPixelShaderConstant(MaterialAmbientID, reinterpret_cast<f32*>(&MaterialAmbient), 4);
		services->setPixelShaderConstant(MaterialDiffuseID, reinterpret_cast<f32*>(&MaterialDiffuse), 4);
		services->setPixelShaderConstant(MaterialEmissiveID, reinterpret_cast<f32*>(&MaterialEmissive), 4);
		services->setPixelShaderConstant(MaterialSpecularID, reinterpret_cast<f32*>(&MaterialSpecular), 4);
		services->setPixelShaderConstant(MaterialShininessID, &MaterialShininess, 1);

		Matrix = V;

		// HACK: Lights are the most expensive parts in the shader calculations.
		// We can save some time if the shaders know which light-type is at which index.
		// So for this game we say there are always 2 diffuse lights and 1-2 specular lights.
		// And then we order diffuse lights to be at the front
		s32 diffuseIdx = 0;
		s32 specularIdx = 2;

		for (s32 i = 0; i < LightCount; ++i)
		{
			SLight CurrentLight = driver->getDynamicLight(i);
/*			else
			{
				const SColorf black(0.f, 0.f, 0.f);
				CurrentLight.DiffuseColor = black;
				CurrentLight.SpecularColor = black;
				CurrentLight.CastShadows = false;
			} */

			Matrix.transformVect(CurrentLight.Position);

			if (CurrentLight.SpecularColor.getRed() > 0 || CurrentLight.SpecularColor.getGreen() > 0 || CurrentLight.SpecularColor.getBlue() > 0 )
			{
				LightPosition[specularIdx] = CurrentLight.Position;
				LightAttenuation[specularIdx] = CurrentLight.Attenuation.Y;
				LightDiffuse[specularIdx] = CurrentLight.DiffuseColor;
				LightSpecular[specularIdx] = CurrentLight.SpecularColor;
				++ specularIdx;
			}
			else
			{
				LightPosition[diffuseIdx] = CurrentLight.Position;
				LightAttenuation[diffuseIdx] = CurrentLight.Attenuation.Y;
				LightDiffuse[diffuseIdx] = CurrentLight.DiffuseColor;
				LightSpecular[diffuseIdx] = CurrentLight.SpecularColor;
				++ diffuseIdx;
			}
		}

		services->setPixelShaderConstant(LightPositionID, reinterpret_cast<f32*>(LightPosition), 3*HC1_MAX_SHADER_LIGHTS);
		services->setPixelShaderConstant(LightAttenuationID, LightAttenuation, HC1_MAX_SHADER_LIGHTS);
		services->setPixelShaderConstant(LightDiffuseID, reinterpret_cast<f32*>(LightDiffuse), 4*HC1_MAX_SHADER_LIGHTS);
		services->setPixelShaderConstant(LightSpecularID, reinterpret_cast<f32*>(LightSpecular), 4*HC1_MAX_SHADER_LIGHTS);
	}

	//services->setPixelShaderConstant(ThicknessID, &Thickness, 1);	// no need currently
}

void COGLES2MaterialBaseCB::SetUseDynamicLight(bool enable)
{
	UseDynamicLight = enable;
}

bool COGLES2MaterialBaseCB::GetUseDynamicLight() const
{
	return UseDynamicLight;
}

COGLES2MaterialSolidCB::COGLES2MaterialSolidCB() :
	FirstUpdate(true), TMatrix0ID(-1), AlphaRefID(-1), TextureUnit0ID(-1), AlphaRef(0.5f), TextureUnit0(0)
{
}

void COGLES2MaterialSolidCB::OnSetMaterial(const SMaterial& material)
{
	COGLES2MaterialBaseCB::OnSetMaterial(material);

	AlphaRef = material.MaterialTypeParam;
}

void COGLES2MaterialSolidCB::OnSetConstants(IMaterialRendererServices* services, s32 userData)
{
	COGLES2MaterialBaseCB::OnSetConstants(services, userData);

	IVideoDriver* driver = services->getVideoDriver();

	if (FirstUpdate)
	{
		TMatrix0ID = services->getVertexShaderConstantID("uTMatrix0");
		AlphaRefID = services->getVertexShaderConstantID("uAlphaRef");
		TextureUnit0ID = services->getVertexShaderConstantID("uTextureUnit0");

		FirstUpdate = false;
	}

	core::matrix4 Matrix = driver->getTransform(ETS_TEXTURE_0);
	services->setPixelShaderConstant(TMatrix0ID, Matrix.pointer(), 16);

	services->setPixelShaderConstant(AlphaRefID, &AlphaRef, 1);
	services->setPixelShaderConstant(TextureUnit0ID, &TextureUnit0, 1);
}

COGLES2MaterialLightmapCB::COGLES2MaterialLightmapCB() :
	FirstUpdate(true), TextureUnit0ID(-1), TextureUnit1ID(-1),
	TextureUnit0(0), TextureUnit1(1)
{
}

void COGLES2MaterialLightmapCB::OnSetMaterial(const SMaterial& material)
{
	COGLES2MaterialBaseCB::OnSetMaterial(material);
}

void COGLES2MaterialLightmapCB::OnSetConstants(IMaterialRendererServices* services, s32 userData)
{
	COGLES2MaterialBaseCB::OnSetConstants(services, userData);

	if (FirstUpdate)
	{
		TextureUnit0ID = services->getVertexShaderConstantID("uTextureUnit0");
		TextureUnit1ID = services->getVertexShaderConstantID("uTextureUnit1");

		FirstUpdate = false;
	}

	services->setPixelShaderConstant(TextureUnit0ID, &TextureUnit0, 1);
	services->setPixelShaderConstant(TextureUnit1ID, &TextureUnit1, 1);
}

COGLES2MaterialTransparentAlphaChannelCB::COGLES2MaterialTransparentAlphaChannelCB()
: COGLES2MaterialSolidCB()
{
}

void COGLES2MaterialTransparentAlphaChannelCB::OnSetMaterial(const SMaterial& material)
{
	COGLES2MaterialSolidCB::OnSetMaterial(material);
}

void COGLES2MaterialTransparentAlphaChannelCB::OnSetConstants(IMaterialRendererServices* services, s32 userData)
{
	COGLES2MaterialSolidCB::OnSetConstants(services, userData);
}

COGLES2MaterialTransparentAddColorCB::COGLES2MaterialTransparentAddColorCB()
: COGLES2MaterialSolidCB()
{
}

void COGLES2MaterialTransparentAddColorCB::OnSetMaterial(const SMaterial& material)
{
	COGLES2MaterialSolidCB::OnSetMaterial(material);
}

void COGLES2MaterialTransparentAddColorCB::OnSetConstants(IMaterialRendererServices* services, s32 userData)
{
	COGLES2MaterialSolidCB::OnSetConstants(services, userData);
}

COGLES2MaterialTransparentVertexAlphaCB::COGLES2MaterialTransparentVertexAlphaCB()
: COGLES2MaterialSolidCB()
{
}

void COGLES2MaterialTransparentVertexAlphaCB::OnSetMaterial(const SMaterial& material)
{
	COGLES2MaterialSolidCB::OnSetMaterial(material);
}

void COGLES2MaterialTransparentVertexAlphaCB::OnSetConstants(IMaterialRendererServices* services, s32 userData)
{
	COGLES2MaterialSolidCB::OnSetConstants(services, userData);
}

}; // namespace hc1

#endif // HC1_OGLES
