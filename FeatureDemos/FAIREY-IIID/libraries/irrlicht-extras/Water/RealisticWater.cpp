/*
 * Copyright (c) 2013, elvman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY elvman ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "RealisticWater.h"

RealisticWaterSceneNode::RealisticWaterSceneNode(scene::ISceneManager* sceneManager, f32 width, f32 height,
												 const irr::core::stringc& resourcePath, core::dimension2du renderTargetSize,
												 scene::ISceneNode* parent, s32 id):
	scene::ISceneNode(parent, sceneManager, id), _time(0),
	_size(width, height), _sceneManager(sceneManager), _refractionMap(NULL), _reflectionMap(NULL),
	_windForce(20.0f),_windDirection(0, 1),_waveHeight(0.3f), _waterColor(0.1f, 0.1f, 0.6f, 1.0f), _colorBlendFactor(0.2f), _camera(NULL)
{
	_videoDriver = sceneManager->getVideoDriver();

	// create new camera
	_camera = sceneManager->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), -1, false);

	_waterMesh = sceneManager->addHillPlaneMesh("RealisticWater", _size, core::dimension2d<u32>(1, 1));

	_waterSceneNode = sceneManager->addMeshSceneNode(_waterMesh->getMesh(0), this);

	video::IGPUProgrammingServices* GPUProgrammingServices = _videoDriver->getGPUProgrammingServices();

	core::stringc waterPixelShader;
	core::stringc waterVertexShader;

	if (_videoDriver->getDriverType() == video::EDT_DIRECT3D9)
	{
		waterPixelShader = resourcePath + "/shaders/Water_ps.hlsl";
		waterVertexShader = resourcePath + "/shaders/Water_vs.hlsl";
	}
	else if (_videoDriver->getDriverType() == video::EDT_OPENGL)
	{
		waterPixelShader = resourcePath + "/shaders/Water_ps.glsl";
		waterVertexShader = resourcePath + "/shaders/Water_vs.glsl";
	}

	_shaderMaterial = GPUProgrammingServices->addHighLevelShaderMaterialFromFiles(
		waterVertexShader.c_str(), "main", video::EVST_VS_1_1,
		waterPixelShader.c_str(), "main", video::EPST_PS_1_1,
		this);

	_waterSceneNode->setMaterialType((video::E_MATERIAL_TYPE)_shaderMaterial);

	irr::video::ITexture* bumpTexture = _videoDriver->getTexture(resourcePath + "/data/waterbump.png");
	_waterSceneNode->setMaterialTexture(0, bumpTexture);

	_refractionMap = _videoDriver->addRenderTargetTexture(renderTargetSize);
	_reflectionMap = _videoDriver->addRenderTargetTexture(renderTargetSize);

	_waterSceneNode->setMaterialTexture(1, _refractionMap);
	_waterSceneNode->setMaterialTexture(2, _reflectionMap);
}

RealisticWaterSceneNode::~RealisticWaterSceneNode()
{
	if (_camera)
	{
		_camera->drop();
		_camera = NULL;
	}

	if (_refractionMap)
	{
		_refractionMap->drop();
		_refractionMap = NULL;
	}

	if (_reflectionMap)
	{
		_reflectionMap->drop();
		_reflectionMap = NULL;
	}

	if (_waterSceneNode)
	{
		_waterSceneNode->drop();
		_waterSceneNode = NULL;
	}

	if (_waterMesh)
	{
		_waterMesh->drop();
		_waterMesh = NULL;
	}
}

// frame
void RealisticWaterSceneNode::OnRegisterSceneNode()
{
	ISceneNode::OnRegisterSceneNode();

	if (IsVisible)
	{
		_sceneManager->registerNodeForRendering(this);
	}
}

void RealisticWaterSceneNode::OnAnimate(u32 timeMs)
{
	ISceneNode::OnAnimate(timeMs);

	_time = timeMs;

	//fixes glitches with incomplete refraction
	const f32 CLIP_PLANE_OFFSET_Y = 5.0f; // 5.0f;

	if (IsVisible)
	{
		setVisible(false); //hide the water

		//refraction
		_videoDriver->setRenderTarget(_refractionMap, true, true); //render to refraction

		//refraction clipping plane
		core::plane3d<f32> refractionClipPlane(0, RelativeTranslation.Y + CLIP_PLANE_OFFSET_Y, 0, 0, -1, 0); //refraction clip plane
		_videoDriver->setClipPlane(0, refractionClipPlane, true);

		_sceneManager->drawAll(); //draw the scene

		//reflection
		_videoDriver->setRenderTarget(_reflectionMap, true, true); //render to reflection

		//get current camera
		scene::ICameraSceneNode* currentCamera = _sceneManager->getActiveCamera();

		//set FOV anf far value from current camera
		// _camera->setFarValue(currentCamera->getFarValue());
		// _camera->setFOV(currentCamera->getFOV());
		
		_camera->setNearValue(500.0f);
		_camera->setFarValue(15000.0f);     /// Must be bigger than water
		_camera->setFOV(10*core::DEGTORAD); /// Playing with FOV and Z distance

		core::vector3df position = currentCamera->getAbsolutePosition();
		position.Y = -position.Y + 2 * RelativeTranslation.Y; //position of the water
		_camera->setPosition(position);

		core::vector3df target = currentCamera->getTarget();

		//invert Y position of current camera
		target.Y = -target.Y + 2 * RelativeTranslation.Y;
		_camera->setTarget(target);

		//set the reflection camera
		_sceneManager->setActiveCamera(_camera);

		//reflection clipping plane
		core::plane3d<f32> reflectionClipPlane(0, RelativeTranslation.Y - CLIP_PLANE_OFFSET_Y, 0, 0, 1, 0);
		_videoDriver->setClipPlane(0, reflectionClipPlane, true);

		_sceneManager->drawAll(); //draw the scene

		//disable clip plane
		_videoDriver->enableClipPlane(0, false);

		//set back old render target
		_videoDriver->setRenderTarget(0, false, true);

		//set back the active camera
		_sceneManager->setActiveCamera(currentCamera);

		setVisible(true); //show it again
	}
}

void RealisticWaterSceneNode::render()
{
	/*core::array<video::IRenderTarget> renderTargets;
	//renderTargets.push_back();
	renderTargets.push_back(_refractionMap);

	_videoDriver->setRenderTarget(renderTargets, true, true);*/
	//_videoDriver->draw2DImage(_reflectionMap,core::position2d<s32>(0,0));
}

