/*-----------------------------------------------------------------------------*
| headerfile ShTlTerrainSceneNode.h                                            |
|                                                                              |
| version 2.00                                                                 |
| date: (29.04.2007)                                                           |
|                                                                              |
| author:  Michal Švantner                                                     |
|                                                                              |
| Shifting Tiled Terrain Scene Node                                            |
|                                                                              |
| This node can render quit large terrain consisting of tiles by storing data  |
| of individual tiles in array and rendering only part of terrain around       |
| defined position and in defined size. This can be position of another scene  |
| node defined by user and terrain node can be set to render around it         |
| automaticaly.                                                                |
| It is done by creating mesh of given size which then glide over invisible    |
| data skelet like a peace of soft cloth, updating its atributes from data     |
| arrays each time it shifts to a new position.                                |
|                                                                              |
| Diferent atributes of individual tiles can be adjusted, most of them on run  |
| time. Namely texture coordinates, position of tile vertices, normals and     |
| color.                                                                       |
|                                                                              |
| Scene node use 2 texture layers.                                             |
| Firsth texture is standard texture which gives detail to terrain close by    |
| and can by assigned to individual tiles by setting their UV coordinates.     |
| User have complete freedom as howe to apply this texture to tiles. Texture   |
| can for example consist of seweral subtextures and each can be set to        |
| diferent tiles using UV coordinates.                                         |
|                                                                              |
| Second texture is created by terrain node internaly and is used to render    |
| color of individual tiles. This adds some variability to terrain at distance.|
| Both textures are blended together using EMT_DETAIL_MAP material type.       |
|                                                                              |
| Normaly textures should be set in oposite order but there are some bugs with |
| updating texture under OpenGL so until it gets solved I have to stick with   |
| detailmap applyed as firsth texture layer. Not perfect but working.          |
|                                                                              |
| This scene node also provide user with functions to get height at given      |
| coordinates and test 3d line for collision useful for line of sight or bullet|
| collision testing.                                                           |
|                                                                              |
| Rendered mesh is split in to seweral sectors which are culled individualy.   |
| This exclude around 60% of polygoons from rendering.                         |
|                                                                              |
| Writen for Irrlicht engine version 1.3                                       |
*-----------------------------------------------------------------------------*/

#ifndef SHTLTERRAINSCENENODE_H
#define SHTLTERRAINSCENENODE_H

#include <irrlicht.h>
using namespace irr;

#include"TlTMesh.h"

// Shifting Tiled Terrain Scene Node class
class ShTlTerrainSceneNode : public scene::ISceneNode
{
    // dimensions of whole terrain
    core::dimension2d<s32> Size;
    
    // bounding box of terrain mesh
    core::aabbox3d<f32> BoundingBox;
    
    // terrain vertex data
    array2d<TlTData> Data;
    
    // terrain tile UV data for 1th texture layer
    array2d<TlTCoords> UVdata;
    
    // terrain mesh sectors
    array2d<TlTSector> Sector;
    
    // array of pointers to vertices of tiles
    array2d<TlTTile> Tile;
    
    // size of vissible terrain mesh
    core::dimension2d<s32> MeshSize;
    
    // position of vissible terrain mesh relative to whole terrain in tiles
    core::vector2d<s32> MeshPosition;
    
    // material
    core::array<video::SMaterial> Material;
    
    // size of terrain tiles
    f32 TileSize;
    
    // node terrain mesh should be rendered around
    scene::ISceneNode* Fnode;
    
    // color texture set as 2nd texture layer
    video::ITexture* CTexture;
    
    // number of sectors rendered last frame
    u32 SectorsRendered;
    
    // howe many tiles should be skiped before terrain mesh gets updated
    s32 ShStep;
    
    // return true if sector is on screen
    virtual bool isSectorOnScreen(TlTSector* sctr);
    
    // update vertices of sector
    virtual void updateVertices(TlTSector &sector);
    
    // update 2nd texture layer
    virtual void updateTexture(u32* p, TlTSector &sector);
    
    // return true if 3d line colide with tile
    virtual bool getIntersectionWithTile(s32 w, s32 h, core::line3d<f32> line,  
        core::vector3df &outIntersection);

public:
    
