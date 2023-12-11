#include "ShaderPostProcess.h"

CShaderPostProcess::CShaderPostProcess(IrrlichtDevice* device, const core::stringc& name,
		io::path vsFile, core::stringc vsEntry, video::E_VERTEX_SHADER_TYPE vsType,
		io::path psFile, core::stringc psEntry, video::E_PIXEL_SHADER_TYPE psType,
		video::E_MATERIAL_TYPE baseMaterial)
: IPostProcess(name), Device(device)
{
	// get the shader material fomr the material manager
	ShaderMaterial = new CShaderMaterial(device, name, vsFile, vsEntry, vsType, psFile, psEntry, psType, baseMaterial);

	// set material parameters 
	getMaterial().Lighting = false;
	getMaterial().BackfaceCulling = false;
	getMaterial().ZBuffer = video::ECFN_NEVER; 
}

CShaderPostProcess::~CShaderPostProcess()
{
	// drop the shader material
	if (ShaderMaterial)
		ShaderMaterial->drop();
}

void CShaderPostProcess::render()
{
	// clear the projection matrix 
	Device->getVideoDriver()->setTransform(video::ETS_PROJECTION, core::IdentityMatrix); 
	
	// clear the view matrix 
	Device->getVideoDriver()->setTransform(video::ETS_VIEW, core::IdentityMatrix); 

	// set the transform
	Device->getVideoDriver()->setTransform(video::ETS_WORLD, core::IdentityMatrix ); 
	
	// select the post proc material
	Device->getVideoDriver()->setMaterial(getMaterial());

	// render the screen quad
	Device->getVideoDriver()->drawIndexedTriangleList(Vertices, 4, Indices, 2);
}
