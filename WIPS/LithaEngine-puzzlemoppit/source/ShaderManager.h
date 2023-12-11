
#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "litha_internal.h"
#include <map>
#include <vector>
#include "ShaderInstanceDef.h"
#include "IRenderSystem.h" // for E_SHADER_LEVEL

class Shader;
class ShaderInstance;

class ShaderManager : public virtual IReferenceCounted
{
	video::IVideoDriver *driver;
	
	bool forceNoShaders;
	
	E_SHADER_LEVEL shaderLevel;
	
	// shader IDs mapped to Shader objects
	std::map<u32, Shader *> shaders;
	
	// Shader instances (irrlicht shader materials)
	//std::map<ShaderInstanceDef, ShaderInstance *, ShaderInstanceDef_Compare> shaderInstances;
	std::vector<ShaderInstance *> shaderInstances;
	
	u32 GenShaderId();
	
public:
	ShaderManager(video::IVideoDriver *driver);
	
	bool ShadersAreAvailable();
	void ForceNoShaders(bool noShaders);
	
	// returns a unique ID for the shader
	u32 RegisterShader(Shader *shader);
	void UnregisterShader(Shader *shader);
	
	Shader *GetShaderById(u32 shaderId);
	
	ShaderInstance *GetShaderInstance(ShaderInstanceDef &shaderInstanceDef);
	
	void SetShaderLevel(E_SHADER_LEVEL level);
	E_SHADER_LEVEL GetShaderLevel();
	
	// IWorld.CreateShader maps directly to this.
	// If ShaderManager had an interface this would be the only method in it, and this would return IShader.
	Shader *CreateShader(const c8 *vertexShaderFileName, const c8 *pixelShaderFileName, video::E_MATERIAL_TYPE baseMaterial);
};

#endif