    // constructor
    // \param smgr -pointer to scene manager
    // \param width - width of terrain in tiles
    // \param height - heighth of terrain in tiles
    // \param tilesize -size of tile
    // \param rendersize -size of rendered terrain mesh in tiles
    // \param parent -parent scene node
    // \param id -ID number
    ShTlTerrainSceneNode(scene::ISceneManager* pSceneManager, s32 width, s32 height, 
        f32 tilesize, s32 rendersize, scene::ISceneNode* parent = 0, s32 id = -1);
    
    // destructor
    ~ShTlTerrainSceneNode();
    
    // frame
    virtual void OnRegisterSceneNode();
    
    // renders terrain
    virtual void render();
    
    // returns the axis aligned bounding box of terrain
    virtual const core::aabbox3d<f32>& getBoundingBox() const;
    
    // recalculate terrain bounding box
    virtual void recalculateBoundingBox();
    
    // returns amount of materials used by terrain
    // this terrain uses only one material
    virtual u32 getMaterialCount();
    
    // returns the material of terrain based on the zero based index
    // \param i -index of material to return
    virtual video::SMaterial& getMaterial(u32 i);
    
    // return number of tiles to skip before terrain mesh gets updated
    virtual s32 getStep();
    
    // set number of tiles to skip before terrain mesh gets updated, default is 1
    // updating slows down rendering and seting step higher will cause terrain to update less ofthen
    // \param newstep -amount of tiles to skip before updating
    virtual void setStep(u32 newstep);
    
    // return dimensions of whole terrain in tiles
    virtual core::dimension2d<s32> getSize();
    
    // return dimension of terrain tile
    virtual f32 getTileSize();
    
    // set new dimensions of terrain tile
    // \param newsize -new size of tile
    virtual void setTileSize(f32 newsize);
    
    // returns dimension of rendered mesh in tiles
    virtual core::dimension2d<s32> getRenderedSize();
    
    // return number of sectors into which terrain mesh is divided
    virtual u32 getSectorCount();
    
    // returns sectors rendered last frame
    virtual u32 getSectorsRendered();
    
    // return relative height of terrain spot at terrain coordinates
    // \param w -width coordinate of spot in tiles
    // \param h -height coordinate of spot in tiles
    virtual f32 getHeight(u32 w, u32 h);
    
    // return height of terrain at any position
    // \param pos -3d coordinates at which to get height of terrain
    virtual f32 getHeight(core::vector3df pos);
    
    // set relative height of terrain spot at terrain coordinates
    // \param w -width coordinate of spot in tiles
    // \param h -height coordinate of spot in tiles
    // \param newheight -new height of spot
    virtual void setHeight(u32 w, u32 h, f32 newheight);
    
    // return normal of terrain at terrain coordinates
    // \param w -width coordinate of spot in tiles
    // \param h -height coordinate of spot in tiles
    virtual core::vector3df getNormal(u32 w, u32 h);
    
    // set normal of terrain at terrain coordinates
    // \param w -width coordinate of spot in tiles
    // \param h -height coordinate of spot in tiles
    // \param newnormal -new normal vector terrain spot
    virtual void setNormal(s32 w, s32 h, core::vector3df newnormal);
    
    // recalculate normal at terrain coordinates
    // \param w -width coordinate of spot in tiles
    // \param h -height coordinate of spot in tiles
    virtual void recalculateNormal(s32 w, s32 h);
    
    // recalculare normals of whole terrain making it look smooth under light
    virtual void smoothNormals();
    
    // get texture coordinates of tile corner
    // \param w -width coordinate of tile
    // \param h -height coordinate of tile
    // \param corner -tile corner, can be LOWER_LEFT, UPPER_LEFT, UPPER_RIGHT, LOWER_RIGHT
    virtual core::vector2d<f32> getTileUV(u32 w, u32 h, TILE_VERTEX corner);
    
