#include <irrlicht.h>
#include <vector>
#include "MetaballSceneNode.h"
#include <iostream>

#pragma comment(lib,"irrlicht.lib")
#define ONLY_METABALLS


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

// This is my first time creating an "enclosed application class" style demo.
// I'm finding it very convenient for things like handling events and shader callbacks
// because you don't have to worry about passing certain variables to those classes
// or using ugly global variables. It also looks alot prettier than sticking everything
// in "int main()", that's probably the main reason why I chose to do it this way. :P
class RefractApp : public irr::IEventReceiver, irr::video::IShaderConstantSetCallBack
{
public:
	RefractApp()
	{

		//printf("Enter sample width or enter 'd' to leave default (0.25):");
		//float SampleWidth = 0.0f;
		//std::cin >> SampleWidth;

		// Create the Irrlicht device.
		device = createDevice(EDT_OPENGL,dimension2du(800,600),32);

		// Yes, this very class is the event reciever.
		device->setEventReceiver(this);

		smgr = device->getSceneManager();
		driver = device->getVideoDriver();

		ISceneNodeFactory *metaFactory = new MetaballSceneNodeFactory(smgr);
		smgr->registerSceneNodeFactory(metaFactory);

		meta = (MetaballSceneNode*)smgr->addSceneNode("MetaballSceneNode", 0);


		meta->setSampleWidth(0.25f);//SampleWidth == 0.0f ? .25f : SampleWidth);
		meta->setBound(3);
		meta->setThreshold(0.8);

		meta->addMetaball(vector3df(1, -1, 1), SColor(255, 0, 0, 255));
		meta->addMetaball(vector3df(-1, -1, -1), SColor(255, 0, 255, 0));
		meta->addMetaball(vector3df(0, 1, 0), SColor(255, 255, 0, 0));
		meta->triangulateVolume();

		meta->setScale(vector3df(100, 100, 100));
		meta->setRotation(vector3df(0, 45, 0));
		meta->setPosition(vector3df(3000,1500,3000));

		//meta->getMaterial(0).Wireframe = true;

		// The following is straight from the terrain tutorial, I'm sure you're all very
		// familiar with it.
		ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0,100,0.5);

		camera->setPosition(core::vector3df(3500,1200,4300));

#ifndef ONLY_METABALLS
		camera->setTarget(core::vector3df(2397*2,1200,2700*2));
#else
		camera->setTarget(meta->getPosition());
#endif
		camera->setFarValue(12000.0f);


		// Skyboxes have a weird as bug when they are projected from an RTT, so I decided
		// to use a skydome. It's also alot prettier. :P
		driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
#ifndef ONLY_METABALLS
		smgr->addSkyDomeSceneNode(driver->getTexture("./SkyDome.jpg"),16,16,1.0,1.5);
#else
		smgr->addSkyDomeSceneNode(driver->getTexture("./SkyDome.jpg"),16,16,1.0,2.0);
#endif
		driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);


		// Here I make a RTT for the refraction, you can use a higher res one if you want,
		// I chose 512^2 for compatibility. I also load the normalmap.
		ITexture* RTTTex = driver->addRenderTargetTexture(dimension2du(512,512));

		meta->getMaterial(0).setTexture(0, RTTTex);
		meta->getMaterial(0).BackfaceCulling = false;
		//meta->getMaterial(0).setTexture(1,driver->getTexture("media/SkyDome.jpg"));

		video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

		// I create the shader material for the metaball.
//		s32 BallMat = gpu->addHighLevelShaderMaterialFromFiles(
	//		"BallV.glsl","main",EVST_VS_2_0,"BallP.glsl","main",
	//		EPST_PS_2_0,this,EMT_SOLID,3);

	//	meta->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)BallMat;

