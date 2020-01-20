
#ifndef SHADER_INSTANCE_DEF_H
#define SHADER_INSTANCE_DEF_H

#include "litha_internal.h"

// Used as a key in a map.
struct ShaderInstanceDef
{
	ShaderInstanceDef()
	{
		baseMaterial = video::EMT_SOLID;
	}
	
	ShaderInstanceDef(const c8 *vertexShaderFileName, const c8 *pixelShaderFileName, video::E_MATERIAL_TYPE baseMaterial)
	{
		this->vertexShaderFileName = vertexShaderFileName;
		this->pixelShaderFileName = pixelShaderFileName;
		this->baseMaterial = baseMaterial;
	}
	
	bool operator==(const ShaderInstanceDef &other) const
	{
		return vertexShaderFileName == other.vertexShaderFileName
				&& pixelShaderFileName == other.pixelShaderFileName
				&& baseMaterial == other.baseMaterial;
	}
	
	/*
	bool operator<(const ShaderInstanceDef &other) const
	{
		//return baseMaterial < other.baseMaterial;
		
		return vertexShaderFileName < other.vertexShaderFileName
				&& pixelShaderFileName < other.pixelShaderFileName
				&& baseMaterial < other.baseMaterial;
	}*/
	
	core::stringc vertexShaderFileName;
	core::stringc pixelShaderFileName;
	video::E_MATERIAL_TYPE baseMaterial;
};

#endif

