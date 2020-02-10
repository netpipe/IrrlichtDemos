
#ifndef __MAP_H
#define __MAP_H

#include <CTileNode.h>

namespace tilemap {

class TileMapGeom {
public:
    
private:
};

class TileMap {
public:
    TileMap(ISceneManager *, dSpaceID, ITexture **);
    ~TileMap();
    void setPos(f32, f32, f32);
    
private:
    ITexture **textures;
    s32 x, y, lx, ly;
    s32 i, j;
    f32 tileWidth, height;
    u32 mapWidth, tileCount, quadsPerTile;
    s32 makeCoord(f32);
    CTileNode **tiles, **tiles2;
    ISceneManager *smgr;
    dSpaceID dSpace;
};

};

#endif
