#include <irrlicht.h>

enum E_EFFECT_TYPE
{
	EET_GOOCH,
	EET_MRWIGGLE,
	EET_ANISO,
	EET_PHONG,
	EET_BRDF,
	EET_COUNT
};

enum E_FILTER_TYPE
{
	EFT_NONE,
	EFT_4PCF,
	EFT_8PCF,
	EFT_12PCF,
	EFT_COUNT
};

typedef struct
{
	irr::scene::ISceneNode* node;
	E_FILTER_TYPE filterType;
	irr::s32 materialType;
	bool depthOnly;
	bool exclude;
} shadowNode;


class effectHandler
{
public:
	effectHandler(irr::IrrlichtDevice* dev, irr::core::dimension2du mapSize, bool useAccurate = true);

	void setLightPosition(irr::core::vector3df position)
	{
		lightCam->setPosition(position);
	};

	void setLightTarget(irr::core::vector3df target)
	{
		lightCam->setTarget(target);
	};

	irr::scene::ICameraSceneNode* getLightCamera()
	{
		return lightCam;
	};

	void addEffectToNode(irr::scene::ISceneNode* node, E_EFFECT_TYPE etype);

	irr::video::ITexture* getShadowMapTexture()
	{
		return ShadowMapTex;
	};

	void setMaxShadowDistanceFromLight(int distance);
	void addNodeToDepthPass(irr::scene::ISceneNode *node);
	void excludeNodeFromDepthPass(irr::scene::ISceneNode *node);
	void setShadowDarkness(float shadalpha);
	void removeShadowFromNode(irr::scene::ISceneNode *node);
	void update();

	void enableFading(float fadeval = 1.5f);
	void disableFading();

	void addShadowToNode(irr::scene::ISceneNode* node, E_FILTER_TYPE filterType = EFT_NONE);
private:
	irr::IrrlichtDevice* device;
	irr::core::dimension2d<irr::s32> mapRes;
	irr::scene::ICameraSceneNode* lightCam;
	irr::scene::ICameraSceneNode* cameraBuffer;
	irr::core::array<shadowNode> shadowNodeArray;
	irr::s32 Depth;
	irr::s32 Detail;
	irr::s32 DetailPCF4;
	irr::s32 DetailPCF8;
	irr::s32 DetailPCF12;
	irr::s32 DetailStore;
	irr::s32 DetailPCF4Store;
	irr::s32 DetailPCF8Store;
	irr::s32 DetailPCF12Store;
	irr::s32 Solid;
	irr::s32 SolidPCF4;
	irr::s32 Gooch;
	irr::s32 Phong;
	irr::s32 Brdf;
	irr::s32 Aniso;
	irr::s32 Wiggle;
	irr::s32 DepthWiggle;
	irr::video::ITexture* ShadowMapTex;
	irr::video::ITexture* ShadowMapBuffer;
};

// This code is covered by a zlib-style license, please find enclosed "License.txt"
// Copyright (C) Ahmed Hilali 2007
