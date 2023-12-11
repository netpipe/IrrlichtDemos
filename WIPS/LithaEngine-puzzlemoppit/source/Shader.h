
#ifndef SHADER_H
#define SHADER_H

#include "IShader.h"
#include <vector>
#include <map>

#include "ShaderInstanceDef.h"

class ShaderManager;
class ShaderInstance;

// This class is used to hold a unique set of shader constants.
// The actual GPU shaders/materials are cached and not necessarily created with each Shader.
class Shader : public IShader
{
	ShaderManager *shaderManager;
	
	// The shader instance contains the actual Irrlicht shader material.
	ShaderInstance *shaderInstance;
	
	u32 shaderId;
	
	std::map<core::stringc, std::vector<f32> > vertexShaderConstants;
	std::map<core::stringc, std::vector<f32> > pixelShaderConstants;
	
	LowLevelShaderRegisterMap vertexRegisterMap;
	LowLevelShaderRegisterMap pixelRegisterMap;
	
	IShaderCallback *callback;
	
	f32 s32_As_f32(s32 value)
	{
		return *((f32 *)(&value));
	}
	
	// Converts array of arbitrary 32bit values to a vector of floats
	// (i.e. these could either be f32* or s32*)
	std::vector<f32> valuesToVector(void *values, u16 count)
	{
		f32 *floats = (f32 *)values;
		
		std::vector<f32> result;
		
		for (u16 i = 0; i < count; i ++)
			result.push_back( floats[i] );
		
		return result;
	}
	
public:
	Shader(ShaderManager *shaderManager, ShaderInstanceDef shaderInstanceDef);
	~Shader();
	
	u32 GetID() { return shaderId; }
	
	void SetVertexRegisterMap(const LowLevelShaderRegisterMap &registerMap);
	void SetPixelRegisterMap(const LowLevelShaderRegisterMap &registerMap);
	
	void SetVertexConstant(const c8 *name, s32 value);
	void SetVertexConstant(const c8 *name, f32 value);
	
	void SetVertexConstants(const c8 *name, s32 *values, u16 count);
	void SetVertexConstants(const c8 *name, f32 *values, u16 count);
	
	void SetPixelConstant(const c8 *name, s32 value);
	void SetPixelConstant(const c8 *name, f32 value);
	
	void SetPixelConstants(const c8 *name, s32 *values, u16 count);
	void SetPixelConstants(const c8 *name, f32 *values, u16 count);
	
	void SetCallback(IShaderCallback *callback);
	
	void ApplyToIrrMaterial(video::SMaterial &material);
	void ClearFromIrrMaterial(video::SMaterial &material, video::E_MATERIAL_TYPE newMaterialType);
	
	// ShaderInstance forwards its OnSetConstants to this.
	void OnSetConstants(video::IMaterialRendererServices *services, s32 userData);
};

#endif
