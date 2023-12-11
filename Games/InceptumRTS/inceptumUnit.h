#pragma once
#include <irrlicht.h>
#include "inceptumUnit.h"

using namespace irr;

class UnitClass
{
protected:
	static IrrlichtDevice* device;
	static scene::ISceneManager* smgr;
	static video::IVideoDriver* driver;
	static scene::ITerrainSceneNode* terrain;
	static f32* deltaTime;
	// ====================================================================
	scene::IAnimatedMeshSceneNode* node;
	// meshes // ==========================================================
	static scene::IAnimatedMesh* mesh1;
	static video::ITexture* texture1_1;
	static video::ITexture* texture1_2;
	// ====================================================================
	scene::ITriangleSelector* selector;
public:
	static void setPointers(IrrlichtDevice* const deviceIn,
		scene::ITerrainSceneNode* const terrainIn, f32* const deltaTimeIn);
	void Initialize(core::vector2d<f32> initPos, f32 initRot, bool Sph);
	void setPosition(core::vector3df position);
	bool compareNodes(scene::ISceneNode* inNode);
};

