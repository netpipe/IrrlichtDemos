// Grass node by Gaz Davidson 2006

#ifndef __C_GRASS_PATCH_SCENE_NODE_H_INCLUDED__
#define __C_GRASS_PATCH_SCENE_NODE_H_INCLUDED__

#define GRASS_PATCH_SIZE 1024

//#include <irrlicht.h>
#include "IWindGenerator.h"

namespace irr
{
namespace scene
{

struct SGrassParticle
{
	video::SColor          color;      // current colour of this blade
	video::SColor          startColor; // colour it starts off at
	core::vector3df        pos;        // its position
	core::dimension2d<s32> sprite;     // the sprite number (x and y)
	core::vector3df        points[4];  // position of points
	f32                    flex;       // how flexible this node is 0 (stiff) to 1 (bendy)
};

class CGrassPatchSceneNode : public ISceneNode
{

public:

	//! constructor
	CGrassPatchSceneNode(
		ITerrainSceneNode* const terrain, ISceneManager* mgr, const s32 id, const core::vector3d<s32>& gridpos,
		const char* filepath, const video::IImage* const heightMap, const video::IImage* const colourMap,
		const video::IImage* const grassMap, scene::IWindGenerator* const windgen);

	//! destructor
	virtual ~CGrassPatchSceneNode();

	//! Returns the material based on the zero based index i.
	virtual video::SMaterial& getMaterial(u32 i);

	//! Returns amount of materials used by this scene node.
	virtual u32 getMaterialCount() const;

	//! pre render
	virtual void OnRegisterSceneNode();

	//! Animate the clouds
	virtual void OnAnimate(u32 timeMs);

	//! render
	virtual void render();

	//! tweak grass node performance
	void setDrawDistance(const f32 draw);

	f32  getDrawDistance() const;

	void setMaxDensity(const u32 max);

	u32  getMaxDensity() const;

	void setRefreshDelay(const u32 ms);

	u32  getRefreshDelay() const;

	void setWindRes(const u32 newres);

	u32  getWindRes() const;

	//! set the number of frames in the texture
	void setImageCount(const core::dimension2d<s32>& ic);
	const core::dimension2d<s32>& getImageCount() const;

	//! returns the axis aligned bounding box of this node
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

private:

	void reallocateBuffers();
	bool Load();
	bool Save();
	bool Create();

	core::array<SGrassParticle>   Particles;        // list of particles
	core::vector3d<s32>           gridpos;          // grid position
	f32                           DrawDistanceSQ;   // maximum draw distance, squared for speed
	u32                           MaxDensity;       // maximum blades per patch
	core::dimension2d<s32>        imagecount;       // number of images wide and tall
	core::dimension2d<f32>        imagesize;        // size of each image

	ITerrainSceneNode* const Terrain;
	const video::IImage* const TerrainHeightMap;	// heightmap to get Y value
	const video::IImage* const TerrainColourMap;	// the colour of the grass
	const video::IImage* const TerrainGrassMap; 	// height and density of grass
	scene::IWindGenerator* WindGen;          		// pointer to wind generator
	core::array<core::vector2df>  WindGrid;         // grid of last wind positions
	u32                           windGridRes;      // grid resolution
	u32                           fpsLock;          // how many fps it runs at.
	u32                           lastwindtime;     // last time the wind changed
	bool                          redrawnextloop;   // if we redo the vertex buffers next loop
	u32                           lastdrawcount;    // last number of triangles drawn

	video::SMaterial              Material;         // Grass material
	core::array<video::S3DVertex> Vertices;         // 4 vertices per particle
	core::array<u16>              Indices;          // 12 indices per particle (2 triangles(*3) per side(*2))
	core::aabbox3d<f32>           Box;              // bounding box

	core::array<f32>              v1;               // these are used for holding
	core::array<f32>              v2;               // the tcoords instead of
	core::array<f32>              v3;               // calculating them each loop.
	core::array<f32>              v4;               // coords are set on setImageCount
};

} // end namespace scene
} // end namespace irr

#endif  // __C_GRASS_PATCH_SCENE_NODE_H_INCLUDED__
