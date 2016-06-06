#include "effectsmanager.h"

CEffectsManager::CEffectsManager(ISceneManager* smgr, IVideoDriver* driver, ITimer* timer)
{
    CEffectsManager::smgr = smgr;
    CEffectsManager::driver = driver;
    CEffectsManager::timer = timer;

    CEffectsManager::TextureFS1 = driver->getTexture("./Media/Textures/center1.png");
    CEffectsManager::TextureFS2 = driver->getTexture("./Media/Textures/center2.png");
    CEffectsManager::TextureFS3 = driver->getTexture("./Media/Textures/center3.png");
    CEffectsManager::TextureFS4 = driver->getTexture("./Media/Textures/center4.png");
}

void CEffectsManager::CreateExplosion(vector3df Position)
{
    CExplosion* Explosion = new CExplosion(CEffectsManager::smgr, CEffectsManager::driver, CEffectsManager::timer, Position, CEffectsManager::TextureFS1, CEffectsManager::TextureFS2, CEffectsManager::TextureFS3, CEffectsManager::TextureFS4);
    CEffectsManager::Explosions.push_back(Explosion);
}

void CEffectsManager::Update()
{
    for(list<CExplosion *>::Iterator Iterator = CEffectsManager::Explosions.begin(); Iterator != CEffectsManager::Explosions.end();)
    {
        if(!(*Iterator)->Update())
        {
            (*Iterator)->Kill();
            Iterator = CEffectsManager::Explosions.erase(Iterator);
        }
        else
        {
            Iterator++;
        }
    }
}

void CEffectsManager::remove()
{
    for(list<CExplosion *>::Iterator Iterator = CEffectsManager::Explosions.begin(); Iterator != CEffectsManager::Explosions.end();)
    {
        (*Iterator)->Kill();
        Iterator = CEffectsManager::Explosions.erase(Iterator);
    }
    if(DEBUG_CONSOLE)
        cout<<"Cleaned Effects Manager"<<endl;
}