    // set texture coordinates of tile
    // \param w -width coordinate of tile
    // \param h -height coordinate of tile
    // \param UVLowerLeft -UV coordinates of lower left corner
    // \param UVUpperLeft -UV coordinates of upper left corner
    // \param UVUpperRight -UV coordinates of upper right corner
    // \param UVLowerRight -UV coordinates of lower right corner
    virtual void setTileUV(u32 w, u32 h, core::vector2d<f32> UVLowerLeft,
        core::vector2d<f32> UVUpperLeft, core::vector2d<f32> UVUpperRight,
        core::vector2d<f32> UVLowerRight);
        
    // stretch texture over whole terrain
    // \param scale -scale of the texture stretched, 2,2 would stretch it twice
    virtual void stretchTexture(core::vector2d<f32> scale = core::vector2d<f32>(1,1));
    
    // stretch texture over every tile individualy
    // \param scale -scale of the texture stretched, 0.5,0.5 would display half of texture over each tile
    virtual void stretchTextureOverTile(core::vector2d<f32> scale = core::vector2d<f32>(1,1));
    
    // rotate texture of tile 90 degrees
    // \param w -width coordinate of tile
    // \param h -height coordinate of tile
    virtual void rotateTileTexture90(u32 w, u32 h);
    
    // rotate texture of tile 180 degrees
    // \param w -width coordinate of tile
    // \param h -height coordinate of tile
    virtual void rotateTileTexture180(u32 w, u32 h);
    
    // rotate texture of tile 270 degrees
    // \param w -width coordinate of tile
    // \param h -height coordinate of tile
    virtual void rotateTileTexture270(u32 w, u32 h);
    
    // flip (mirror) texture of tile horizontaly
    // \param w -width coordinate of tile
    // \param h -height coordinate of tile
    virtual void flipTileTextureHorizontal(u32 w, u32 h);
    
    // flip (mirror) texture of tile verticaly
    // \param w -width coordinate of tile
    // \param h -height coordinate of tile
    virtual void flipTileTextureVertical(u32 w, u32 h);
    
    // get color of tile at terrain coordinates
    // \param w -width coordinate of tile
    // \param h -height coordinate of tile
    virtual video::SColor getColor(u32 w, u32 h);
    
    // set color of tile at terrain coordinates
    // \param w -width coordinate of tile
    // \param h -height coordinate of tile
    // \param newcolor -new color of tile
    virtual void setColor(u32 w, u32 h, video::SColor newcolor);
    
    // set rendered mesh position relative to terrain
    // note that origin of mesh is in its lower left corner
    // \param pos -new position of mesh relative to terrain in tiles
    virtual void setMeshPosition(core::vector2d<s32> pos);
    
    // center rendered mesh at 3d coordinates
    // \param pos -new position mesh should be rendered around
    virtual void centerAt(core::vector3d<f32> pos);
    
    // update rendered mesh
    virtual void update();
    
    // set scene node terrain mesh should be automaticly rendered arround
    // can be camera or player node for example
    // \param node -scene node to automaticaly follow
    virtual void follow(scene::ISceneNode* node);
    
    // cancel pervious function
    // stop following scene node if any
    virtual void stop();
    
    // test if 3d line colide with terrain
    // returns true if yes, false if not and store intersection in "outIntersection" vector
    // \param line -3d line
    // \param outIntersection -vector to store intersection point if any
    virtual bool getIntersectionWithLine( core::line3d<f32> line,  core::vector3df &outIntersection);
    
    // load height data from texture
    // parameters allow to specify place where to load data, which makes possible
    // to load terrain from seweral smaller textures
    // \param filename -filename of texture
    // \param scale -scale to apply at height data, if you want white color to be height of 10.5 set scale to 10.5
    // \param w -width coordinate of tile were to start loading
    // \param h -height coordinate of tile were to start loading
    virtual void loadHeightMap(const c8 *filename, f32 scale, u32 w = 0, u32 h = 0);
    
    // load color data from texture
    // parameters allow to specify place where to load data, which makes possible
    // to load terrain from seweral smaller textures
    // \param filename -filename of texture
    // \param w -width coordinate of tile were to start loading
    // \param h -height coordinate of tile were to start loading
    virtual void loadColorMap(const c8 *filename, u32 w = 0, u32 h = 0);
};
#endif