// returns the axis aligned bounding box of terrain
const core::aabbox3d<f32>& RealisticWaterSceneNode::getBoundingBox() const
{
	return _waterSceneNode->getBoundingBox();
}

void RealisticWaterSceneNode::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
	video::IVideoDriver* driver = services->getVideoDriver();

	core::matrix4 projection = driver->getTransform(video::ETS_PROJECTION);
	core::matrix4 view = driver->getTransform(video::ETS_VIEW);
	core::matrix4 world = driver->getTransform(video::ETS_WORLD);

	core::matrix4 cameraView = _camera->getViewMatrix();

	// vertex shader constants
	// services->setVertexShaderConstant("View", view.pointer(), 16);

	core::matrix4 worldViewProj = projection;
	worldViewProj *= view;
	worldViewProj *= world;

	core::matrix4 worldReflectionViewProj = projection;
	worldReflectionViewProj *= cameraView;
	worldReflectionViewProj *= world;

	f32 waveLength = 0.1f;
	f32 time = _time / 100000.0f;
	core::vector3df cameraPosition = _sceneManager->getActiveCamera()->getPosition();

	bool fogEnabled = getMaterial(0).getFlag(video::EMF_FOG_ENABLE);
	irr::video::SColor color;
	irr::video::E_FOG_TYPE fogType;
	f32 start;
	f32 end;
	f32 density;
	bool pixelFog;
	bool rangeFog;
	driver->getFog(color, fogType, start, end, density, pixelFog, rangeFog);

