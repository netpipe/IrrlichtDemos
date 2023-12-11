#include "explosion.h"

CExplosion::CExplosion(ISceneManager* smgr, IVideoDriver* driver, ITimer* timer, vector3df Position, ITexture* TextureFS1, ITexture* TextureFS2, ITexture* TextureFS3, ITexture* TextureFS4)
{
    CExplosion::smgr = smgr;
    CExplosion::driver = driver;
    CExplosion::Position = Position;
    CExplosion::timer = timer;

    CExplosion::StartTime = timer->getTime();
    CExplosion::KillMe = false;

    CExplosion::TextureFS1 = TextureFS1;
    CExplosion::TextureFS2 = TextureFS2;
    CExplosion::TextureFS3 = TextureFS3;
    CExplosion::TextureFS4 = TextureFS4;
}

bool CExplosion::Update()
{
    if(CExplosion::timer->getTime()-CExplosion::StartTime > 500)
        return false;

    return true;
}

void CExplosion::Kill()
{

}
