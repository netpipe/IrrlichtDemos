
#include "ShaderManager.h"
#include "Shader.h"
#include "ShaderInstance.h"

u32 ShaderManager::GenShaderId()
{
	while (true)
	{
		u32 id = (u32)rand();
		
		if (!GetShaderById(id))
			return id;
	}
}

ShaderManager::ShaderManager(video::IVideoDriver *driver)
{
	this->driver = driver;
	
	forceNoShaders = false;
	
	SetShaderLevel(ESL_HIGH);
}

bool ShaderManager::ShadersAreAvailable()
{
	if (forceNoShaders)
		return false;
	
	// OpenGL only
	
	if (shaderLevel == ESL_LOW)
	{
		return driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1)
				&& driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1);
	}
	else
	{
		return driver->queryFeature(video::EVDF_ARB_GLSL);
	}
}

void ShaderManager::ForceNoShaders(bool noShaders)
{
	this->forceNoShaders = noShaders;
}

u32 ShaderManager::RegisterShader(Shader *shader)
{
	ASSERT(shader);
	
	u32 shaderId = GenShaderId();
	
	shaders[shaderId] = shader;
	
	return shaderId;
}

void ShaderManager::UnregisterShader(Shader *shader)
{
	ASSERT(shader);
	shaders.erase(shader->GetID());
}

Shader *ShaderManager::GetShaderById(u32 shaderId)
{
	if (shaders.count(shaderId))
		return shaders[shaderId];
	else
		return NULL;
}

ShaderInstance *ShaderManager::GetShaderInstance(ShaderInstanceDef &shaderInstanceDef)
{
	for (u32 i = 0; i < shaderInstances.size(); i ++)
	{
		if (shaderInstances[i]->GetDef() == shaderInstanceDef)
			return shaderInstances[i];
	}
	
	// Doesn't exist, so create
	ShaderInstance *shaderInstance = new ShaderInstance(driver, this, shaderInstanceDef);
	
	// It's immediately (and permanently) added to the Irrlicht engine as a material so we
	// no longer need to keep track of it.
	shaderInstance->drop();
	
	shaderInstances.push_back(shaderInstance);
	
	return shaderInstance;
	
	// Was formerly using map for this.
	// I had a bug in my map functor or something, so I took the easy route and am using vector instead.
	/*
	// Does this shader instance already exist?
	if (shaderInstances.find(shaderInstanceDef) == shaderInstances.end() )
	{
		// Doesn't exist, so create
		ShaderInstance *shaderInstance = new ShaderInstance(driver, this, shaderInstanceDef);
		
		// It's immediately (and permanently) added to the Irrlicht engine as a material so we
		// no longer need to keep track of it.
		shaderInstance->drop();
		
		shaderInstances[shaderInstanceDef] = shaderInstance;
	}
	
	return shaderInstances[shaderInstanceDef];
	*/
}

void ShaderManager::SetShaderLevel(E_SHADER_LEVEL level)
{
	shaderLevel = level;
	
	NOTE << "Shader level set to " << s32(level) << " (" << (level == ESL_LOW ? "low" : "high") << ")";
}

E_SHADER_LEVEL ShaderManager::GetShaderLevel()
{
	return shaderLevel;
}

Shader *ShaderManager::CreateShader(const c8 *vertexShaderFileName, const c8 *pixelShaderFileName, video::E_MATERIAL_TYPE baseMaterial)
{
	if (!ShadersAreAvailable())
	{
		WARN << "Shaders not available.";
		return NULL;
	}
	
	return new Shader(this, ShaderInstanceDef(vertexShaderFileName,pixelShaderFileName,baseMaterial));
}
