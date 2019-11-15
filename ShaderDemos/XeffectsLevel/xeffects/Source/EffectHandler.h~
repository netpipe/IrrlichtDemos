#ifndef H_XEFFECTS_EFFECT_HANDLER
#define H_XEFFECTS_EFFECT_HANDLER

#include <irrlicht.h>
#include "CShaderPre.h"
#include "CScreenQuad.h"

/// Shadow mode enums, sets whether a node recieves shadows, casts shadows, or both.
/// If the mode is ESM_CAST, it will not be affected by shadows or lighting.
enum E_SHADOW_MODE
{
	ESM_RECEIVE,
	ESM_CAST,
	ESM_BOTH,
	ESM_EXCLUDE,
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

struct SShadowLight
{
	/// Shadow light constructor. The first parameter is the square shadow map resolution.
	/// This should be a power of 2 number, and within reasonable size to achieve optimal
	/// performance and quality. Recommended sizes are 512 to 4096 subject to your target
	/// hardware and quality requirements. The next two parameters are position and target,
	/// the next one is the light color. The next two are very important parameters,
	/// the far value and the near value. The higher the near value, and the lower the
	/// far value, the better the depth precision of the shadows will be, however it will
	/// cover a smaller volume. The next is the FOV, if the light was to be considered
	/// a camera, this would be similar to setting the camera's field of view. The last
	/// parameter is whether the light is directional or not, if it is, an orthogonal
	/// projection matrix will be created instead of a perspective one.
	SShadowLight(	const irr::u32 shadowMapResolution,
					const irr::core::vector3df& position, 
					const irr::core::vector3df& target,
					irr::video::SColorf lightColour = irr::video::SColor(0xffffffff), 
					irr::f32 nearValue = 10.0, irr::f32 farValue = 100.0,
					irr::f32 fov = 90.0 * irr::core::DEGTORAD64, bool directional = false)
					:	pos(position), tar(target), farPlane(directional ? 1.0f : farValue), diffuseColour(lightColour), 
						mapRes(shadowMapResolution)
	{
		nearValue = nearValue <= 0.0f ? 0.1f : nearValue;

		updateViewMatrix();
		
		if(directional)
			projMat.buildProjectionMatrixOrthoLH(fov, fov, nearValue, farValue);
		else
			projMat.buildProjectionMatrixPerspectiveFovLH(fov, 1.0f, nearValue, farValue);
	}

	/// Sets the light's position.
	void setPosition(const irr::core::vector3df& position)
	{
		pos = position;
		updateViewMatrix();
	}

	/// Sets the light's target.
	void setTarget(const irr::core::vector3df& target)
	{
		tar = target;
		updateViewMatrix();
	}

	/// Gets the light's position.
	const irr::core::vector3df& getPosition() const
	{
		return pos;
	}

	/// Gets the light's target.
	const irr::core::vector3df& getTarget()  const
	{
		return tar;
	}

	/// Sets the light's view matrix.
	void setViewMatrix(const irr::core::matrix4& matrix)
	{
		viewMat = matrix;
		irr::core::matrix4 vInverse;
		viewMat.getInverse(vInverse);
		pos = vInverse.getTranslation();
	}

	/// Sets the light's projection matrix.
	void setProjectionMatrix(const irr::core::matrix4& matrix)
	{
		projMat = matrix;
	}

	/// Gets the light's view matrix.
	irr::core::matrix4& getViewMatrix()
	{
		return viewMat;
	}

	/// Gets the light's projection matrix.
	irr::core::matrix4& getProjectionMatrix()
	{
		return projMat;
	}

	/// Gets the light's far value.
	irr::f32 getFarValue() const
	{
		return farPlane;
	}

	/// Gets the light's color.
	const irr::video::SColorf& getLightColor() const
	{
		return diffuseColour;
	}

	/// Sets the light's color.
	void setLightColor(const irr::video::SColorf& lightColour) 
	{
		diffuseColour = lightColour;
	}

	/// Sets the shadow map resolution for this light.
	void setShadowMapResolution(const irr::u32 shadowMapResolution)
	{
		mapRes = shadowMapResolution;
	}

	/// Gets the shadow map resolution for this light.
	const irr::u32 getShadowMapResolution() const
	{
		return mapRes;
	}

private:

