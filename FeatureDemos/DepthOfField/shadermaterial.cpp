#include "shadermaterial.h"
//#include "shadermaterialcallback.h"
#include <irrlicht.h>
#include <iostream>
using namespace irr;
#pragma comment(lib, "Irrlicht.lib")



class ShaderCallBack : public video::IShaderConstantSetCallBack
{
public:
	ShaderCallBack() {};
	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData){};
};


ShaderMaterial::ShaderMaterial(IrrlichtDevice* d, c8* vsFileName, c8* psFileName, s32 userData,
							   video::IShaderConstantSetCallBack* callback, int w, int h)
{
	device = d;
	texture = device->getVideoDriver()->createRenderTargetTexture(core::dimension2d<s32>(w,h));
	init(vsFileName, psFileName, callback, userData);
}

ShaderMaterial::~ShaderMaterial() {}


void ShaderMaterial::init(c8* vsFileName, c8* psFileName, video::IShaderConstantSetCallBack* callback, s32 userData)
{
	printf("shadermaterial %s %s \n",  vsFileName, psFileName );
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* gui = device->getGUIEnvironment();

	if (!driver->queryFeature(video::EVDF_PIXEL_SHADER_2_0) && !driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1))
	{
		device->getLogger()->log("WARNING: Pixel shaders disabled because of missing driver/hardware support.");
		psFileName = 0;
	}

	if (!driver->queryFeature(video::EVDF_VERTEX_SHADER_2_0) && !driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1))
	{
		device->getLogger()->log("WARNING: Vertex shaders disabled because of missing driver/hardware support.");
		vsFileName = 0;
	}

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	if (gpu)
	{
		if (!callback) {
			callback = new ShaderCallBack();
		} else {
			callback->grab();
		}
		material = gpu->addHighLevelShaderMaterialFromFiles(vsFileName, "vertexMain", video::EVST_VS_2_0,
			psFileName, "pixelMain", video::EPST_PS_2_0, callback, video::EMT_SOLID, userData);

		callback->drop();
	}
}
