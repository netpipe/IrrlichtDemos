#include "SpriteManager.h"

using namespace irr;
using namespace core;
using namespace video;

SpriteManager::SpriteManager(IVideoDriver *driver, ITexture *texture, int size)
        : driver(driver), texture(0), textureIndex(0),
        sprites(0),
        vertices(0),
        indices(0),
        createCount(0),
        maxSprites(0)
{
    setMaterialTexture(texture);
    setMaximumSprites(size);
}

SpriteManager::~SpriteManager()
{
    if (maxSprites > 0)
    {
        delete [] sprites;
        delete [] vertices;
        delete [] indices;
    }
}

void SpriteManager::setup(irr::video::IVideoDriver *d, irr::video::ITexture *texture)
{
    driver = d;
    setMaterialTexture(texture);
}

void SpriteManager::setMaterialTexture(irr::video::ITexture *t, int ti)
{
    textureIndex = ti;
    texture = t;
    material.setTexture(textureIndex, texture);
    material.Wireframe = false;
    material.Lighting = false;
    material.MaterialType = EMT_TRANSPARENT_VERTEX_ALPHA;
}

void SpriteManager::drawAll(float x, float y)
{
    if (createCount == 0 || !texture || !driver)
        return;

    matrix4 mat;
    mat.setTranslation(vector3df(x,y,0));

    int primCount = primCount = 0;

    dimension2d<u32> size = driver->getCurrentRenderTargetSize();

    rectf screenRect(-20,-20,size.Width+20,size.Height+20);
    for (int i = 0; i<createCount; i++)
    {
        if (!sprites[i].isVisible())
            continue;

        sprites[i].transform(mat);

#if 1   // clipping
        // comment out - does more damage in fps?
        S3DVertex *vertices = sprites[i].vertices;
        vector2df lt(vertices[0].Pos.X,vertices[0].Pos.Y);
        vector2df rb(vertices[2].Pos.X,vertices[2].Pos.Y);
        vector2df pc = rb - lt;
        pc *= 0.5;
        lt += pc;

        if (!screenRect.isPointInside(lt))
            continue;
#endif

        // add to render primitive list
        u16 *idx = &indices[primCount * 3];
        // triangle 1
        idx[0] = i * 4;
        idx[1] = i * 4 + 1;
        idx[2] = i * 4 + 3;
        // triangle 2
        idx[3] = i * 4 + 1;
        idx[4] = i * 4 + 2;
        idx[5] = i * 4 + 3;

        primCount+=2;
    }

    if (!primCount)
        return;

    driver->setMaterial(material);

    driver->draw2DVertexPrimitiveList(
        &vertices[0],
        createCount*4,
        &indices[0],
        primCount,
        EVT_STANDARD,
        scene::EPT_TRIANGLES,
        EIT_16BIT);
}

int SpriteManager::getMaximumSprites()
{
    return maxSprites;
}

void SpriteManager::setMaximumSprites(int count)
{
    if (count < 32)
        count = 32;

    if (count != maxSprites) {

        if (maxSprites) {
            delete [] sprites;
            delete [] vertices;
            delete [] indices;
            createCount = 0;
        }

        sprites = new Sprite[count];
        vertices = new S3DVertex[count * 4];
        indices = new u16[count * 4];
    }

    maxSprites = count;
}

Sprite* SpriteManager::addSprite(int sx, int sy, int sw, int sh)
{
    if (createCount == maxSprites)
        return &nullSprite;

    int idx = createCount++;
    Sprite *spr = &sprites[idx];
    spr->init(idx, &vertices[idx * 4], texture->getSize().Width, texture->getSize().Height, sx, sy, sw, sh);

    return spr;
}

Sprite* SpriteManager::getHitSprite(float x, float y)
{
    Sprite *hit = 0;
    for (int i = 0; i<createCount; i++)
    {
        if (!sprites[i].isVisible() || !sprites[i].isEnabled())
            continue;

        if (sprites[i].hitTest(x, y))
            hit = &sprites[i];
    }
    return hit;
}

void SpriteManager::clear()
{
    createCount = 0;
}