#ifndef ONLY_METABALLS
		// add terrain scene node
		scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"media/terrain-heightmap.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0.f, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(40.f, 4.4f, 40.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		4										// smoothFactor
		);

		terrain->setMaterialFlag(video::EMF_LIGHTING, false);
		terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
		terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));
		terrain->setMaterialType(video::EMT_DETAIL_MAP);
		terrain->scaleTexture(1.0f, 20.0f);

		// create triangle selector for the terrain
		scene::ITriangleSelector* selector
			= smgr->createTerrainTriangleSelector(terrain, 0);
		terrain->setTriangleSelector(selector);
		selector->drop();

		// create collision response animator and attach it to the camera
		scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, camera, core::vector3df(60,100,60),
			core::vector3df(0,0,0),
			core::vector3df(0,50,0));
		camera->addAnimator(anim);
		anim->drop();


		// I'm just using a basic cube scene node for the glass pane, "scaled to flatness".
		ISceneNode* GlassPane = smgr->addCubeSceneNode();
		GlassPane->setScale(vector3df(100,150,1));
		GlassPane->setPosition(core::vector3df(2397*2,1400,2700*2));
		GlassPane->setRotation(vector3df(0,60,0));

		ITexture* NormMap = driver->getTexture("media/NormalMap.png");

		GlassPane->setMaterialTexture(0, RTTTex);
		GlassPane->setMaterialTexture(1, NormMap);

		ISceneNode* GlassMesh = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/dwarf.x"));
		GlassMesh->setPosition(core::vector3df(2400*2,1200,2000*2));
		GlassMesh->setScale(core::vector3df(10,10,10));
		GlassMesh->setMaterialTexture(0, RTTTex);
		GlassMesh->setMaterialTexture(1, NormMap);


		// I create the shader material for the glass pane.
		s32 GlassMat = gpu->addHighLevelShaderMaterialFromFiles(
			"GlassV.glsl","main",EVST_VS_2_0,"GlassP.glsl","main",
			EPST_PS_2_0,this);

		GlassPane->setMaterialType(E_MATERIAL_TYPE(GlassMat));
		GlassMesh->setMaterialType(E_MATERIAL_TYPE(GlassMat));

		// Create a particle system and emitter for the raindrops, I won't go into detail
		// here as it's covered in many other tutorials.
		scene::IParticleSystemSceneNode* PSys = smgr->addParticleSystemSceneNode(false);

		PSys->setPosition(core::vector3df(2000*2,1400,2200*2));

		scene::IParticleEmitter* PEmit = PSys->createPointEmitter(vector3df(0,0.3f,0),50,400,SColor(255,255,255,255),
			SColor(255,255,255,255),2000,4000,30);

		//scene::IParticleEmitter* PEmit = PSys->createBoxEmitter(aabbox3df(-10,28,-10,10,30,10),vector3df(0,0,0),500,1000);
		PSys->setEmitter(PEmit);
		//PSys->setParent(camera);
		PSys->setParticlesAreGlobal(true);
		PSys->setParticleSize(dimension2d<f32>(20,20));
		PSys->addAffector(PSys->createGravityAffector(vector3df(0,-0.18f,0)));
		PSys->addAffector(PSys->createFadeOutParticleAffector());
		PSys->addAnimator(smgr->createFlyCircleAnimator(PSys->getPosition(),150,0.002f));

		// Apply a shader material to the particles. Make sure the base material is
		// EMT_TRANSPARENT_ALPHA_CHANNEL so that it takes the output alpha colour
		// into account.
		s32 DropMat = gpu->addHighLevelShaderMaterialFromFiles(
			"DropV.glsl","main",EVST_VS_2_0,"DropP.glsl","main",
			EPST_PS_2_0,this,EMT_TRANSPARENT_ALPHA_CHANNEL);

		PSys->setMaterialType(E_MATERIAL_TYPE(DropMat));

		// I load the bumpmap for the particles. This looks like a single round bump,
		// it also has an alpha channel encoded in it that's used for the particle
		// transparency.
		ITexture* ParticleNormMap = driver->getTexture("media/ParticleBump.png");
		PSys->setMaterialTexture(0, RTTTex);
		PSys->setMaterialTexture(1, ParticleNormMap);

