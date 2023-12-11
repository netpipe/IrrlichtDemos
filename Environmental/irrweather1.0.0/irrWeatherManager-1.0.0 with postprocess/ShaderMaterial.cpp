#include "ShaderMaterial.h"

#include <iostream>

CShaderMaterial::CShaderMaterial(IrrlichtDevice* device, const core::stringc& name,
		io::path vsFile, core::stringc vsEntry, video::E_VERTEX_SHADER_TYPE vsType,
		io::path psFile, core::stringc psEntry, video::E_PIXEL_SHADER_TYPE psType,
		video::E_MATERIAL_TYPE baseMaterial) : Device(device), Name(name), PixelShaderFlags(0), VertexShaderFlags(0)
{
	s32 userdata = 0;
	io::path vsPath;
	io::path psPath;

	// log action
	core::stringc msg = core::stringc("compiling material ") + name;
	Device->getLogger()->log(msg.c_str(), ELL_INFORMATION);

	// create destination path for (driver specific) shader files
	if(Device->getVideoDriver()->getDriverType() == video::EDT_OPENGL)
	{
		vsPath = core::stringc("../../Data/media/shaders/glsl/") + vsFile;
		psPath = core::stringc("../../Data/media/shaders/glsl/") + psFile;
		userdata = 1;
	}
	if(Device->getVideoDriver()->getDriverType() == video::EDT_DIRECT3D9)
	{
		vsPath=core::stringc("../../Data/media/shaders/hlsl/") + vsFile;
		psPath=core::stringc("../../Data/media/shaders/hlsl/") + psFile;
		userdata = 0;
	}

	// create shader material
	video::IGPUProgrammingServices* gpu = Device->getVideoDriver()->getGPUProgrammingServices();
	s32 matID = gpu->addHighLevelShaderMaterialFromFiles(
			vsPath, vsEntry.c_str(), vsType,
			psPath, psEntry.c_str(), psType,
			this, baseMaterial,	userdata);

	// set material type
	Material.MaterialType = (video::E_MATERIAL_TYPE)matID;

	// set the default texturenames and texture wrap
	// these are overridden by the effect.xml configuration
	for (u32 i=0; i<video::MATERIAL_MAX_TEXTURES; ++i)
	{
		TextureName[i] = core::stringc("texture") + core::stringc(i);
		Material.TextureLayer[i].TextureWrapU = video::ETC_REPEAT;
		Material.TextureLayer[i].TextureWrapV = video::ETC_REPEAT;
	}
}