	void updateViewMatrix()
	{
		viewMat.buildCameraLookAtMatrixLH(pos, tar,
			(pos - tar).dotProduct(irr::core::vector3df(1.0f, 0.0f, 1.0f)) == 0.0f ?
			irr::core::vector3df(0.0f, 0.0f, 1.0f) : irr::core::vector3df(0.0f, 1.0f, 0.0f)); 
	}

	irr::video::SColorf diffuseColour;
	irr::core::vector3df pos, tar;
	irr::f32 farPlane;
	irr::core::matrix4 viewMat, projMat;
	irr::u32 mapRes;
};

// This is a general interface that can be overidden if you want to perform operations before or after
// a specific post-processing effect. You will be passed an instance of the EffectHandler.
// The function names themselves should be self-explanatory ;)
class EffectHandler;
class IPostProcessingRenderCallback
{
public:
	virtual void OnPreRender(EffectHandler* effect) = 0;
	virtual void OnPostRender(EffectHandler* effect) = 0;
};

// Shader callback prototypes.
class DepthShaderCB;
class ShadowShaderCB;
class ScreenQuadCB;

/// Main effect handling class, use this to apply shadows and effects.
class EffectHandler
{
public:

	/*	EffectHandler constructor. Initializes the EffectHandler.

		Parameters:
		irrlichtDevice: Current Irrlicht device.
		screenRTTSize: Size of screen render target for post processing. Default is screen size.
		useVSMShadows: Shadows will use VSM filtering. It is recommended to only use EFT_NONE when this is enabled.
		useRoundSpotlights: Shadow lights will have a soft round spot light mask. Default is false.
		use32BitDepthBuffers: XEffects will use 32-bit depth buffers if this is true, otherwise 16-bit. Default is false.
	*/
	EffectHandler(irr::IrrlichtDevice* irrlichtDevice, 
		const irr::core::dimension2du& screenRTTSize = irr::core::dimension2du(0, 0),
		const bool useVSMShadows = false, const bool useRoundSpotLights = false,
		const bool use32BitDepthBuffers = false);
	
	/// Destructor.
	~EffectHandler();

	/// Adds a shadow light. Check out the shadow light constructor for more information.
	void addShadowLight(const SShadowLight& shadowLight)
	{
		LightList.push_back(shadowLight);
	}

	/// Retrieves a reference to a shadow light. You may get the max amount from getShadowLightCount.
	SShadowLight& getShadowLight(irr::u32 index)
	{
		return LightList[index];
	}

	/// Retrieves the current number of shadow lights.
	const irr::u32 getShadowLightCount() const
	{
		return LightList.size();
	}

	/// Retrieves the shadow map texture for the specified square shadow map resolution.
	/// Only one shadow map is kept for each resolution, so if multiple lights are using
	/// the same resolution, you will only see the last light drawn's output.
	/// The secondary param specifies whether to retrieve the secondary shadow map used in blurring.
	irr::video::ITexture* getShadowMapTexture(const irr::u32 resolution, const bool secondary = false);

	/// Retrieves the screen depth map texture if the depth pass is enabled. This is unrelated to the shadow map, and is
	/// meant to be used for post processing effects that require screen depth info, eg. DOF or SSAO.
	irr::video::ITexture* getDepthMapTexture()
	{
		return DepthRTT;
	}

	/// This function is now unrelated to shadow mapping. It simply adds a node to the screen space depth map render, for use
	/// with post processing effects that require screen depth info. If you want the functionality of the old method (A node that
	/// only casts but does not recieve shadows, use addShadowToNode with the ESM_CAST shadow mode.
	void addNodeToDepthPass(irr::scene::ISceneNode *node);

	/// This function is now unrelated to shadow mapping. It simply removes a node to the screen space depth map render, for use
	/// with post processing effects that require screen depth info.
	void removeNodeFromDepthPass(irr::scene::ISceneNode *node);

	/// Enables/disables an additional pass before applying post processing effects (If there are any) which records screen depth info
	/// to the depth buffer for use with post processing effects that require screen depth info, such as SSAO or DOF. For nodes to be
	/// rendered in this pass, they must first be added using addNodeToDepthPass(SceneNode).
	void enableDepthPass(bool enableDepthPass);

