#include "RealisticWater.h"

RealisticWaterSceneNode::RealisticWaterSceneNode(scene::ISceneManager* sceneManager, f32 width, f32 height, 
												video::ITexture* bumpTexture, ITimer* timer, core::dimension2di renderTargetSize,
												scene::ISceneNode* parent, s32 id):
	scene::ISceneNode( parent?parent:sceneManager->getRootSceneNode(), sceneManager, id),
	Timer(timer), Size(width,height), SceneManager(sceneManager), RefractionMap(0), ReflectionMap(0),
	WindForce(20.0f),WindDirection(0,1),WaveHeight(0.3f), WaterColor(0.1f, 0.1f, 0.6f, 1.0f), ColorBlendFactor(0.2f), Camera(0)
{
	VideoDriver = SceneManager->getVideoDriver();

	scene::ICameraSceneNode* CurrentCamera=SceneManager->getActiveCamera(); //get current camera
	Camera=SceneManager->addCameraSceneNode(); //create new camera
	SceneManager->setActiveCamera(CurrentCamera); //set back the previous camera	

	scene::IAnimatedMesh* WaterMesh=SceneManager->addHillPlaneMesh("RealisticWater",Size,core::dimension2d<s32>(1,1));

	WaterSceneNode=SceneManager->addMeshSceneNode(WaterMesh->getMesh(0),this);
	
	//WaterMesh->drop();

	video::IGPUProgrammingServices* GPUProgrammingServices = VideoDriver->getGPUProgrammingServices();

	std::string WaterPixelShader = "";
	std::string WaterVertexShader = "";

	if (VideoDriver->getDriverType()==video::EDT_DIRECT3D9)
	{
		WaterPixelShader="shaders/Water_ps.hlsl";
		WaterVertexShader="shaders/Water_vs.hlsl";
	}
	else if (VideoDriver->getDriverType()==video::EDT_OPENGL)
	{
		WaterPixelShader="shaders/Water_ps.glsl";
		WaterVertexShader="shaders/Water_vs.glsl";
	}

	ShaderMaterial=GPUProgrammingServices->addHighLevelShaderMaterialFromFiles(
		WaterVertexShader.c_str(),"main",video::EVST_VS_1_1,
		WaterPixelShader.c_str(),"main",video::EPST_PS_2_0,
		this,video::EMT_LIGHTMAP);

	WaterSceneNode->setMaterialType((video::E_MATERIAL_TYPE)ShaderMaterial);

	WaterSceneNode->setMaterialTexture(0,bumpTexture);

	RefractionMap=VideoDriver->createRenderTargetTexture(renderTargetSize);
	ReflectionMap=VideoDriver->createRenderTargetTexture(renderTargetSize);

	WaterSceneNode->setMaterialTexture(1,RefractionMap);
	WaterSceneNode->setMaterialTexture(2,ReflectionMap);
}

RealisticWaterSceneNode::~RealisticWaterSceneNode()
{
	//Camera->drop();

	if (RefractionMap) RefractionMap->drop();
	if (ReflectionMap) ReflectionMap->drop();
}