void CShaderMaterial::OnSetConstants(video::IMaterialRendererServices* services, s32 userdata)
{
	// set the constants for the pixelshader
	if(PixelShaderConstant.size())
	{
		core::map<const core::stringc, f32>::Iterator psParamIter = PixelShaderConstant.getIterator();
		for(; !psParamIter.atEnd(); psParamIter++)
		{
			services->setPixelShaderConstant(
				psParamIter.getNode()->getKey().c_str(),
				&psParamIter.getNode()->getValue(),
				1);
		}
	}
	if(PixelShaderUniform.size())
	{
		core::map<const core::stringc, SUniformDescriptor>::Iterator psParamIter = PixelShaderUniform.getIterator();
		for(; !psParamIter.atEnd(); psParamIter++)
		{
			services->setPixelShaderConstant(
				psParamIter.getNode()->getKey().c_str(),
				psParamIter.getNode()->getValue().ParamPointer,
				psParamIter.getNode()->getValue().ParamCount);
		}
	}

	// set the constants for the vertexshader
	if(VertexShaderConstant.size())
	{
		core::map<const core::stringc, f32>::Iterator vsParamIter = VertexShaderConstant.getIterator();
		for(; !vsParamIter.atEnd(); vsParamIter++)
		{
			services->setVertexShaderConstant(
				vsParamIter.getNode()->getKey().c_str(),
				&vsParamIter.getNode()->getValue(),
				1);
		}
	}
	if(VertexShaderUniform.size())
	{
		core::map<const core::stringc, SUniformDescriptor>::Iterator vsParamIter = VertexShaderUniform.getIterator();
		for(; !vsParamIter.atEnd(); vsParamIter++)
		{
			services->setVertexShaderConstant(
				vsParamIter.getNode()->getKey().c_str(),
				vsParamIter.getNode()->getValue().ParamPointer,
				vsParamIter.getNode()->getValue().ParamCount);
		}
	}

	// set the elapsed time if the shader wants it
	if (getPixelShaderFlag(ESC_TIME) || getVertexShaderFlag(ESC_TIME))
	{
		// get the elapsed time in ms
		f32 elapsedTime = Device->getTimer()->getTime()* 0.001f;

		if(getPixelShaderFlag(ESC_TIME))
			services->setPixelShaderConstant("ElapsedTime", &elapsedTime, 1);
		if(getVertexShaderFlag(ESC_TIME))
			services->setVertexShaderConstant("ElapsedTime", &elapsedTime, 1);
	}

	// set a random value if the shader wants it
	if (getPixelShaderFlag(ESC_RANDOM) || getVertexShaderFlag(ESC_RANDOM))
	{
		// create a random value in the intervall [0, 1]
		f32 random = (f32) rand() / (f32) RAND_MAX;

		if(getPixelShaderFlag(ESC_RANDOM))
			services->setPixelShaderConstant("RandomValue", &random, 1);
		if(getVertexShaderFlag(ESC_RANDOM))
			services->setVertexShaderConstant("RandomValue", &random, 1);
	}

	// set the projection matrix if the shader wants it
	if (getPixelShaderFlag(ESC_PROJECTION) || getVertexShaderFlag(ESC_PROJECTION))
	{
		// get the projection matrix
		core::matrix4 projMatrix = Device->getVideoDriver()->getTransform(video::ETS_PROJECTION);

		if(getPixelShaderFlag(ESC_PROJECTION))
			services->setPixelShaderConstant("ProjMatrix", projMatrix.pointer(), 16);
		if(getVertexShaderFlag(ESC_PROJECTION))
			services->setVertexShaderConstant("ProjMatrix", projMatrix.pointer(), 16);
	}

	// set the view matrix if the shader wants it
	if (getPixelShaderFlag(ESC_VIEW) || getVertexShaderFlag(ESC_VIEW))
	{
		// get the view matrix
		core::matrix4 viewMatrix = Device->getVideoDriver()->getTransform(video::ETS_VIEW);

		if(getPixelShaderFlag(ESC_VIEW))
			services->setPixelShaderConstant("ViewMatrix", viewMatrix.pointer(), 16);
		if(getVertexShaderFlag(ESC_VIEW))
			services->setVertexShaderConstant("ViewMatrix", viewMatrix.pointer(), 16);
	}

	// set the world matrix if the shader wants it
	if (getPixelShaderFlag(ESC_WORLD) || getVertexShaderFlag(ESC_WORLD))
	{
		// get the world matrix
		core::matrix4 worldMatrix = Device->getVideoDriver()->getTransform(video::ETS_WORLD);

		if(getPixelShaderFlag(ESC_WORLD))
			services->setPixelShaderConstant("WorldMatrix", worldMatrix.pointer(), 16);
		if(getVertexShaderFlag(ESC_WORLD))
			services->setVertexShaderConstant("WorldMatrix", worldMatrix.pointer(), 16);
	}

	// set the world view matrix if the shader wants it
	if (getPixelShaderFlag(ESC_WORLDVIEW) || getVertexShaderFlag(ESC_WORLDVIEW))
	{
		// calculate the world view matrix
		core::matrix4 worldViewMatrix = Device->getVideoDriver()->getTransform(video::ETS_VIEW);
		worldViewMatrix *= Device->getVideoDriver()->getTransform(video::ETS_WORLD);

		if(getPixelShaderFlag(ESC_WORLDVIEW))
			services->setPixelShaderConstant("WorldViewMatrix", worldViewMatrix.pointer(), 16);
		if(getVertexShaderFlag(ESC_WORLDVIEW))
			services->setVertexShaderConstant("WorldViewMatrix", worldViewMatrix.pointer(), 16);
	}

	// set the world view projection matrix if the shader wants it
	if (getPixelShaderFlag(ESC_WORLDVIEWPROJ) || getVertexShaderFlag(ESC_WORLDVIEWPROJ))
	{
		// calculate the world view projection matrix
		core::matrix4 worldViewProjMatrix = Device->getVideoDriver()->getTransform(video::ETS_PROJECTION);
		worldViewProjMatrix *= Device->getVideoDriver()->getTransform(video::ETS_VIEW);
		worldViewProjMatrix *= Device->getVideoDriver()->getTransform(video::ETS_WORLD);

		if(getPixelShaderFlag(ESC_WORLDVIEWPROJ))
			services->setPixelShaderConstant("WorldViewProjMatrix", worldViewProjMatrix.pointer(), 16);
		if(getVertexShaderFlag(ESC_WORLDVIEWPROJ))
			services->setVertexShaderConstant("WorldViewProjMatrix", worldViewProjMatrix.pointer(), 16);
	}

	// set the camera position if the shader wants it
	scene::ICameraSceneNode* camera = Device->getSceneManager()->getActiveCamera();
	if (camera && (getPixelShaderFlag(ESC_CAMERAPOSITION) || getVertexShaderFlag(ESC_CAMERAPOSITION)) )
	{
		// get the camera position
		f32 cameraPosition[3];
		camera->getAbsolutePosition().getAs3Values(cameraPosition);

		if(getPixelShaderFlag(ESC_CAMERAPOSITION))
			services->setPixelShaderConstant("CameraPosition", &cameraPosition[0], 3);
		if(getVertexShaderFlag(ESC_CAMERAPOSITION))
			services->setVertexShaderConstant("CameraPosition", &cameraPosition[0], 3);
	}

    // set a random value if the shader wants it
	if (getPixelShaderFlag(ESC_MOUSEPOSITIONX) || getVertexShaderFlag(ESC_MOUSEPOSITIONX))
	{
		// create a random value in the intervall [0, 1]
		f32 mousePositionX = Device->getCursorControl()->getRelativePosition().X;

		if(getPixelShaderFlag(ESC_MOUSEPOSITIONX))
			services->setPixelShaderConstant("MousePositionX", &mousePositionX, 1);
		if(getVertexShaderFlag(ESC_MOUSEPOSITIONX))
			services->setVertexShaderConstant("MousePositionX", &mousePositionX, 1);
	}

    // set a random value if the shader wants it
	if (getPixelShaderFlag(ESC_MOUSEPOSITIONY) || getVertexShaderFlag(ESC_MOUSEPOSITIONY))
	{
		// create a random value in the intervall [0, 1]
		f32 mousePositionY = Device->getCursorControl()->getRelativePosition().Y;

		if(getPixelShaderFlag(ESC_MOUSEPOSITIONY))
			services->setPixelShaderConstant("MousePositionY", &mousePositionY, 1);
		if(getVertexShaderFlag(ESC_MOUSEPOSITIONY))
			services->setVertexShaderConstant("MousePositionY", &mousePositionY, 1);
	}

	// set buffer dimensions
	if(Material.TextureLayer[0].Texture != NULL)
	{
		// set buffer width if the shader wants it
		if (getPixelShaderFlag(ESC_BUFFERWIDTH) || getVertexShaderFlag(ESC_BUFFERWIDTH))
		{
			// get the texture width
			f32 width = (f32) Material.TextureLayer[0].Texture->getSize().Width;

			if(getPixelShaderFlag(ESC_BUFFERWIDTH))
				services->setPixelShaderConstant("BufferWidth", &width, 1);
			if(getVertexShaderFlag(ESC_BUFFERWIDTH))
				services->setVertexShaderConstant("BufferWidth", &width, 1);
		}
		// set buffer height if the shader wants it
		if (getPixelShaderFlag(ESC_BUFFERHEIGHT) || getVertexShaderFlag(ESC_BUFFERHEIGHT))
		{
			// get the texture height
			f32 height = (f32) Material.TextureLayer[0].Texture->getSize().Height;

			if(getPixelShaderFlag(ESC_BUFFERHEIGHT))
				services->setPixelShaderConstant("BufferHeight", &height, 1);
			if(getVertexShaderFlag(ESC_BUFFERHEIGHT))
				services->setVertexShaderConstant("BufferHeight", &height, 1);
		}
	}

	// set texture names for OpenGL Shaders
	// (this is not necessary for DirectX)
	if(userdata == 1)
	{
		for (u32 i=0; i<video::MATERIAL_MAX_TEXTURES; ++i)
		{
			if(Material.TextureLayer[i].Texture != NULL)
				services->setPixelShaderConstant(TextureName[i].c_str(), (int*) &i, 1);
		}
	}
}
