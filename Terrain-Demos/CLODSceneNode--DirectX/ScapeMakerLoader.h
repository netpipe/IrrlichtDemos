//!
//! SCAPEMAKELOADER
//! Object for loading files generated from ScapeMaker (http://www.dplate.de/scapemaker/)
//!
//! CSIMPLEWATERPLANE
//! A waterplane capable of texture scrooling, and render lightmaps ( for use with scapemaker
//!
//! By Paulo Rogério de Oliveira ( paulo_cmv@hotmail.com < MSN - EMAIL > )
//! ----------------------------
//! You can use it for any purpose, as you want, only dont remove this notice.
//! If you make any changes, improvements, or you have comments, sugestions,
//! you can report it back by mail or msn. This way i can try to improve the code.


//! PREPARING SCAPEMAKER

//! Textures must be .tga or .png. Convert your objects in ScapeMaker\Objects before starting terrain creation
//! Must have lightmap.jpg, waterlightmap.jpg, water.jpg and heightmap.png in engine folder
//! Must have terraintexture.bmp in engine/textures folder
//! objects info are copied from engine/objects folder

//! NOTES WHEN LOADING SCAPEMAKER MAPS

//! NOTE THIS DONT LOAD THE .ZIP FILE EXPORTED FROM SCAPEMAKER, it loads some files created
//! from scapemaker that are inside landscapes/<mapname>/ path: i.e: scapemaker/landscapes/river
//! for river map
//! note also that not all this files are necessary, others must be exported by hand

//! 1. Export heightmap.png (256x256 pixels) from Topogray tab and put it under <mapname>/engine folder
//! 2. In Environment tab, uncheck Ignore Objects and generate it. Export textures with lightmaps
//! and put it in <mapname>engine/textures folder
//! 3. In Environment tab, check Ignore Objects and generate it again. This way, you will have a lightmap.jpg
//! file without shadows of trees. This shadows interferes when loader get a pixel from lightmap to
//! determine the overal color (vertex colors) of objects. If you get a lightmap with objects shadow
//! then the loader will pick a pixel under the tree position, that will be always dark (shadow)

//! I choosed to edit lightmap.jpg ( smooth and improve brightness & contrast ) to have better looking objects ( not to dark )

//! SKYBOX
//! Scapemaker have a skybox generating script, so you can use it.
//! rename your skybox textures like river_sb_top.bmp and put it in engine folder
//! top texture must be rotated 90º right
//! bottom texture must be rotated 270º right
//! in future, i will try to correct the skybox script

//! FOR MORE INFO ABOUT NECESSARY FILES, TAKE A LOOK AT RIVER FOLDER

#include <irrlicht.h>

#include <string.h>
#include "SceneReader.h"

#include "CQuadTreeSceneNode.h"
#include "CLODSceneNode.h"


class CSimpleWaterSceneNode : public scene::ISceneNode
{
public:

    CSimpleWaterSceneNode( IrrlichtDevice* Device, s32 ID, s32 upPosition, ITerrainSceneNode* terrain, const c8* mapname );

    ~CSimpleWaterSceneNode();

    video::SMaterial& getMaterial(s32 i);

    s32 getMaterialCount();

    const core::aabbox3d<f32>& getBoundingBox() const;

    void OnPreRender();

    void render();

private:
    core::aabbox3df BBox;
    IrrlichtDevice* device;
    video::SMaterial material;
    video::IVideoDriver* driver;
    f32 watx;
    int transp;
    int rgb;
    f32 uppos;
    video::S3DVertex2TCoords vertices[4];
    core::matrix4 mat;
    u16 indices[6];
};

class ScapeMakeLoader
{
private:
    IrrlichtDevice* device;
    const c8* levelname;
    scene::ITerrainSceneNode* terrain;
    f32 terrainscale;

    float getTerrainHeight( video::IImage* TerrainHeightMap, scene::ITerrainSceneNode* Terrain, core::vector3df p );

    core::stringc changeFileExt( core::stringc filename, core::stringc newExt );

    void LoadObjects( video::IImage* TerrainHeightMap, video::IImage* TerrainLightMap );

    scene::ITerrainSceneNode* LoadTerrain( const c8* TextureName, const c8* TerrainHeightMapFileName );

public:

    ScapeMakeLoader( IrrlichtDevice* Device );

    ~ScapeMakeLoader();

    scene::ITerrainSceneNode* getTerrain();

    void LoadScape( const c8* levelfile, s16 scale );
};
