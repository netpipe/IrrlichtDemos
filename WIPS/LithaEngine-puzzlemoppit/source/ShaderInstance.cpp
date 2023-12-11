
#include "ShaderInstance.h"
#include "ShaderManager.h"
#include "Shader.h"

ShaderInstance::ShaderInstance(video::IVideoDriver *driver, ShaderManager *shaderManager,
		ShaderInstanceDef &shaderInstanceDef)
{
	this->shaderManager = shaderManager;
	this->shaderInstanceDef = shaderInstanceDef;
	
	usedMaterial = NULL;
	
	video::IGPUProgrammingServices *gpu = driver->getGPUProgrammingServices();
	
	if (shaderManager->GetShaderLevel() == ESL_LOW)
	{
		irrMaterialId = gpu->addShaderMaterialFromFiles(
				shaderInstanceDef.vertexShaderFileName,
				shaderInstanceDef.pixelShaderFileName,
				this, shaderInstanceDef.baseMaterial, 0);
	}
	else
	{
		irrMaterialId = gpu->addHighLevelShaderMaterialFromFiles(
				shaderInstanceDef.vertexShaderFileName, "main", video::EVST_VS_1_1,
				shaderInstanceDef.pixelShaderFileName, "main", video::EPST_PS_1_1,
				this, shaderInstanceDef.baseMaterial, 0);
	}
	
	NOTE << "New shader instance, material ID is " << irrMaterialId;
	
	if (irrMaterialId == -1)
	{
		WARN << "Could not create shader instance.";
	}
}

s32 ShaderInstance::GetIrrMaterialId()
{
	return irrMaterialId;
}

const ShaderInstanceDef &ShaderInstance::GetDef()
{
	return shaderInstanceDef;
}

void ShaderInstance::OnSetMaterial(const video::SMaterial &material)
{
	usedMaterial = &material;
}

void ShaderInstance::OnSetConstants(video::IMaterialRendererServices *services, s32 userData)
{
	ASSERT( usedMaterial );
	
	// Now find the correct Shader object to use to set the shader constants.
	// shader ID is packed in MaterialTypeParam2.
	
	f32 MaterialTypeParam2 = usedMaterial->MaterialTypeParam2;
	
	// get f32 pointer, cast to a u32 pointer, then dereference
	u32 shaderId = *((u32 *)(&MaterialTypeParam2));
	
	if (Shader *shader = shaderManager->GetShaderById(shaderId))
	{
		shader->OnSetConstants(services, userData);
	}
}


