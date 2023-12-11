#ifndef SHADER_INSTANCE_H
#define SHADER_INSTANCE_H

#include "litha_internal.h"
#include "ShaderInstanceDef.h"

class ShaderManager;

// An instance of an Irrlicht shader material
class ShaderInstance : public video::IShaderConstantSetCallBack
{
	ShaderManager *shaderManager;
	
	ShaderInstanceDef shaderInstanceDef;
	
	s32 irrMaterialId;
	
	// Cached from OnSetMaterial, for use in OnSetConstants.
	const video::SMaterial *usedMaterial;
	
public:
	ShaderInstance(video::IVideoDriver *driver, ShaderManager *shaderManager, ShaderInstanceDef &shaderInstanceDef);
	
	s32 GetIrrMaterialId();
	
	// Get the definition of this shader instance.
	// Used when checking if a shader instance has already been created for a definition.
	const ShaderInstanceDef &GetDef();
	
	// NEED TO ENSURE THAT IRRLICHT DOES ACTUALLY SET THE MATERIAL BEFORE CALLING
	// OnSetConstants!!
	
	void OnSetMaterial(const video::SMaterial &material);
	
	void OnSetConstants(video::IMaterialRendererServices *services, s32 userData);
};

#endif