// frame
void RealisticWaterSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void RealisticWaterSceneNode::OnAnimate(u32 timeMs)
{
	if (IsVisible)
	{
		setVisible(false); //hide the water

		//refraction
		VideoDriver->setRenderTarget(RefractionMap, true, true, video::SColor(0,0,0,255)); //render to refraction

		core::plane3d<f32> refractionClipPlane(0,RelativeTranslation.Y+5,0, 0,-1,0); //refraction clip plane
		//VideoDriver->setClipPlane(0,refractionClipPlane,true); // DC NOTE: Not supported in irrlicht 1.3

		SceneManager->drawAll(); //draw the scene

		//VideoDriver->enableClipPlane(0,false); // DC NOTE: Not supported in irrlicht 1.3

		//reflection
		VideoDriver->setRenderTarget(ReflectionMap, true, true, video::SColor(0,0,0,255)); //render to reflection

		scene::ICameraSceneNode* CurrentCamera=SceneManager->getActiveCamera(); //get current camera

		Camera->setFarValue(CurrentCamera->getFarValue());
		Camera->setFOV(CurrentCamera->getFOV());

		core::vector3df position=CurrentCamera->getPosition();
		position.Y=-position.Y+2*RelativeTranslation.Y; //position of the water
		Camera->setPosition(position);

		core::vector3df target=CurrentCamera->getTarget();
		target.Y=-target.Y+2*RelativeTranslation.Y;
		Camera->setTarget(target);

		SceneManager->setActiveCamera(Camera); //set the reflection camera

		//core::plane3d<f32> reflectionClipPlane(0,RelativeTranslation.Y,0, 0,1,0);
		//VideoDriver->setClipPlane(0,reflectionClipPlane,true);

		SceneManager->drawAll(); //draw the scene

		//VideoDriver->enableClipPlane(0,false);

		// set back old render target
		VideoDriver->setRenderTarget(0,true,true,video::SColor(255,255,255,255));

		SceneManager->setActiveCamera(CurrentCamera);

		setVisible(true); //show it again
	}

	ISceneNode::OnAnimate(timeMs);
}

void RealisticWaterSceneNode::render()
{
	//VideoDriver->draw2DImage(ReflectionMap,core::position2d<s32>(0,0));
}

// returns the axis aligned bounding box of terrain
const core::aabbox3d<f32>& RealisticWaterSceneNode::getBoundingBox() const
{
	return WaterSceneNode->getBoundingBox();
}

void RealisticWaterSceneNode::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
	video::IVideoDriver* driver = services->getVideoDriver();

	core::matrix4 projection = driver->getTransform(video::ETS_PROJECTION);
	core::matrix4 view = driver->getTransform(video::ETS_VIEW);
	core::matrix4 world = driver->getTransform(video::ETS_WORLD);

	core::matrix4 cameraView = Camera->getViewMatrix();

	//vertex shader constants
	services->setVertexShaderConstant("View", view.pointer(), 16);
	
	core::matrix4 worldViewProj = projection;			
	worldViewProj *= view;
	worldViewProj *= world;
	services->setVertexShaderConstant("WorldViewProj", worldViewProj.pointer(), 16);
	
	core::matrix4 worldReflectionViewProj=projection;
	worldReflectionViewProj *= cameraView;
	worldReflectionViewProj *= world;
	services->setVertexShaderConstant("WorldReflectionViewProj", worldReflectionViewProj.pointer(), 16);

	f32 WaveLength=0.1f;
	services->setVertexShaderConstant("WaveLength", &WaveLength, 1);

	f32 time=Timer->getTime()/100000.0f;
	services->setVertexShaderConstant("Time", &time, 1);

	services->setVertexShaderConstant("WindForce", &WindForce, 1);

	services->setVertexShaderConstant("WindDirection", &WindDirection.X, 2);

	//pixel shader constants
	core::vector3df CameraPosition = SceneManager->getActiveCamera()->getPosition();
	services->setPixelShaderConstant("CameraPosition", &CameraPosition.X, 3);

	services->setPixelShaderConstant("WaveHeight", &WaveHeight, 1);

	services->setPixelShaderConstant("WaterColor", &WaterColor.r, 4);

	services->setPixelShaderConstant("ColorBlendFactor", &ColorBlendFactor, 1);

	//matrix4 projTex=matrix4(
}

void RealisticWaterSceneNode::setWindForce(const f32 windForce)
{
	WindForce=windForce;
}

void RealisticWaterSceneNode::setWindDirection(const core::vector2df& windDirection)
{
	WindDirection=windDirection;
	WindDirection.normalize();
}

void RealisticWaterSceneNode::setWaveHeight(const f32 waveHeight)
{
	WaveHeight=waveHeight;
}

void RealisticWaterSceneNode::setWaterColor(const video::SColorf& waterColor)
{
	WaterColor=waterColor;
}

void RealisticWaterSceneNode::setColorBlendFactor(const f32 colorBlendFactor)
{
	ColorBlendFactor=colorBlendFactor;
}