#if (IRRLICHT_VERSION_MAJOR == 1 && IRRLICHT_VERSION_MINOR == 9)
	services->setVertexShaderConstant(services->getVertexShaderConstantID("WorldViewProj"), worldViewProj.pointer(), 16);
	services->setVertexShaderConstant(services->getVertexShaderConstantID("WorldReflectionViewProj"), worldReflectionViewProj.pointer(), 16);
	services->setVertexShaderConstant(services->getVertexShaderConstantID("WaveLength"), &waveLength, 1);
	services->setVertexShaderConstant(services->getVertexShaderConstantID("Time"), &time, 1);
	services->setVertexShaderConstant(services->getVertexShaderConstantID("WindForce"), &_windForce, 1);
	services->setVertexShaderConstant(services->getVertexShaderConstantID("WindDirection"), &_windDirection.X, 2);
	services->setPixelShaderConstant(services->getVertexShaderConstantID("CameraPosition"), &cameraPosition.X, 3);
	services->setPixelShaderConstant(services->getVertexShaderConstantID("WaveHeight"), &_waveHeight, 1);
	services->setPixelShaderConstant(services->getVertexShaderConstantID("WaterColor"), &_waterColor.r, 4);
	services->setPixelShaderConstant(services->getVertexShaderConstantID("ColorBlendFactor"), &_colorBlendFactor, 1);
#else
	services->setVertexShaderConstant("WorldViewProj", worldViewProj.pointer(), 16);
	services->setVertexShaderConstant("WorldReflectionViewProj", worldReflectionViewProj.pointer(), 16);
	services->setVertexShaderConstant("WaveLength", &waveLength, 1);
	services->setVertexShaderConstant("Time", &time, 1);
	services->setVertexShaderConstant("WindForce", &_windForce, 1);
	services->setVertexShaderConstant("WindDirection", &_windDirection.X, 2);
	services->setPixelShaderConstant("CameraPosition", &cameraPosition.X, 3);
	services->setPixelShaderConstant("WaveHeight", &_waveHeight, 1);
	services->setPixelShaderConstant("WaterColor", &_waterColor.r, 4);
	services->setPixelShaderConstant("ColorBlendFactor", &_colorBlendFactor, 1);
#endif

	//texture constants for GLSL
	if (driver->getDriverType() == video::EDT_OPENGL)
	{
		int var0 = 0;
		int var1 = 1;
		int var2 = 2;

#if (IRRLICHT_VERSION_MAJOR == 1 && IRRLICHT_VERSION_MINOR == 9)
		services->setPixelShaderConstant(services->getVertexShaderConstantID("WaterBump"), &var0, 1);
		services->setPixelShaderConstant(services->getVertexShaderConstantID("RefractionMap"), &var1, 1);
		services->setPixelShaderConstant(services->getVertexShaderConstantID("ReflectionMap"), &var2, 1);

		services->setPixelShaderConstant(services->getVertexShaderConstantID("FogEnabled"), (int*)&fogEnabled, 1);
		services->setPixelShaderConstant(services->getVertexShaderConstantID("FogMode"), (int*)&fogType, 1);
#else
		services->setPixelShaderConstant("WaterBump", &var0, 1);
		services->setPixelShaderConstant("RefractionMap", &var1, 1);
		services->setPixelShaderConstant("ReflectionMap", &var2, 1);

		services->setPixelShaderConstant("FogEnabled", (int*)&fogEnabled, 1);
		services->setPixelShaderConstant("FogMode", (int*)&fogType, 1);
#endif
	}
}

void RealisticWaterSceneNode::setWindForce(const f32 windForce)
{
	_windForce = windForce;
}

void RealisticWaterSceneNode::setWindDirection(const core::vector2df& windDirection)
{
	_windDirection = windDirection;
	_windDirection.normalize();
}

void RealisticWaterSceneNode::setWaveHeight(const f32 waveHeight)
{
	_waveHeight = waveHeight;
}

void RealisticWaterSceneNode::setWaterColor(const video::SColorf& waterColor)
{
	_waterColor = waterColor;
}

void RealisticWaterSceneNode::setColorBlendFactor(const f32 colorBlendFactor)
{
	_colorBlendFactor = colorBlendFactor;
}
