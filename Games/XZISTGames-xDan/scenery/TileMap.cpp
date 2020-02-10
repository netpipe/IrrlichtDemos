
#include <TileMap.h>

namespace tilemap {

TileMap::TileMap(ISceneManager *smgr, dSpaceID dSpace, ITexture **textures)
{
    this->smgr = smgr;
    this->dSpace = dSpace;
    this->textures = textures;
    
    // width of each tile
    tileWidth = 8000;
    // number of tiles total (view ahead)
    mapWidth = 5;
    // quads (2 triangles) across tile
    quadsPerTile = 10;//20;
    
    tileCount = mapWidth*mapWidth;
    tiles = new CTileNode*[tileCount];
    tiles2 = new CTileNode*[tileCount];
    
    for (i = 0; i < tileCount; i ++)
    {
        tiles[i] = NULL;
    }
    
    lx = 0;
    ly = 0;
}

TileMap::~TileMap()
{
    for (i = 0; i < tileCount; i ++)
    {
        if (tiles[i])
        {
            tiles[i]->removeAll();
            tiles[i]->remove();
        }
    }
    delete [] tiles;
    delete [] tiles2;
}

void TileMap::setPos(f32 rx, f32 ry, f32 rz)
{
    lx = x;
    ly = y;
    x = makeCoord(rx);
    y = makeCoord(rz);
    height = -ry;
    
    // if changed. remove old,set NULL
    if (x != lx || y != ly) {
        s32 dx = lx-x, dy = ly-y;
        //file with NULL
        for (i = 0; i < tileCount; i ++) {
            tiles2[i] = NULL;
        }
        
        //put old tilemap in middle. IF available.
        for (i = 0; i < mapWidth; i ++) {
            for (j = 0; j < mapWidth; j ++) {
                if ( (i-dy) >= 0 && (i-dy) < mapWidth && (j-dx) >=0 && (j-dx) < mapWidth ) {
                    tiles2[i*mapWidth+j] = tiles[(i-dy)*mapWidth+(j-dx)];
                }
            }
        }
        
        for (i = 0; i < mapWidth; i ++) {
            for (j = 0; j < mapWidth; j ++) {
                if ( (i+dy) >= 0 && (i+dy) < mapWidth && (j+dx) >=0 && (j+dx) < mapWidth ) {
                }
                else {
                    if (tiles[i*mapWidth+j]) {
                        tiles[i*mapWidth+j]->remove();
                        tiles[i*mapWidth+j] = NULL;
                    }
                }
            }
        }
        
        for (i = 0; i < tileCount; i ++) {
            tiles[i] = tiles2[i];
        }
        
    }
    u32 xx = 0, yy = 0;
    // create new tiles if necessary. could be done in a thread.
    for (i = 0; i < tileCount; i ++) {
        if (!tiles[i])
        {            
            /* generate new tile side buffers */
            CTileNode *border[4];
            border[0] = NULL;
            border[1] = NULL;
            border[2] = NULL;
            border[3] = NULL;
            
            // left
            if (xx > 0) border[0] = tiles[yy*mapWidth+xx-1];
            if (yy < mapWidth-1) border[1] = tiles[(yy+1)*mapWidth+xx];
            if (xx < mapWidth-1) border[2] = tiles[yy*mapWidth+xx+1];
            if (yy > 0) border[3] = tiles[(yy-1)*mapWidth+xx];
            
            f32 tx = (i - ((i/mapWidth) * mapWidth)) * tileWidth + tileWidth*x - (tileWidth * (mapWidth/2));
            f32 ty = (i/mapWidth) * tileWidth + tileWidth*y - (tileWidth * (mapWidth/2));
            
            CTileNodeInfo tileInfo;
            tileInfo.dSpace = dSpace;
            /*
            - some prob with new ctilenodes or possibly higher problem but only appearing here.
            - crash no longer happens when either:
                - new CTileNode is no longer used (obviously with tile-> references also disabled
                - rendering is disabled (smgr->drawAll();)
                - tilemap->setPos disabled
            PROBLEM DISCOVERED: prob is with setmaterial texture just below...
            
            PROBLEM SOLVED:
                When level class was created to replace GAME class, I forgot that in the new class
                initialization code was done then the function ended. Whereas with the previous
                class, data was kept as the constructor also performed the main loop.
                The array textures[] needed to be maintained as a class member instead of just
                within the constructor.
            */
            
            tiles[i] = new CTileNode(smgr->getRootSceneNode(), smgr, 0, tileWidth, quadsPerTile, vector3df(tx, height, ty), 
            border, tileInfo);
            
            tiles[i]->drop();
            
            tiles[i]->setMaterialTexture(0, textures[0]);
            tiles[i]->setMaterialFlag(EMF_FOG_ENABLE, true);
            tiles[i]->setMaterialFlag(EMF_LIGHTING, true);
            
            tiles[i]->setPosition(vector3df(tx, height, ty));
            
            // add trees, AFTER positioning tile
            for (j = 0; j < 5; j ++)
            {
                f32 tx2 = rand()%((int)tileWidth);
                f32 ty2 = rand()%((int)tileWidth);
                if ( (rand()%100) < 20)
                {
                    // add some geometry
                    tiles[i]->addGeom(0, vector3df(tx2, tiles[i]->getHeight(tx2,ty2), ty2));
                }
            }
            
            // repeated why hmmmmmmmmmm?
            tiles[i]->setPosition(vector3df(tx, height, ty));
            
            tiles[i]->updateAbsolutePosition();
        }
        xx ++;
        if (xx == mapWidth) { xx = 0; yy ++;}
    }
}

s32 TileMap::makeCoord(f32 pos)
{
    if (pos >= 0) {
        return (int)(pos/tileWidth);
    } else {
        return (int)(pos/tileWidth) - 1;
    }
}

};
