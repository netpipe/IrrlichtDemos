#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "globals.h"
#include "main.h"

class CExplosion
{
    public:
    CExplosion(ISceneManager* smgr, IVideoDriver* driver, ITimer* timer, vector3df Position, ITexture* TextureFS1, ITexture* TextureFS2, ITexture* TextureFS3, ITexture* TextureFS4);
    bool Update();
    void Kill();

    private:
    bool KillMe;
    u32 StartTime;

    ITimer* timer;
    ISceneManager* smgr;
    IVideoDriver* driver;
    vector3df Position;

    IMeshSceneNode* Fireball;
    IParticleSystemSceneNode* FlameSmoke1;
    IParticleSystemSceneNode* FlameSmoke2;
    IParticleSystemSceneNode* FlameSmoke3;
    IParticleSystemSceneNode* FlameSmoke4;
    IParticleSystemSceneNode* Sparks;
    ITexture* TextureFS1;
    ITexture* TextureFS2;
    ITexture* TextureFS3;
    ITexture* TextureFS4;
    ITexture* FireballTexture;
    IVolumeLightSceneNode* LightNode;
};

#endif
