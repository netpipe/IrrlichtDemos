#ifndef _EFFECTSMANAGER_H
#define _EFFECTSMANAGER_H

#include "globals.h"
#include "main.h"
#include "explosion.h"

class CEffectsManager
{
    public:
    CEffectsManager(ISceneManager* smgr, IVideoDriver* driver, ITimer* timer);
    void CreateExplosion(vector3df Position);
    void Update();

    void remove();

    private:
    ITimer* timer;
    ISceneManager* smgr;
    IVideoDriver* driver;

    ITexture* TextureFS1;
    ITexture* TextureFS2;
    ITexture* TextureFS3;
    ITexture* TextureFS4;

    list<CExplosion* > Explosions;
};

#endif
