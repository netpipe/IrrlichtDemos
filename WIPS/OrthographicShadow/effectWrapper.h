#ifndef H_XEFFECTS
#define H_XEFFECTS

#include <irrlicht.h>
#include "CShaderPre.h"
#include "CScreenQuad.h"

/// Various effect types. Apply using addEffectToNode();
enum E_EFFECT_TYPE
{
	EET_GOOCH,
	EET_MRWIGGLE,
	EET_ANISO,
	EET_PHONG,
	EET_BRDF,
	EET_COUNT
};

/// Shadow mode enums, sets whether a node recieves shadows, casts shadows, or both.
enum E_SHADOW_MODE
{
	ESM_RECEIVE,
	ESM_CAST,
	ESM_BOTH,
	ESM_COUNT
};

/// Various filter types, up to 16 samples PCF.
enum E_FILTER_TYPE
{
	EFT_NONE,
	EFT_4PCF,
	EFT_8PCF,
	EFT_12PCF,
	EFT_16PCF,
	EFT_COUNT
};

typedef struct
{
	irr::scene::ISceneNode* node;
	E_FILTER_TYPE filterType;
	irr::core::array<irr::s32> materialTypes;
	E_SHADOW_MODE shadowMode;
	bool excludeDepth;
} shadowNode;


/// Main effect handling class, use this apply shadows and effects.
class effectHandler
{
public:

	/// To initialize the effectHandler, pass it the current Irrlicht device, the shadow map size and the folder where the
	/// shaders are contained. You may also pass an alternate size for the post-processing RTT (Using the default initializes
	/// it to the same size as the screen.)
	effectHandler(irr::IrrlichtDevice* dev, irr::core::dimension2d<irr::s32> mapSize, irr::core::stringc shaderFolder = "", 
		irr::core::dimension2d<irr::s32> screenRTTSize = irr::core::dimension2d<irr::s32>(0,0));
	

	/// Sets the shadow lights position.
	void setLightPosition(irr::core::vector3df position)
	{
		lightCam->setPosition(position);
	};

	/// Sets the shadow lights direction.
	void setLightTarget(irr::core::vector3df target)
	{
		lightCam->setTarget(target);
	};

	/// Retrieves the light projection camera.
	irr::scene::ICameraSceneNode* getLightCamera()
	{
		return lightCam;
	};

	/// Adds an effect to a scene node by applying a shader material. Look at the E_EFFECT_TYPE enums for the various effect
	/// types available.
	void addEffectToNode(irr::scene::ISceneNode* node, E_EFFECT_TYPE etype);

	/// Retrieves the shadow map texture.
	irr::video::ITexture* getShadowMapTexture()
	{
		return ShadowMapTex;
	};

	/// Retrieves the screen depth map texture if the depth pass is enabled. This is unrelated to the shadow map, and is
	/// meant to be used for post processing effects that require screen depth info, eg. DOF or SSAO.
	irr::video::ITexture* getDepthMapTexture()
	{
		return DepthRTT;
	};

	/// Sets the maximum shadow distance to the light, this is an important setting, it should be set to the lowest possible
	/// distance that will still cover the entire scene for the best quality.
	void setMaxShadowDistanceFromLight(irr::f32 distance);

	/// Returns the maximum shadow distance from the light.
	irr::f32 getMaxShadowDistanceFromLight() 
	{ 
		return maxdist;
	};

	/// This function is now unrelated to shadow mapping. It simply adds a node to the screen space depth map render, for use
	/// with post processing effects that require screen depth info. If you want the functionality of the old method (A node that
	/// only casts but does not recieve shadows, use addShadowToNode with the ESM_CAST shadow mode.
	void addNodeToDepthPass(irr::scene::ISceneNode *node);

	/// This function is now unrelated to shadow mapping. It simply removes a node to the screen space depth map render, for use
	/// with post processing effects that require screen depth info.
	void removeNodeFromDepthPass(irr::scene::ISceneNode *node);

	/// Sets how dark the shadows should be. A value between 0.0f and 2.0f is recommended. 
	void setShadowDarkness(irr::f32 shadalpha);

	/// Enables/disables an additional pass before applying post processing effects (If there are any) which records screen depth info
	/// to the depth buffer for use with post processing effects that require screen depth info, such as SSAO or DOF. For nodes to be
	/// rendered in this pass, they must first be added using addNodeToDepthPass(SceneNode).
	void enableDepthPass(bool enableDepthPass);

	/// Returns the shadow darkness value.
	irr::f32 getShadowDarkness() 
	{ 
		return shaddark;
	};

	/// Removes shadows from a scene node.
	void removeShadowFromNode(irr::scene::ISceneNode *node);

	/// Updates the effects handler. This must be done between IVideoDriver::beginScene and IVideoDriver::endScene.
	/// This function now replaces smgr->drawAll(). So place it where smgr->drawAll() would normally go. Please note
	/// that the clear colour from IVideoDriver::beginScene is not preserved, so you must instead specify the clear
	/// colour using effectHandler::setClearColour(Colour).
	/// A render target may be passed as the output target, else rendering will commence on the backbuffer.
	void update(irr::video::ITexture* outputTarget = 0);