	/// Removes shadows from a scene node.
	void removeShadowFromNode(irr::scene::ISceneNode* node)
	{
		SShadowNode tmpShadowNode = {node, ESM_RECEIVE, EFT_NONE};
		irr::s32 i = ShadowNodeArray.binary_search(tmpShadowNode);

		if(i != -1)
			ShadowNodeArray.erase(i);
	}

	// Excludes a scene node from lighting calculations, avoiding any side effects that may
	// occur from XEffect's light modulation on this particular scene node.
	void excludeNodeFromLightingCalculations(irr::scene::ISceneNode* node)
	{
		SShadowNode tmpShadowNode = {node, ESM_EXCLUDE, EFT_NONE};
		ShadowNodeArray.push_back(tmpShadowNode);
	}

	/// Updates the effects handler. This must be done between IVideoDriver::beginScene and IVideoDriver::endScene.
	/// This function now replaces smgr->drawAll(). So place it where smgr->drawAll() would normally go. Please note
	/// that the clear colour from IVideoDriver::beginScene is not preserved, so you must instead specify the clear
	/// colour using EffectHandler::setClearColour(Colour).
	/// A render target may be passed as the output target, else rendering will commence on the backbuffer.
	void update(irr::video::ITexture* outputTarget = 0);

	/// Adds a shadow to the scene node. The filter type specifies how many shadow map samples
	/// to take, a higher value can produce a smoother or softer result. The shadow mode can
	/// be either ESM_BOTH, ESM_CAST, or ESM_RECEIVE. ESM_BOTH casts and receives shadows,
	/// ESM_CAST only casts shadows, and is unaffected by shadows or lighting, and ESM_RECEIVE
	/// only receives but does not cast shadows.
	void addShadowToNode(irr::scene::ISceneNode* node, E_FILTER_TYPE filterType = EFT_NONE, E_SHADOW_MODE shadowMode = ESM_BOTH);
	
	/// Returns the device time divided by 100, for use with the shader callbacks.
	irr::f32 getTime() 
	{ 
		return device->getTimer()->getTime() / 100.0f;
	}
	
	/// Sets the scene clear colour, for when the scene is cleared before smgr->drawAll().
	void setClearColour(irr::video::SColor ClearCol)
	{
		ClearColour = ClearCol;
	}
	
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
	
	Texture4 - "UserMapSampler"
	A custom texture that can be set by the user using setPostProcessingUserTexture(irr::video::ITexture* userTexture).

	The last parameter is the render callback, you may pass 0 if you do not need one.
	Please see IPostProcessingRenderCallback for more info about this callback.
	*/
	void addPostProcessingEffect(irr::s32 MaterialType, IPostProcessingRenderCallback* callback = 0);

	/// Sets the IPostProcessingRenderCallback for the specified post processing effect.
	/// The old callback if previously set will be automatically deleted.
	void setPostProcessingRenderCallback(irr::s32 MaterialType, IPostProcessingRenderCallback* callback = 0)
	{
		SPostProcessingPair tempPair(MaterialType, 0);
		irr::s32 i = PostProcessingRoutines.binary_search(tempPair);

		if(i != -1)
		{
			if(PostProcessingRoutines[i].renderCallback)
				delete PostProcessingRoutines[i].renderCallback;

			PostProcessingRoutines[i].renderCallback = callback;
		}
	}

	/// Removes the first encountered post processing effect with the specified material type.
	void removePostProcessingEffect(irr::s32 MaterialType)
	{
		SPostProcessingPair tempPair(MaterialType, 0);
		irr::s32 i = PostProcessingRoutines.binary_search(tempPair);

		if(i != -1)
		{
			if(PostProcessingRoutines[i].renderCallback)
				delete PostProcessingRoutines[i].renderCallback;

			PostProcessingRoutines.erase(i);
		}
	}

	/// Adds a post processing effect by reading a pixel shader from a file. The vertex shader is taken care of.
	/// The vertex shader will pass the correct screen quad texture coordinates via the TEXCOORD0 semantic in
	/// Direct3D or the gl_TexCoord[0] varying in OpenGL.
	/// See addPostProcessingEffect for more info.
	/// Returns the Irrlicht material type of the post processing effect.
	irr::s32 addPostProcessingEffectFromFile(const irr::core::stringc& filename,
		IPostProcessingRenderCallback* callback = 0);

