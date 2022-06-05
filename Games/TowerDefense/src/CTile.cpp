#include "CTile.h"
#include "GameVariables.h"
#include "CSprite.h"

CTile::CTile()
{
    //ctor
    Type = ETT_FORBIDDEN;
    Sprite = NULL;
    Around[0] = NULL;
    Around[1] = NULL;
    Around[2] = NULL;
    Around[3] = NULL;
}

CTile::~CTile()
{
    //dtor
    if (Sprite)
    {
        Sprite->remove();
        Sprite->drop();
    }
}

void CTile::init(int x, int y, CTile* a, CTile* b, CTile* c, CTile* d, irr::scene::ISceneManager* manager)
{
    Pos[0] = x;
    Pos[1] = y;
    Position = irr::core::vector3df(x*tile_size+tile_size/2.f, y*tile_size+tile_size/2.f, 5.f);
    setUp(a);
    setRight(b);
    setDown(c);
    setLeft(d);

    Sprite = new CSprite(manager->getRootSceneNode(), manager, -1, Position, irr::core::vector3df(0,0,0), irr::core::vector3df(tile_size, tile_size, 1.f));

    setType(Type);
}

void CTile::setType(E_TILE_TYPE type)
{
    Type = type;
    switch(Type)
    {
        case ETT_BUILD:
            Sprite->setTexture("textures/desert-texture.bmp");
            break;
        case ETT_STREET:
            Sprite->setTexture("textures/street.bmp");
            break;
        case ETT_FORBIDDEN:
            Sprite->setTexture("textures/grass.bmp");
            break;
        case ETT_GOAL:
            Sprite->setTexture("textures/goal.bmp");
            break;
        case ETT_START:
            Sprite->setTexture("textures/start.bmp");
            break;
        case ETT_COUNT:
            setType(ETT_FORBIDDEN);
            break;
        default:
            setType(ETT_FORBIDDEN);
            break;
    }
}

E_TILE_TYPE CTile::getType(void)
{
    return Type;
}

void CTile::select(bool s)
{
    if (s)
        Sprite->getMaterial(0).DiffuseColor = irr::video::SColor(255,255,0,0);
    else
        Sprite->getMaterial(0).DiffuseColor = irr::video::SColor(255,255,0,0);
}

void CTile::setUp(CTile* tile)
{
    Around[0] = tile;
}

void CTile::setRight(CTile* tile)
{
    Around[1] = tile;
}

void CTile::setDown(CTile* tile)
{
    Around[2] = tile;
}

void CTile::setLeft(CTile* tile)
{
    Around[3] = tile;
}

CTile * CTile::getUp(void)
{
    return Around[0];
}

CTile * CTile::getRight(void)
{
    return Around[1];
}

CTile * CTile::getDown(void)
{
    return Around[2];
}

CTile * CTile::getLeft(void)
{
    return Around[3];
}

