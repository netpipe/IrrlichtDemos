#ifndef _SPRITEMANAGER_H
#define _SPRITEMANAGER_H

#include <irrlicht.h>

#include "Sprite.h"

class SpriteManager : public irr::IReferenceCounted {
public:

    SpriteManager(irr::video::IVideoDriver *driver = 0, irr::video::ITexture *texture = 0, int size = 8000);
    ~SpriteManager();

    void setup(irr::video::IVideoDriver *driver, irr::video::ITexture *texture);

    void drawAll(float x = 0, float y = 0);

    int getMaximumSprites();

    void setMaximumSprites(int count);

    void setMaterialTexture(irr::video::ITexture *texture, int id = 1);

    Sprite* addSprite(int sx, int sy, int sw, int sh);

    void clear();

    Sprite* getHitSprite(float x, float y);

protected:

    irr::video::IVideoDriver *driver;
    irr::video::ITexture *texture;

    Sprite *sprites;
    irr::video::S3DVertex *vertices;
    irr::u16 *indices;
    int createCount;
    int maxSprites;
    irr::video::SMaterial material;

    int textureIndex;

    Sprite nullSprite;
};

#endif // _SPRITEMANAGER_H
