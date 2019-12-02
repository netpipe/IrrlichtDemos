/*
	Programmed by Do-young Chung

	Programmed Date 080526

	For:

	TSTerrain(Texture Splatting Height Terrain) Mesh and SceneNode

*/

#pragma once

#include <iostream>
#include <tchar.h>
#include <string>
#include <d3dx9.h>
#include <irrlicht.h>


#pragma comment(lib, "Irrlicht.lib")


using namespace irr;


class TSTerrainSceneNode: public scene::ISceneNode, video::IShaderConstantSetCallBack
{

public:

	// Constructor and Deconstructor ------------------------------------------------//

	TSTerrainSceneNode(
		ILogger					*logger,
		scene::ISceneManager	*sceneManager,
		ITimer					*timer,
		const c8*				name,
		const c8*				heightMapFileName,
		const core::dimension2d<f32>&	stretchSize,
		f32								maxHeight,
		const core::dimension2d<s32>&	defaultVertexBlockSize,
		f32 text1Scale, f32 text2Scale, f32 text3Scale, f32 text4Scale,
		scene::ISceneNode* parent = 0, s32 id = -1);

	virtual ~TSTerrainSceneNode();


	// Inherited From scene::ISceneNode ---------------------------------------------//

	virtual video::SMaterial& getMaterial(u32 i);

	virtual u32 getMaterialCount();

	virtual void OnRegisterSceneNode();

	virtual void OnAnimate(u32 timeMs);

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;


	// Inherited From video::IShaderConstantSetCallBack -----------------------------//

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);


	// Original methods -------------------------------------------------------------//

	// Setter And Getter

	scene::SMesh *getMesh();

	// Effect methods

	void setToTransparent(bool doChange);


private:

	// Helper Methods ---------------------------------------------------------------//

	bool loadHeightMap(
		const c8* name,
		video::IImage* heightmap, 
		const core::dimension2d<f32>& stretchSize,
		f32 maxHeight, const core::dimension2d<s32> maxVtxBlockSize,
		bool debugBorders);	

	// Copied From CTerrainSceneNode
	void calculateNormals(scene::SMeshBufferLightMap* pMeshBuffer, core::dimension2d<s32> terrainData);

	
	// Attributes -------------------------------------------------------------------//
	ILogger					*my_logger;

	video::IVideoDriver		*my_videoDriver;

	scene::ISceneManager	*my_sceneManager;

	s32						my_solidTSMaterial;
	s32						my_transparentTSMaterial;

	ITimer					*my_timer;

	core::array<video::SMaterial>	my_materials;

	scene::SMesh					*my_mesh;

	scene::SMeshBufferLightMap		my_RenderBuffer;

	core::dimension2d<f32>	my_size;

	f32						my_detailTextScale;		// Initial value 1.0
	f32						my_lightMapTextScale;	// Initial value 1.0
	f32						my_colorMapTextScale;	// Initial value 1.0
	f32						my_text1Scale;		
	f32						my_text2Scale;
	f32						my_text3Scale;
	f32						my_text4Scale;


	bool					isTransparent;

};