	/// Sets a shader parameter for a post-processing effect. The first parameter is the material type, the second
	/// is the uniform paratmeter name, the third is a float pointer that points to the data and the last is the
	/// component count of the data. Please note that the float pointer must remain valid during render time.
	/// To disable the setting of a parameter you may simply pass a null float pointer.
	void setPostProcessingEffectConstant(const irr::s32 materialType, const irr::core::stringc& name, const irr::f32* data,
		const irr::u32 count);

	/// Returns the screen quad scene node. This is not required in any way, but some advanced users may want to adjust
	/// its material settings accordingly.
	const CScreenQuad& getScreenQuad() 
	{
		return ScreenQuad;
	}

	/// Sets the active scene manager.
	void setActiveSceneManager(irr::scene::ISceneManager* smgrIn)
	{
		smgr = smgrIn;
	}

	/// Gets the active scene manager.
	irr::scene::ISceneManager* getActiveSceneManager()
	{
		return smgr;
	}
	
	/// This allows the user to specify a custom, fourth texture to be used in the post-processing effects.
	/// See addPostProcessingEffect for more info.
	void setPostProcessingUserTexture(irr::video::ITexture* userTexture)
	{
		ScreenQuad.getMaterial().setTexture(3, userTexture);
	}

	/// Sets the global ambient color for shadowed scene nodes.
	void setAmbientColor(irr::video::SColor ambientColour)
	{
		AmbientColour = ambientColour;
	}

	/// Gets the global ambient color.
	irr::video::SColor getAmbientColor() const
	{
		return AmbientColour;
	}

	/// Generates a randomized texture composed of uniformly distributed 3 dimensional vectors.
	irr::video::ITexture* generateRandomVectorTexture(const irr::core::dimension2du& dimensions,
		const irr::core::stringc& name = "randVec");

	/// Sets a new screen render target resolution.
	void setScreenRenderTargetResolution(const irr::core::dimension2du& resolution);

	/// Returns the device that this EffectHandler was initialized with.
	irr::IrrlichtDevice* getIrrlichtDevice() {return device;}

private:

	struct SShadowNode
	{
		bool operator < (const SShadowNode& other) const
		{
			return node < other.node;
		}

		irr::scene::ISceneNode* node;

		E_SHADOW_MODE shadowMode;
		E_FILTER_TYPE filterType;
	};

	struct SPostProcessingPair
	{
		SPostProcessingPair(const irr::s32 materialTypeIn, ScreenQuadCB* callbackIn,
			IPostProcessingRenderCallback* renderCallbackIn = 0)
			: materialType(materialTypeIn), callback(callbackIn), renderCallback(renderCallbackIn) {}

		bool operator < (const SPostProcessingPair& other) const
		{
			return materialType < other.materialType;
		}

		ScreenQuadCB* callback;
		IPostProcessingRenderCallback* renderCallback;
		irr::s32 materialType;
	};

	SPostProcessingPair obtainScreenQuadMaterialFromFile(const irr::core::stringc& filename, 
		irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID);

	irr::IrrlichtDevice* device;
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::core::dimension2du mapRes;
	
	irr::s32 Depth;
	irr::s32 DepthT;
	irr::s32 DepthWiggle;
	irr::s32 Shadow[EFT_COUNT];
	irr::s32 LightModulate;
	irr::s32 Simple;
	irr::s32 WhiteWash;
	irr::s32 WhiteWashTRef;
	irr::s32 WhiteWashTAdd;
	irr::s32 WhiteWashTAlpha;
	irr::s32 VSMBlurH;
	irr::s32 VSMBlurV;
	
	DepthShaderCB* depthMC;
	ShadowShaderCB* shadowMC;

	irr::video::ITexture* ScreenRTT;
	irr::video::ITexture* DepthRTT;

	irr::core::array<SPostProcessingPair> PostProcessingRoutines;
	irr::core::array<SShadowLight> LightList;
	irr::core::array<SShadowNode> ShadowNodeArray;
	irr::core::array<irr::scene::ISceneNode*> DepthPassArray;

	irr::core::dimension2du ScreenRTTSize;
	irr::video::SColor ClearColour;
	irr::video::SColor AmbientColour;
	CScreenQuad ScreenQuad;

	bool shadowsUnsupported;
	bool use32BitDepth;
	bool useVSM;
	bool DepthPass;
};

#endif

// Copyright (C) 2007-2009 Ahmed Hilali 