#ifndef _SHADERPOSTPROCESS_H
#define _SHADERPOSTPROCESS_H
 
#include "IPostprocess.h"
#include "ShaderMaterial.h"

class CShaderPostProcess : public IPostProcess
{
public:
	// constructor
	CShaderPostProcess(IrrlichtDevice* device, const core::stringc& name,
		io::path vsFile, core::stringc vsEntry, video::E_VERTEX_SHADER_TYPE vsType,
		io::path psFile, core::stringc psEntry, video::E_PIXEL_SHADER_TYPE psType,
		video::E_MATERIAL_TYPE baseMaterial);

	// destructor
	virtual ~CShaderPostProcess();

protected:
	// the irrlicht device
	IrrlichtDevice* Device;

	// shader material of the postprocess
	CShaderMaterial* ShaderMaterial;

public:
	// returns the material of the postprocess
	virtual video::SMaterial& getMaterial() { return ShaderMaterial->getMaterial(); }
	
	// returns the shader material object of the postprocess
	virtual CShaderMaterial* getShaderMaterial() { return ShaderMaterial; }
	
	// renders the postprocess
	virtual void render();
};

#endif