	/// Adds a shadow to the scene node. There are several conditions for this:
	/// 1. The scene node material type must be of EMT_SOLID, EMT_DETAIL or EMT_LIGHTMAP.
	/// 2. The material type must be explicitly set before applying the shadow map, even if the material type is EMT_SOLID.
	/// Also remember that this will take over the lighting of the scene node, and it will only be affected by the one
	/// shadow map light.
	void addShadowToNode(irr::scene::ISceneNode* node, E_FILTER_TYPE filterType = EFT_NONE, E_SHADOW_MODE shadowMode = ESM_BOTH);
	
	/// Returns the device time divided by 100, for use with the shader callbacks.
	irr::f32 getTime() 
	{ 
		return device->getTimer()->getTime() / 100.0f;
	};
	
	/// Sets the scene clear colour, for when the scene is cleared before smgr->drawAll().
	void setClearColour(irr::video::SColor ClearCol)
	{
		ClearColour = ClearCol;
	};
	
	/**
	A very easy to use post processing function. Simply add a material type to apply to the screen as a post processing
	effect and it will be applied. You can add as many material types as you desire, and they will be double buffered and
	executed in sequance.
	For the material types, I recommend using "ScreenQuadCB" as the callback and refering to the texture names that are passed
	(When using OpenGL, in DirectX uniforms are not required to bind textures).
	Please note that this will only work in OpenGL on vanilla Irrlicht, DX requires the large RTT patch to be able to create
	sufficiently sized rendertargets for post processing. (Or you can just remove the engine check for Pow2).
	
	The structure of the textures is as follows:

	Texture1 - "ColorMapSampler"
	This is passed on from the previous post processing effect as they are executed in sequance. For example, if you do a
	horizontal blur on the first post processing material, then a vertical blur in the second material, you will use this
	sampler to access the post processed data of the horizontal blur when it is time to do the vertical blur. If accessed
	from the first post processing material, it will just contain the untainted screen map data.

	Texture2 - "ScreenMapSampler"
	The second texture will always contain the untainted screen map data, from when the scene is first rendered. It will
	remain unchanged no matter how many post processing materials are applied. This kind of data is necessary, for example
	in bloom or DOF, you would require a copy of the blurred scene data and a copy of the normal untainted, unblurred screen
	data, and mix between them based on certain factors such as depth or luminance.

	Texture3 - "DepthMapSampler"
	If a depth pass has been enabled using enableDepthPass, then this sampler will contain the screen space depth information.
	For better quality this is encoded to 16bits, and can be decoded like so:
		Texture.red + (Texture.green / 256.0f);
	That is by adding the red channel to the green channel which is first divided by 256. 
	The data can still be used without decoding, in 8 bit precision, by just accessing the red component of the texture. Though
	this is not recommended as 8 bit precision is usually not sufficient for modern post processing effects.
	*/

	void addPostProcessingEffect(irr::s32 MaterialType);

	/// Removes the first encountered post processing effect with the specified material type.
	void removePostProcessingEffect(irr::s32 MaterialType)
	{
		for(irr::u32 i = 0;i < PostProcessingRoutines.size();++i)
			if(PostProcessingRoutines[i] == MaterialType)
			{
				PostProcessingRoutines.erase(i);
				break;
			}
	};

	/// Adds a post processing effect by reading a pixel shader from a file. The vertex shader is taken care of.
	void addPostProcessingEffectFromFile(irr::core::stringc filename);

	/// Returns the screen quad scene node. This is not required in any way, but some advanced users may want to adjust
	/// its material settings accordingly.
	CBaseFilter* getScreenQuad() 
	{
		return ScreenQuad;
	};

	/// Sets the active scene manager.
	void setActiveSceneManager(irr::scene::ISceneManager* smgrIn)
	{
		smgr = smgrIn;
	};
	
	// This allows the user to specify a custom, fourth texture to be used in the post-processing effects.
	void setPostProcessingUserTexture(irr::video::ITexture* userTexture)
	{
		if(ScreenQuad)
			ScreenQuad->getMaterial(0).setTexture(3,userTexture);
	};

private:
	irr::IrrlichtDevice* device;
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::core::dimension2d<irr::s32> mapRes;
	irr::scene::ICameraSceneNode* lightCam;
	irr::core::array<shadowNode> shadowNodeArray;
	irr::core::array<irr::scene::ISceneNode*> DepthPassArray;
	irr::s32 Depth;
	irr::s32 Detail;
	irr::s32 DetailPCF4;
	irr::s32 DetailPCF8;
	irr::s32 DetailPCF12;
	irr::s32 DetailPCF16;
	irr::s32 Solid;
	irr::s32 SolidPCF4;
	irr::s32 SolidPCF8;
	irr::s32 SolidPCF12;
	irr::s32 SolidPCF16;
	irr::s32 Gooch;
	irr::s32 Phong;
	irr::s32 Brdf;
	irr::s32 Aniso;
	irr::s32 Wiggle;
	irr::s32 DepthWiggle;
	irr::f32 maxdist;
	irr::f32* depthLink;
	irr::f32 shaddark;
	irr::video::ITexture* ShadowMapTex;
	irr::video::ITexture* ScreenRTT;
	irr::video::ITexture* DepthRTT;
	irr::core::array<irr::s32> PostProcessingRoutines;
	irr::core::array<irr::s32> BufferMaterialList;
	irr::video::SColor ClearColour;
	irr::core::dimension2d<irr::s32> ScreenRTTSize;
	irr::core::matrix4* ViewLink;
	irr::core::matrix4* ProjLink;
	CBaseFilter* ScreenQuad;
	bool shadowsUnsupported;
	bool DepthPass;
};

#endif

// Copyright (C) Ahmed Hilali 2008