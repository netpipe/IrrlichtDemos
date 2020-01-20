
#include "Shader.h"
#include "ShaderManager.h"
#include "ShaderInstance.h"

Shader::Shader(ShaderManager *shaderManager, ShaderInstanceDef shaderInstanceDef)
{
	this->shaderManager = shaderManager;
	
	shaderId = shaderManager->RegisterShader(this);
	
	shaderInstance = shaderManager->GetShaderInstance(shaderInstanceDef);
	
	ASSERT( shaderInstance );
	
	callback = NULL;
}

Shader::~Shader()
{
	if (callback)
		callback->drop();
	
	shaderManager->UnregisterShader(this);
}

void Shader::SetVertexRegisterMap(const LowLevelShaderRegisterMap &registerMap)
{
	vertexRegisterMap = registerMap;
}

void Shader::SetPixelRegisterMap(const LowLevelShaderRegisterMap &registerMap)
{
	pixelRegisterMap = registerMap;
}

void Shader::SetVertexConstant(const c8 *name, s32 value)
{
	vertexShaderConstants[core::stringc(name)] = std::vector<f32>(1,s32_As_f32(value));
}

void Shader::SetVertexConstant(const c8 *name, f32 value)
{
	vertexShaderConstants[core::stringc(name)] = std::vector<f32>(1,value);
}

void Shader::SetVertexConstants(const c8 *name, s32 *values, u16 count)
{
	vertexShaderConstants[core::stringc(name)] = valuesToVector((void *)values, count);
}

void Shader::SetVertexConstants(const c8 *name, f32 *values, u16 count)
{
	vertexShaderConstants[core::stringc(name)] = valuesToVector((void *)values, count);
}

void Shader::SetPixelConstant(const c8 *name, s32 value)
{
	pixelShaderConstants[core::stringc(name)] = std::vector<f32>(1,s32_As_f32(value));
}

void Shader::SetPixelConstant(const c8 *name, f32 value)
{
	pixelShaderConstants[core::stringc(name)] = std::vector<f32>(1,value);
}

void Shader::SetPixelConstants(const c8 *name, s32 *values, u16 count)
{
	pixelShaderConstants[core::stringc(name)] = valuesToVector((void *)values, count);
}

void Shader::SetPixelConstants(const c8 *name, f32 *values, u16 count)
{
	pixelShaderConstants[core::stringc(name)] = valuesToVector((void *)values, count);
}

void Shader::SetCallback(IShaderCallback *callback)
{
	SET_REF_COUNTED_POINTER(this->callback, callback)
}

void Shader::ApplyToIrrMaterial(video::SMaterial &material)
{
	material.MaterialType = (video::E_MATERIAL_TYPE)shaderInstance->GetIrrMaterialId();
	
	// Also set to the unique ID of this for setting constants.
	material.MaterialTypeParam2 = *((f32 *)(&shaderId));
}

void Shader::ClearFromIrrMaterial(video::SMaterial &material, video::E_MATERIAL_TYPE newMaterialType)
{
	material.MaterialType = newMaterialType;
	material.MaterialTypeParam2 = 0.0;
}

void Shader::OnSetConstants(video::IMaterialRendererServices *services, s32 userData)
{
	if (callback)
		callback->ShaderOnSetConstants(this);
	
	if (shaderManager->GetShaderLevel() == ESL_HIGH)
	{
		// Set all vertex shader constants
		for (std::map<core::stringc, std::vector<f32> >::const_iterator i = vertexShaderConstants.begin();
				i != vertexShaderConstants.end(); i ++)
		{
			const core::stringc &name = i->first;
			const std::vector<f32> &floats = i->second;
			
			services->setVertexShaderConstant(name.c_str(), &floats[0], floats.size());
		}
		
		// Set all pixel shader constants
		for (std::map<core::stringc, std::vector<f32> >::const_iterator i = pixelShaderConstants.begin();
				i != pixelShaderConstants.end(); i ++)
		{
			const core::stringc &name = i->first;
			const std::vector<f32> &floats = i->second;
			
			services->setPixelShaderConstant(name.c_str(), &floats[0], floats.size());
		}
	}
	else // ESL_LOW, low level shaders.
	{
		// For low level shaders, we set registers, not named variables.
		// LowLevelShaderRegisterMap contains the register -> named variable mappings.
		
		// vertex shaders (using vertexRegisterMap)
		{
			// Just a list of names... which map in to vertexShaderConstants.
			const std::vector<LowLevelShaderRegisterMap::RegisterComponentGroup> &names
					= vertexRegisterMap.GetRegisterComponentNames();
			std::vector<f32> data;
			
			for (u32 i = 0; i < names.size(); i ++)
			{
				LowLevelShaderRegisterMap::RegisterComponentGroup rcg = names[i];
				
				// Name NULL or doesn't exist in named variable map?
				// Unused, just add zeroes.
				if (rcg.name.size() == 0 || vertexShaderConstants.count(rcg.name) == 0)
				{
					for (u32 j = 0; j < rcg.size; j ++)
						data.push_back(0.f);
					
					continue;
				}
				
				const std::vector<f32> &groupFloats = vertexShaderConstants[rcg.name];
				
				// Must match the number of register components!
				ASSERT( groupFloats.size() == rcg.size );
				
				// Add it to the data
				for (u32 j = 0; j < groupFloats.size(); j ++)
					data.push_back(groupFloats[j]);
			}
			
			// Each register has 4 components.
			// This should have been enforced by LowLevelShaderRegisterMap class.
			ASSERT( data.size() % 4 == 0 );
			
			services->setVertexShaderConstant(&(data[0]), 0, data.size() / 4);
		}
		
		// pixel shaders (using pixelRegisterMap)
		// basically copy pasted from above.
		{
			// Just a list of names... which map in to vertexShaderConstants.
			const std::vector<LowLevelShaderRegisterMap::RegisterComponentGroup> &names
					= pixelRegisterMap.GetRegisterComponentNames();
			std::vector<f32> data;
			
			for (u32 i = 0; i < names.size(); i ++)
			{
				LowLevelShaderRegisterMap::RegisterComponentGroup rcg = names[i];
				
				// Name NULL or doesn't exist in named variable map?
				// Unused, just add zeroes.
				if (rcg.name.size() == 0 || pixelShaderConstants.count(rcg.name) == 0)
				{
					for (u32 j = 0; j < rcg.size; j ++)
						data.push_back(0.f);
					
					continue;
				}
				
				const std::vector<f32> &groupFloats = pixelShaderConstants[rcg.name];
				
				// Must match the number of register components!
				ASSERT( groupFloats.size() == rcg.size );
				
				// Add it to the data
				for (u32 j = 0; j < groupFloats.size(); j ++)
					data.push_back(groupFloats[j]);
			}
			
			// Each register has 4 components.
			// This should have been enforced by LowLevelShaderRegisterMap class.
			ASSERT( data.size() % 4 == 0 );
			
			services->setPixelShaderConstant(&(data[0]), 0, data.size() / 4);
		}
	}
}