#endif

		// Set the tint colour, I like it pure white, but you may want blue or
		// similar. (Maybe red for blood effects with the raindrops?).
		TintColour = SColorf(1.0f,1.0f,1.0f,1.0f);

		FreezeMovement = false;
		while(device->run())
		{
			if(device->isWindowActive())
			{
				//const int BallsSize = meta->Balls.size();
				//for(int i = 0;i < BallsSize;++i)
				//	meta->Balls[i] += meta->Balls[i] * 0.01f;

				if(!FreezeMovement)
				{
					f32 currSin = sin(float(device->getTimer()->getTime() / 1000.0f)) * 1.5f;
					meta->Balls[0] = vector3df(1, -1, 1) * currSin;
					meta->Balls[1] = vector3df(-1, -1, -1) * currSin;
					meta->Balls[2] = vector3df(0, 1, 0) * currSin;
					meta->triangulateVolume();
				}

				driver->beginScene(false,false,SColor(0,0,0,0));

				// First we render the scene to our rendertarget.
				// Make sure to set the glasspane and particles systems non-visible.
				driver->setRenderTarget(RTTTex, true, true, SColor(255,100,100,200));


#ifndef ONLY_METABALLS
					// Hide the glass pane.
					GlassPane->setVisible(false);
					GlassMesh->setVisible(false);
#endif
					meta->setVisible(false);

					// For some reason the particle system has a bug with setVisible,
					// so instead we force the particles invisible with a shader uniform.
					// Its not very efficient but it gets the job done until particles are fixed.
					ForceAlphaTo0 = true;

					// Draw the scene.
					smgr->drawAll();

				// Then we switch back to the screen and set our glass pane and
				// particles back to visible again.
				driver->setRenderTarget(0, true, true, SColor(255,100,100,200));


#ifndef ONLY_METABALLS
					// Show the glass pane.
					GlassPane->setVisible(true);
					GlassMesh->setVisible(true);
#endif

					meta->setVisible(true);

					// Show the particles.
					ForceAlphaTo0 = false;

					// Draw the scene.
					smgr->drawAll();
					//driver->draw3DBox(meta->getBoundingBox());
				driver->endScene();

				if(driver->getFPS() != OldFps)
				{
					OldFps = driver->getFPS();
					core::stringw WindowCaption = "Irrlicht MetaBall Demo FPS: ";
					WindowCaption += OldFps;
					WindowCaption += " SampleWidth (Z/X): ";
					WindowCaption += meta->SampleWidth;
					WindowCaption += " Space freezes balls.";

					device->setWindowCaption(WindowCaption.c_str());
				}
			}
		}
	};

	~RefractApp()
	{
		// Drop the device when the app is destroyed.
		device->drop();
		device = NULL;
	}

	// Here we can change the tint colour based on key input.
	virtual bool OnEvent(const irr::SEvent& event)
	{
		if(event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			if(!event.KeyInput.PressedDown)
			{
				switch(event.KeyInput.Key)
				{
				case irr::KEY_KEY_1:
					TintColour = SColorf(1.0f,0.5f,0.5f,1.0f);
					break;
				case irr::KEY_KEY_2:
					TintColour = SColorf(0.5f,1.0f,0.5f,1.0f);
					break;
				case irr::KEY_KEY_3:
					TintColour = SColorf(0.5f,0.5f,1.0f,1.0f);
					break;
				case irr::KEY_KEY_4:
					TintColour = SColorf(1.0f,1.0f,1.0f,1.0f);
					break;
				case irr::KEY_KEY_Z:
					if(meta->SampleWidth > 0.1f)
					{
						meta->setSampleWidth(meta->SampleWidth - 0.025f);
						OldFps = 0;
						if(FreezeMovement)
							meta->triangulateVolume();
					}
					break;
				case irr::KEY_KEY_X:
					if(meta->SampleWidth < 0.25f)
					{
						meta->setSampleWidth(meta->SampleWidth + 0.025f);
						OldFps = 0;
						if(FreezeMovement)
							meta->triangulateVolume();
					}
					break;
				case irr::KEY_SPACE:
					FreezeMovement = !FreezeMovement;
					break;
				}
			}
		}

		return false;
	}

	virtual void OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData)
	{
		// Here we are setting the necessary shader constants.
		// Both shaders require the same information, so they share this function.

		if(userData == 3 && meta)
		{
			vector3df BallPos = meta->Balls[0];
			services->setVertexShaderConstant("Ball1", &(BallPos.X), 3);
			BallPos = meta->Balls[1];
			services->setVertexShaderConstant("Ball2", &(BallPos.X), 3);
			BallPos = meta->Balls[2];
			services->setVertexShaderConstant("Ball3", &(BallPos.X), 3);
		}

		WorldMat = driver->getTransform(ETS_WORLD);
		services->setVertexShaderConstant("mWorld", WorldMat.pointer(), 16);

		vector3df CamPos = smgr->getActiveCamera()->getPosition();
		services->setVertexShaderConstant("CamPos", &CamPos.X, 4);

		float AlphaAdjust = 1.0f;

		if(ForceAlphaTo0)
			AlphaAdjust = 0.0f;

		services->setVertexShaderConstant("AlphaAdjust", &AlphaAdjust, 1);

		int TexAddress = 0;
		services->setPixelShaderConstant("RTTexture", (float*)(&TexAddress), 1);
		int TexAddress2 = 1;
		services->setPixelShaderConstant("NormalMap", (float*)(&TexAddress2), 1);

		// Set the tint colour.
		services->setPixelShaderConstant("TintColour", (float*)(&TintColour.r), 4);
	}

	// Member > Global variables :D
	int OldFps;
	MetaballSceneNode* meta;
	bool ForceAlphaTo0;
	SColorf TintColour;
	IrrlichtDevice* device;
	ISceneManager* smgr;
	IVideoDriver* driver;
	matrix4 WorldMat;
	bool FreezeMovement;
};

int main ()
//int main()
{
	// I didn't bother with App.run() etc, constructors/destructors are there for a reason.
	RefractApp App;
	return 0;
}
